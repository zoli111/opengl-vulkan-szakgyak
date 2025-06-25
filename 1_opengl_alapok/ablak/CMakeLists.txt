cmake_minimum_required(VERSION 3.5)
project(ablak)
add_executable(ablak ablak.cpp)
target_link_libraries(ablak glfw GL)
