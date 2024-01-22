cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(global)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

file(GLOB ALLOC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/allocator/*.cc)
file(GLOB CONFIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/config/*.cc)
file(GLOB EXCEPTION_SRC ${CMAKE_CURRENT_SOURCE_DIR}/exception/*.cc)
file(GLOB DEBUG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/debug/*.cc)
file(GLOB STRING_POOL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/stringpool/*.cc)
file(GLOB TECH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/tech/*.cc)
file(GLOB UTIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cc)
set(SRC ${ALLOC_SRC} ${CONFIG_SRC} ${EXCEPTION_SRC} ${DEBUG_SRC} ${STRING_POOL_SRC} ${TECH_SRC} ${UTIL_SRC})

add_library(global STATIC ${SRC})
link_libraries(global parse)
