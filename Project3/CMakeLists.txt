cmake_minimum_required(VERSION 3.10)
project(Project3)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Add the executable
add_executable(Project3 Project3.cc)

# Link against required libraries
target_link_libraries(Project3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} GLU glm)
