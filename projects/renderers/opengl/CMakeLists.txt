set(SOURCES 
   "src/libmain.cpp"

   "src/opengl3_api.cpp"

   "${ENGINE_DIR}/public/console.cpp"
)

set(INCLUDE_DIRS
   "public"
   "${ENGINE_DIR}/public"   
)

# Windows isn't as good with libraries as *nix systems
if(WIN32)
   list(APPEND INCLUDE_DIRS "include")
   list(APPEND LINK_DIRS "lib")
else()
   set(LIBRARIES "glfw")
   
endif()

add_library(MantaGL3 SHARED ${SOURCES})

target_include_directories(MantaGL3 PUBLIC ${INCLUDE_DIRS})
target_link_directories(MantaGL3 PUBLIC ${LINK_DIRS})

set_target_properties(MantaGL3 PROPERTIES
   OUTPUT_NAME "OpenGL3_api"
)
