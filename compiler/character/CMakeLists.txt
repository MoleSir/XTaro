cmake_minimum_required(VERSION 3.0)
project(character)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(character STATIC ${SRC})
target_link_libraries(character global simulate)