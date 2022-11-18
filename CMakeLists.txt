cmake_minimum_required(VERSION 3.12)
project(tmpl_sort)


add_executable(a.out main.cpp)
set_property(TARGET a.out PROPERTY CXX_STANDARD 20)
target_include_directories(a.out PRIVATE ${CMAKE_SOURCE_DIR})
