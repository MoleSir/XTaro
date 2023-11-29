cmake_minimum_required(VERSION 3.0)

project(xtaro-main)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

add_executable(xtaro-main main.cpp)
target_link_libraries(xtaro-main parser-gds global)
