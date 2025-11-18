cmake_minimum_required(VERSION 3.10)
Project2(Project2)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Add the executable
add_executable(Project2 Project2.cc)

# Link against required libraries
# Add GLU to the list of libraries
target_link_libraries(Project2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)