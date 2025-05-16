#include <Eigen/Dense>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/per_vertex_normals.h>
#include "coral_base/coral.h"

using Eigen::MatrixXd;
using Eigen::MatrixXi;

int main() {
    Coral c;
    c.init();
    for (int i = 0; i < 150; i++) {
        c.grow(1);
    }
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(c.vertices, c.indices);
    viewer.launch();
    return 0;
}