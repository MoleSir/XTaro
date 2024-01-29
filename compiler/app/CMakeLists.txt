cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(xtaro)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

include_directories(${GLOBAL_PATH}/)
include_directories(${SIMULATE_PATH}/)
include_directories(${CIRCUIT_PATH}/)
include_directories(${CHARACTER_PATH}/)
include_directories(${PARSE_PATH}/)
include_directories(${APP_PATH}/)

file(GLOB HELP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/help/*.cc)
file(GLOB INTERACTIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/interactive/*.cc)
file(GLOB SCRIPT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/script/*.cc)
file(GLOB VERSION_SRC ${CMAKE_CURRENT_SOURCE_DIR}/version/*.cc)
file(GLOB XTARO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/xtaro/*.cc)
set(SRC ${HELP_SRC} ${INTERACTIVE_SRC} ${SCRIPT_SRC} ${VERSION_SRC} ${XTARO_SRC} main.cc)

add_executable(xtaro ${SRC})
target_link_libraries(xtaro global character parse circuit)
