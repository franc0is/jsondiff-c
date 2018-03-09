cmake_minimum_required(VERSION 3.1)
project(jsondiff-c)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_C_COMPILER "g++-7")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "g++-7")

add_library(jsondiff src/jsondiff.cpp)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(jsondiff ${CONAN_LIBS})

add_executable(jsondiff-tool "${PROJECT_SOURCE_DIR}/examples/jsondiff-tool/jsondiff-tool.c")
target_link_libraries(jsondiff-tool jsondiff)
target_link_libraries(jsondiff-tool ${CONAN_LIBS})

enable_testing()
add_executable(jsondiff-tests test/test_main.cpp test/test_apply.cpp test/test_diff.cpp)
target_link_libraries(jsondiff-tests ${CONAN_LIBS})
target_link_libraries(jsondiff-tests jsondiff)
add_test(NAME run-tests
         COMMAND jsondiff-tests)