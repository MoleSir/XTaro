cmake_minimum_required(VERSION 3.0)
project(XTaro)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/lib)
set(EXEC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)

add_subdirectory(compiler/apps)
add_subdirectory(compiler/parser)
add_subdirectory(compiler/test)


