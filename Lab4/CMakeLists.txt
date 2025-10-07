cmake_minimum_required(VERSION 3.10)
project(Lab4)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Add the executable
add_executable(Lab4 Lab4.cc)

# Link against required libraries
# Add GLU to the list of libraries
target_link_libraries(Lab4 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)