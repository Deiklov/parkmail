cmake_minimum_required(VERSION 3.7)
project(IZ2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")
add_library(single_st_lib STATIC single_proc.c)
target_include_directories(single_st_lib PUBLIC ${PROJECT_SOURCE_DIR}/)#статическая
add_executable(IZ2_static main_static.c)
target_link_libraries(IZ2_static PUBLIC single_st_lib pthread)
#########
add_library(multi_dyn_lib SHARED multi_proc.c)
target_include_directories(multi_dyn_lib PUBLIC ${PROJECT_SOURCE_DIR}/)#динамическая
add_executable(IZ2_dynamic main_dynamic.c)
target_link_libraries(IZ2_dynamic PRIVATE multi_dyn_lib ${CMAKE_DL_LIBS})



find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(tests_gtest tests_gtest.cpp)
#target_include_directories(tests_gtest PUBLIC ${PROJECT_SOURCE_DIR}/)
## links gtest
target_link_libraries(tests_gtest PUBLIC ${GTEST_LIBRARIES} pthread single_st_lib)
target_link_libraries(tests_gtest PRIVATE ${CMAKE_DL_LIBS} multi_dyn_lib)