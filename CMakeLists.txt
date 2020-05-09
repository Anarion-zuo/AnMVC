cmake_minimum_required(VERSION 3.10)
project(MyMVC)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(src)
add_executable(MyMVC main.cpp)

include_directories(include)
include_directories(include/base)
# anarion base
target_link_libraries(MyMVC anarionmvc)
