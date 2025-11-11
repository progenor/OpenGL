cmake_minimum_required(VERSION 3.10)
project(Project)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Add the executable
add_executable(Project Project.cc)

# Link against required libraries
# Add GLU to the list of libraries
target_link_libraries(Project ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)