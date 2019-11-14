cmake_minimum_required(VERSION 3.7)
project(IZ2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -ggdb3")


set(LIB_SOURCE_FILES multi_proc.c single_proc.c)
#создаем статическую библиотеку и поодключаем ее к цели IZ2_static


add_library(st_lib STATIC ${LIB_SOURCE_FILES})
target_include_directories(st_lib PUBLIC ${PROJECT_SOURCE_DIR}/)
add_executable(IZ2_static main_static.c)
target_link_libraries(IZ2_static PUBLIC st_lib pthread)
#создаем динамическую библиотеку и поодключаем ее к цели IZ2_dynamic


add_library(dyn_lib SHARED ${LIB_SOURCE_FILES})
target_include_directories(dyn_lib PUBLIC ${PROJECT_SOURCE_DIR}/)#динамическая
add_executable(IZ2_dynamic main_dynamic.c)
target_link_libraries(IZ2_dynamic PRIVATE dyn_lib ${CMAKE_DL_LIBS})

#подрубаем GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(tests_gtest tests_gtest.cpp)
target_link_libraries(tests_gtest PUBLIC ${GTEST_LIBRARIES} pthread st_lib)