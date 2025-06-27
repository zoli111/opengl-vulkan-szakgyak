cmake_minimum_required(VERSION 3.5)
project(haromszog)
add_executable(haromszog haromszog.cpp)
target_link_libraries(haromszog glfw GL)
