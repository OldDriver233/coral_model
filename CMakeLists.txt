cmake_minimum_required(VERSION 3.30)
project(coral)

set(CMAKE_CXX_STANDARD 20)

add_executable(coral main.cpp
        coral_base/coral.cpp
        utils/union_set.h)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(coral PRIVATE Eigen3::Eigen)

find_package(libigl CONFIG REQUIRED)
target_link_libraries(coral PRIVATE igl::igl_core igl::igl_glfw igl::igl_imgui igl::igl_opengl)