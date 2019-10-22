cmake_minimum_required(VERSION 3.7)
project(IZ2)

set(CMAKE_CXX_STANDARD 14)
add_library(single_thread_libr STATIC single_thread.c)
target_include_directories(single_thread_libr PUBLIC ${PROJECT_SOURCE_DIR}/)#статическая

add_library(multi_thread_libr SHARED multi_thread.c)
target_include_directories(multi_thread_libr PUBLIC ${PROJECT_SOURCE_DIR}/)#динамическая
add_executable(IZ2 main.c)
target_link_libraries(IZ2 PRIVATE single_thread_libr)
target_link_libraries(IZ2 PRIVATE multi_thread_libr pthread)


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(tests_gtest tests_gtest.cpp)
target_include_directories(tests_gtest PUBLIC ${PROJECT_SOURCE_DIR}/)
## links gtest
target_link_libraries(tests_gtest ${GTEST_LIBRARIES} pthread single_thread_libr multi_thread_libr)