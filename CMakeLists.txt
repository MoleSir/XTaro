cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(XTaroProject)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/lib)
set(EXEC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)

set(APP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/app)
set(GLOBAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/global)
set(SIMULATE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/simulate)
set(CIRCUIT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/circuit)
set(CHARACTER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/character)
set(PARSE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/parse)

add_subdirectory(compiler/app)
add_subdirectory(compiler/global)
add_subdirectory(compiler/character)
add_subdirectory(compiler/parse)
add_subdirectory(compiler/circuit)
