cmake_minimum_required(VERSION 3.23)
cmake_policy(VERSION 3.23)
project(presentacion VERSION 0.1.0 DESCRIPTION "Presentacion")

add_executable(prensentacion 
  main.cpp
  BHTree.cpp
  Particle.cpp
  Space.cpp
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenMP REQUIRED)

target_compile_options(prensentacion PRIVATE -Wall -Wextra -Wno-unknown-pragmas)

target_include_directories(prensentacion PRIVATE src include)

target_link_libraries(prensentacion PRIVATE OpenGL::GL OpenGL::GLX GLUT::GLUT OpenMP::OpenMP_CXX)
