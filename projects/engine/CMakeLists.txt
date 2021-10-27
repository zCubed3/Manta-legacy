set(SOURCES
        "src/main.cpp"

        "src/console/console.cpp"
        "src/console/common_console.cpp"

        "src/assets/manta_packer.cpp"
        "src/assets/texture.cpp"
        "src/assets/model.cpp"
        "src/assets/shader.cpp"
        "src/assets/resources.cpp"
        "src/assets/material.cpp"

        "src/assets/material_values/material_color.cpp"
        "src/assets/material_values/material_float.cpp"

        "src/actors/actor.cpp"
        "src/actors/world.cpp"

        "src/actors/engine/camera.cpp"
        "src/actors/engine/light.cpp"
        "src/actors/engine/skybox.cpp"

        "src/components/component.cpp"

        "src/components/engine/renderer.cpp"

        "src/rendering/opengl/glrenderer.cpp"
        "src/rendering/opengl/assets/glmodel.cpp"
        "src/rendering/opengl/assets/glshader.cpp"
        "src/rendering/opengl/assets/gltexture.cpp"

        "${DEPENDS_DIR}/imgui/imgui.cpp"
        "${DEPENDS_DIR}/imgui/imgui_internal.h"
        "${DEPENDS_DIR}/imgui/imgui_draw.cpp"
        "${DEPENDS_DIR}/imgui/imgui_tables.cpp"
        "${DEPENDS_DIR}/imgui/imgui_widgets.cpp"
        "${DEPENDS_DIR}/imgui/backends/imgui_impl_glfw.cpp"
        "${DEPENDS_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
        "${DEPENDS_DIR}/imgui/misc/cpp/imgui_stdlib.cpp"
        )

set(INCLUDE_DIRS
        "${DEPENDS_DIR}"
        "${ENGINE_DIR}/src"
        )

if (DEFINED WIN32)
    set(LIBRARIES)
else ()
    #find_package(PkgConfig REQUIRED)

    #pkg_check_modules(MONOPKG REQUIRED mono-2)

    #${MONOPKG_LIBRARIES}
    #${MONOPKG_INCLUDE_DIRS}

    set(LIBRARIES "GL" "dl" "glfw" "GLEW")
    list(APPEND INCLUDE_DIRS)
endif ()

add_executable(MantaEngine ${SOURCES})
target_include_directories(MantaEngine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(MantaEngine PUBLIC ${LIBRARIES})

set_target_properties(MantaEngine PROPERTIES
        OUTPUT_NAME "Manta"
        )

