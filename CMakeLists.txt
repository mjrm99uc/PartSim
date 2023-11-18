cmake_minimum_required(VERSION 3.23)
cmake_policy(VERSION 3.23)
project(PartSim VERSION 0.1.0 DESCRIPTION "N-Body simulator with either brute force or Barnes-Hut algorithm, parallelized with OpenMP and rendered with OpenGL.")

add_executable(PartSim
  main.cpp
  BHTree.cpp
  Particle.cpp
  Space.cpp
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenMP REQUIRED)

target_compile_options(PartSim PRIVATE -Wall -Wextra -Wno-unknown-pragmas)

target_include_directories(PartSim PRIVATE src include)

target_link_libraries(PartSim PRIVATE OpenGL::GL OpenGL::GLX GLUT::GLUT OpenMP::OpenMP_CXX)
