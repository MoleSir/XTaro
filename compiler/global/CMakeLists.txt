cmake_minimum_required(VERSION 3.0)
project(global)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

add_library(global STATIC ./allocator/allocator.cc ./log/log.cc ./global.cc)