set(SOURCES
   "main.cpp"
)

set(INCLUDE_DIRS
   "public"
)

add_executable(AuroraEngine ${SOURCES})
target_include_directories(AuroraEngine PUBLIC ${INCLUDE_DIRS})
