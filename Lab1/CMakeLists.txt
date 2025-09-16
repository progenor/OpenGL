cmake_minimum_required(VERSION 3.10)
project(Pontok)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Pontok Pontok.cc)
target_link_libraries(Pontok OpenGL::GL GLUT::GLUT)
