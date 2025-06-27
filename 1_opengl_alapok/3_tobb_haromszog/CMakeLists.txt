cmake_minimum_required(VERSION 3.5)
project(haromszogek)
add_executable(haromszogek haromszogek.cpp)
target_link_libraries(haromszogek glfw GL)
