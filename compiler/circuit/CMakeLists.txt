cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(circuit)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

file(GLOB BASE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/base/*.cc)
file(GLOB MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/module/*.cc)
file(GLOB FACTORY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/factory/*.cc)
set(SRC ${BASE_SRC} ${MODULE_SRC} ${FACTORY_SRC})

add_library(circuit STATIC ${SRC})
target_link_libraries(circuit global)