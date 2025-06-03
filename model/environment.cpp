#include "environment.h"

#include <cmath>

#include "../utils/random.h"

OceanFloor::OceanFloor() {
    generate();
}

void OceanFloor::generate() {
    vertices.resize(4, 3);
    vertices <<
            -size, -size, depth,
            size, -size, depth,
            size, size, depth,
            -size, size, depth;

    faces.resize(2, 3);
    faces <<
            0, 1, 2,
            0, 2, 3;
}

WaterSurface::WaterSurface() {
    generate();
}

void WaterSurface::generate(double size) {
    int divisions = 20;
    vertices.resize((divisions + 1) * (divisions + 1), 3);
    faces.resize(2 * divisions * divisions, 3);

    double step = size / divisions;
    int idx = 0;
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            double x = -size / 2 + j * step;
            double y = -size / 2 + i * step;
            vertices.row(idx++) << x, y, level;
        }
    }

    idx = 0;
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            int v0 = i * (divisions + 1) + j;
            int v1 = v0 + 1;
            int v2 = v0 + (divisions + 1);
            int v3 = v2 + 1;

            faces.row(idx++) << v0, v1, v2;
            faces.row(idx++) << v2, v1, v3;
        }
    }
}

void WaterSurface::update(double time) {
    for (int i = 0; i < vertices.rows(); ++i) {
        double x = vertices(i, 0);
        double y = vertices(i, 1);
        vertices(i, 2) = level + waveHeight * sin(waveFrequency * (x + time)) *
                         cos(waveFrequency * (y + time));
    }
}

MarineEnvironment::MarineEnvironment() = default;

void MarineEnvironment::update(double time, double dt) {
    waterSurface.update(time);
}
