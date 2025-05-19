#include <Eigen/Dense>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/per_vertex_normals.h>
#include "coral_base/coral.h"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::MatrixXi;

int main() {
    Coral c;
    c.init();
    int st;
    std::cin>>st;
    for (int i = 0; i < st; i++) {
        c.grow(.25);
    }
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(c.vertices, c.indices);
    viewer.launch();
    return 0;
}