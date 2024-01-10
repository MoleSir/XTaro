cmake_minimum_required(VERSION 3.0)

project(xtaro-main)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

add_executable(xtaro-main main.cc xtaro.cc)
target_link_libraries(xtaro-main global character parse circuit)
