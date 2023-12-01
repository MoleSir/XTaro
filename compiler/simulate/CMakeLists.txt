cmake_minimum_required(VERSION 3.0)
project(simulate)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${GLOBAL_PATH}/)

file(GLOB COMMAND_SRC ${CMAKE_CURRENT_SOURCE_DIR}/command/*.cc)
set(SRC ${COMMAND_SRC} ./simulator/simulator.cc )

add_library(simulate STATIC ${SRC})
target_link_libraries(simulate global)