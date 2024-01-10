cmake_minimum_required(VERSION 3.0)
project(parse)

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)

# file(GLOB GDS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/gds/*.cc)
file(GLOB LIBERTY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/liberty/*.cc)
file(GLOB JSON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/json/*.cc)
set(SRC ${GDS_SRC} ${LIBERTY_SRC} ${JSON_SRC})

add_library(parse STATIC ${SRC})
target_link_libraries(parse global)
