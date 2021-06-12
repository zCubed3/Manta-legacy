set(SOURCES
   "src/main.cpp"

   "src/modular/dynamiclib.cpp"

   "public/console.cpp"
)

set(INCLUDE_DIRS
   "public"
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
