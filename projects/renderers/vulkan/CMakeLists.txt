set(SOURCES ) # Currently doesn't work!

set(INCLUDE_DIRS
   "public"
   "${ENGINE_DIR}/public"
   "${DEPENDS_DIR}"
)

# Windows isn't as good with libraries as *nix systems
if(WIN32)
   list(APPEND INCLUDE_DIRS "include")
   list(APPEND LINK_DIRS "lib")
else()
   set(LIBRARIES "glfw")
   
endif()

add_library(MantaVK SHARED ${SOURCES})

target_include_directories(MantaVK PUBLIC ${INCLUDE_DIRS})
target_link_directories(MantaVK PUBLIC ${LINK_DIRS})

set_target_properties(MantaVK PROPERTIES
   OUTPUT_NAME "Vulkan_api"
)
