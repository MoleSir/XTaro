cmake_minimum_required(VERSION 3.0)

project(xtaro-main)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

file(GLOB INTERPRETER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/interpreter/*.cc)
file(GLOB XTARO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/xtaro/*.cc)
set(SRC ${INTERPRETER_SRC} ${XTARO_SRC} main.cc)

add_executable(xtaro-main ${SRC})
target_link_libraries(xtaro-main global character parse circuit)
