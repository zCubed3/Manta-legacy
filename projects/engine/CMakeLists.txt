set(SOURCES
   "src/main.cpp"
   
   "src/modular/dynamiclib.cpp"
)

set(INCLUDE_DIRS
   "public"
)

if(DEFINED WIN32)
   set(LIBRARIES)
else()
   set(LIBRARIES "dl")
endif()

add_executable(MantaEngine ${SOURCES})
target_include_directories(MantaEngine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(MantaEngine PUBLIC ${LIBRARIES})

set_target_properties(MantaEngine PROPERTIES
   OUTPUT_NAME "Manta"
)
