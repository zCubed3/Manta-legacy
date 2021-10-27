#include "monoruntime.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/blob.h>
#include <mono/metadata/image.h>
#include <mono/metadata/row-indexes.h>

#include <manta_macros.hpp>

#include <filesystem>

using namespace std::filesystem;

#include <imgui/imgui.h>

// Useless normally, only here so I don't throw up over repeated code
void mono_printf(const char *text) {
    printf("[Mono]: %s", text);
}

void MonoScriptingBackend::Initialize() {
    mono_printf("Initializing Mono scripting backend!\n");

    mantaDomain = mono_jit_init("MantaMono");

    mono_printf("Registering C->C# bindings...\n");
    RegisterCBindings();

    LoadAssemblies();

    mono_printf("Checking integrity of runtime!\n");
    TestRuntime();
}

void MonoScriptingBackend::LoadAssemblies() {
    // Folder should be at ./bin
    for (auto file: directory_iterator("./bin")) {
        printf("[Mono]: Attempting to open Mono Assembly at %s\n", file.path().c_str());

        MonoAssembly *assembly = mono_domain_assembly_open(mantaDomain, file.path().c_str());
        if (!assembly) {
            printf("[Mono]: Failed to open...\n");
            continue;
        }

        std::string name = file.path().stem().c_str();
        loadedAssemblies.emplace(name, assembly);
        printf("[Mono]: Opened and registered as %s!\n", name.c_str());
    }
}

// Testing function, allows us to check if mono is working correctly
void MonoScriptingBackend::TestRuntime() {
    auto iter = loadedAssemblies.find("Manta.Internal");

    if (iter != loadedAssemblies.end()) {
        MonoAssembly *assembly = iter->second;

        MonoImage *mantaImage = mono_assembly_get_image(assembly);
        MonoClass *klass = mono_class_from_name(mantaImage, "MantaInternal", "MantaTests");

        if (!klass) {
            mono_printf(
                    "Failed to find MantaInternal::MantaTests in compiled Manta dll!\nSomething either went wrong with the path or this build is broken!\n");
            return;
        }

        MonoMethodDesc *testMethodDesc = mono_method_desc_new(":__test_method()", false);
        MonoMethod *testMethod = mono_method_desc_search_in_class(testMethodDesc, klass);
        mono_method_desc_free(testMethodDesc);

        if (!testMethod) {
            mono_printf("Failed to find MantaTests::__test_method()\n");
            return;
        }

        MonoObject *exception;

        mono_runtime_invoke(testMethod, nullptr, {}, &exception);
    }
}

// C -> C# bindings
static void mantamono_printf(MonoString *string) {
    printf("%s", mono_string_to_utf8(string));
}

void manta_register_mono_method(MonoScriptingBackend *backend, const char *methodName, void *cfunc) {
    std::string fullName = "MantaInternal.CBindings::__c__";
    fullName += methodName;

    if (backend) {
        if (backend->debug) {
            printf("[Mono]: [Debug] Registered C->C# for %s\n", fullName.c_str());
        }
    }

    mono_add_internal_call(fullName.c_str(), cfunc);
}

void MonoScriptingBackend::RegisterCBindings() {
    manta_register_mono_method(this, "printf", (void *) mantamono_printf);
}

// Debugging window, used to explore types and other things defined by loaded assemblies

void MonoScriptingBackend::DrawDebugWindow() {
    if (!showWindow)
        return;

    ImGui::Begin("Mono Debug");

    if (ImGui::TreeNode("Assembly Explorer")) {
        for (auto pair: loadedAssemblies) {
            if (ImGui::TreeNode(pair.first.c_str())) {
                ImGui::PushID(pair.first.c_str());
                if (ImGui::Button("Populate Debug Info")) {
                    // Based on https://stackoverflow.com/questions/47502846/getting-the-assembly-classes-using-mono-in-c
                    auto iter = loadedAssembliesDebugInfo.find(pair.second);

                    MonoAssemblyDebugInfo *debugInfo;
                    if (iter == loadedAssembliesDebugInfo.end()) {
                        debugInfo = new MonoAssemblyDebugInfo;
                        loadedAssembliesDebugInfo.emplace(pair.second, debugInfo);
                    } else
                        debugInfo = iter->second;

                    MonoImage *image = mono_assembly_get_image(pair.second);
                    const MonoTableInfo *imageTypedefInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
                    int rows = mono_table_info_get_rows(imageTypedefInfo);
                    for (int r = 0; r < rows; r++) {
                        uint32_t columns[MONO_TYPEDEF_SIZE];
                        mono_metadata_decode_row(imageTypedefInfo, r, columns, MONO_TYPEDEF_SIZE);
                        const char *typeName = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAME]);
                        const char *typeNamespace = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAMESPACE]);

                        auto namespaceIter = debugInfo->classes.find(std::string(typeNamespace));

                        std::vector<std::string> *classes = nullptr;
                        if (namespaceIter == debugInfo->classes.end()) {
                            classes = new std::vector<std::string>;
                            debugInfo->classes.emplace(std::string(typeNamespace), classes);
                        } else
                            classes = namespaceIter->second;

                        if (classes != nullptr) {
                            classes->emplace_back(std::string(typeName));
                        }
                    }
                }

                for (auto debugInfo: loadedAssembliesDebugInfo) {
                    for (auto pair: debugInfo.second->classes) {
                        if (ImGui::TreeNode(pair.first.c_str())) {
                            for (auto entry: *pair.second) {
                                ImGui::Text("%s::%s", pair.first.c_str(), entry.c_str());
                            }

                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::TreePop();
                ImGui::PopID();
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();
}
