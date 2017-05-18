cmake_minimum_required(VERSION 3.7)
project(PBMCompressor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/main.c src/fileManagement.c src/mersenneTwister.c src/pointManagement.c src/tsp.c src/structures.c)
add_executable(PBMCompressor ${SOURCE_FILES})
target_link_libraries(PBMCompressor m)