cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(raytracer)
add_executable(raytracer main.cpp sphere.cpp interval.cpp camera.cpp vec3.cpp)
target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
