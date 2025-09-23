cmake_minimum_required(VERSION 3.10)
project(Lab2)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Lab2 Lab2.cc)
target_link_libraries(Lab2 OpenGL::GL GLUT::GLUT)
