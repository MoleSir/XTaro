cmake_minimum_required(VERSION 3.0)
project(parser-gds)

aux_source_directory(./ SRC)
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

add_library(parser-gds STATIC ${SRC})