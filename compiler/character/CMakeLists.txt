cmake_minimum_required(VERSION 3.0)
project(character)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

file(GLOB COMMAND_SRC ${CMAKE_CURRENT_SOURCE_DIR}/command/*.cc)
file(GLOB DELAY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/delay/*.cc)
file(GLOB FUNCTION_SRC ${CMAKE_CURRENT_SOURCE_DIR}/function/*.cc)
# file(GLOB SETUPHOLD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/setuphold/*.cc)
file(GLOB SIMULATOR ${CMAKE_CURRENT_SOURCE_DIR}/simulator/*.cc)
set(SRC ${COMMAND_SRC} ${DELAY_SRC} ${FUNCTION_SRC} ${SIMULATOR})

add_library(character STATIC ${SRC})
link_libraries(character global simulate)