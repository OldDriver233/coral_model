#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Eigen/Dense>
#include <vector>

using Eigen::MatrixXd;
using Eigen::MatrixXi;
using Eigen::RowVector3d;

struct OceanFloor {
    MatrixXd vertices;
    MatrixXi faces;
    RowVector3d color = RowVector3d(0.1, 0.3, 0.2);
    double depth = 0.0;
    double size = 20.0;

    OceanFloor();

    void generate();
};

struct WaterSurface {
    MatrixXd vertices;
    MatrixXi faces;
    double level = 10;
    double waveHeight = 0.2;
    double waveFrequency = 0.5;

    WaterSurface();

    void generate(double size = 20.0);

    void update(double time);
};

struct MarineEnvironment {
    OceanFloor oceanFloor;
    WaterSurface waterSurface;

    MarineEnvironment();

    void update(double time, double dt);
};

#endif //ENVIRONMENT_H
