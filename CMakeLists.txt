project(jsondiff-c)
cmake_minimum_required(VERSION 2.8.12)
#add_definitions(-std=c++11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(jsondiff src/jsondiff.cpp)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(jsondiff ${CONAN_LIBS})

add_executable(jsondiff-tool "${PROJECT_SOURCE_DIR}/examples/jsondiff-tool/jsondiff-tool.c")
target_link_libraries(jsondiff-tool jsondiff)
target_link_libraries(jsondiff-tool ${CONAN_LIBS})