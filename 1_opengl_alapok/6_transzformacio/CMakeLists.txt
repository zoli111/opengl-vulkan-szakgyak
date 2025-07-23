cmake_minimum_required(VERSION 3.5)
project(traszformacio)
add_executable(traszformacio main.cpp texture.cpp)
target_link_libraries(traszformacio glfw GL)

configure_file(${CMAKE_SOURCE_DIR}/vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/vertex.glsl COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/fragment.glsl COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/foreground.png
    ${CMAKE_CURRENT_BINARY_DIR}/foreground.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/background.jpg
    ${CMAKE_CURRENT_BINARY_DIR}/background.jpg COPYONLY)
