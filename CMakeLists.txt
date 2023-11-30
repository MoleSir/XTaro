cmake_minimum_required(VERSION 3.0)
project(XTaro)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/lib)
set(EXEC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)

set(GLOBAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/compiler/global)

add_subdirectory(compiler/app)
add_subdirectory(compiler/global)
add_subdirectory(compiler/simulate)


