set(SOURCES
   "src/main.cpp"

   "src/modular/dynamiclib.cpp"

   "public/console.cpp"

   "public/assets/model.cpp"
   "public/assets/shader.cpp"

   "public/common/common_console.cpp"

   "src/entities/camera.cpp"
)

set(INCLUDE_DIRS
   "public"
   "${DEPENDS_DIR}"
)

if(DEFINED WIN32)
   set(LIBRARIES)
else()
   set(LIBRARIES "dl" "glfw") # GLFW is in here since we use generally

   if ("COMPILE_GL" IN_LIST RENDER_APIS)
      message("Linking GLEW to engine for OpenGL support")
      list(APPEND LIBRARIES "GLEW")
   endif()
endif()

add_executable(MantaEngine ${SOURCES})
target_include_directories(MantaEngine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(MantaEngine PUBLIC ${LIBRARIES})

set_target_properties(MantaEngine PROPERTIES
   OUTPUT_NAME "Manta"
)

# Copies default engine content to binary directory post-build
add_custom_command(TARGET MantaEngine POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECTS_DIR}/engine_data" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data"
   COMMENT "Copied default engine content to binary directory!"
)
