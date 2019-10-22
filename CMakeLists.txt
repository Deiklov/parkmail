cmake_minimum_required(VERSION 3.10)
project(IZ2)

set(CMAKE_CXX_STANDARD 17)
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
##############
add_executable(IZ2 main.c)
set(LIB_SOURCE_FILES single_thread.c multi_thread.c)
add_library(static_single STATIC ${LIB_SOURCE_FILES})# сделали статическую библиотеку
target_link_libraries(IZ2 static_single pthread)#собрали бинарь с использов. стат либ


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(tests_gtest tests_gtest.cpp)

# links gtest
target_link_libraries(tests_gtest ${GTEST_LIBRARIES} pthread)
target_link_libraries(tests_gtest static_single)
target_link_libraries(IZ2 Threads::Threads)