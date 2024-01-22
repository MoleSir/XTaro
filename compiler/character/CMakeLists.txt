cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(character)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

file(GLOB COMMAND_SRC ${CMAKE_CURRENT_SOURCE_DIR}/command/*.cc)
file(GLOB SIMULATOR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/simulator/*.cc)
file(GLOB CHARACTER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/character/*.cc)
set(SRC ${COMMAND_SRC} ${SIMULATOR_SRC} ${CHARACTER_SRC})

add_library(character STATIC ${SRC})
link_libraries(character global simulate)