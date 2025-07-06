cmake_minimum_required(VERSION 3.5)
project(shader)
add_executable(shader main.cpp)
target_link_libraries(shader glfw GL)

# shader fajlok masolasa a futtathato program melle
# minden make utan lefut, ha volt modositva
configure_file(${CMAKE_SOURCE_DIR}/vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/vertex.glsl COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/fragment.glsl COPYONLY)
