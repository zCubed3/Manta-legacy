set(SOURCES 
   "src/libmain.cpp"
)

set(INCLUDE_DIRS
   "public"
   "${PROJECTS_DIR}/engine/public"   
)

add_library(MantaGL3 SHARED ${SOURCES})
target_include_directories(MantaGL3 PUBLIC ${INCLUDE_DIRS})
set_target_properties(MantaGL3 PROPERTIES
   OUTPUT_NAME "OpenGL3_api"
)
