cmake_minimum_required(VERSION 3.5)
project(glfw_alapok)
add_executable(program main.cpp)
target_link_libraries(program glfw GL)
