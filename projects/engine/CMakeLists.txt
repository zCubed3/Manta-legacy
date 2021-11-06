set(SOURCES
        "${DEPENDS_DIR}/imgui/imgui.cpp"
        "${DEPENDS_DIR}/imgui/imgui_demo.cpp"
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

set(SOURCE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(SOURCE_EXTENSIONS
        "*.cpp"
)

get_sources(${SOURCE_DIRS} ${SOURCE_EXTENSIONS} GLOB_SOURCES)

add_executable(MantaEngine ${GLOB_SOURCES} ${SOURCES})
target_include_directories(MantaEngine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(MantaEngine PUBLIC ${LIBRARIES})

set_target_properties(MantaEngine PROPERTIES
        OUTPUT_NAME "Manta"
        )

