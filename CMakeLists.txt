cmake_minimum_required(VERSION 3.30)
project(coral)

set(CMAKE_CXX_STANDARD 20)

add_executable(coral main.cpp
        utils/union_set.h
        model/coral.cpp
        model/environment.cpp
        viewer/viewer.cpp
)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(coral PRIVATE Eigen3::Eigen)

find_package(libigl CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

include_directories(glad/include)
add_library(glad STATIC glad/src/glad.c)

target_link_libraries(coral PRIVATE igl::igl_core glfw glad)