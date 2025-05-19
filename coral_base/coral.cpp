#include <igl/per_vertex_normals.h>
#include <igl/edge_lengths.h>
#include <cmath>
#include <numbers>
#include <vector>
#include "../utils/union_set.h"
#include "../utils/misc.h"
#include "../utils/random.h"
#include "coral.h"

void Coral::init() {
    const double size = 0.5;
    const double sqrt_3_2 = std::sqrt(3) / 2;
    vertices = MatrixXd(7, 3);
    indices = MatrixXi(6, 3);
    vertices<<
        0, 0, .4,
        size, 0, 0,
        size * 0.5, size * sqrt_3_2, 0,
        size * -0.5, size * sqrt_3_2, 0,
        -size, 0, 0,
        size * -0.5, -size * sqrt_3_2, 0,
        size * 0.5, -size * sqrt_3_2, 0;
    indices<<
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1;
    m_id = 7;
}


void Coral::grow(double delta_time) {
    MatrixXd norms;
    igl::per_vertex_normals(vertices, indices, norms);
    for (int i = 0; i < norms.rows(); i++) {
        MatrixXd norm = norms.row(i);
        double n_x = norm(0);
        double n_y = norm(1);
        double n_z = norm(2);
        if (abs(vertices(i, 2)) < .002) {
            n_z = 0;
            double r = hypot(n_x, n_y);
            n_x /= r;
            n_y /= r;
            norm(0) = n_x;
            norm(1) = n_y;
            norm(2) = 0;
        }
        if (norm.hasNaN()) continue;
        auto zeta = 2 * std::numbers::inv_pi * std::atan(n_z / std::hypot(n_x, n_y));
        //zeta = clamp(zeta + random.rand_double(.0, .03), -1.0, 1.0);
        if (zeta >= s_min && zeta <= s_max) {
            vertices.row(i) += norm * delta_time * alpha * random.rand_double(0.95, 1.05);
        }
    }

    MatrixXd lengths;
    igl::edge_lengths(vertices, indices, lengths);
    std::unordered_set<std::pair<int, int>, PairHash> edges_to_break;
    std::unordered_set<std::pair<int, int>, PairHash> edges_to_merge;
    int face_change = 0;

    for (int i = 0; i < lengths.rows(); i++) {
        for (int j = 0; j < 3; j++) {
            auto i1 = indices(i, (j + 1) % 3);
            auto i2 = indices(i, (j + 2) % 3);
            auto p = std::make_pair(std::min(i1, i2), std::max(i1, i2));
            if (lengths(i, j) < d_min) {
                edges_to_merge.emplace(p);
                face_change--;
            } else if (lengths(i, j) > d_max) {
                edges_to_break.emplace(p);
                face_change++;
            }
        }
    }

    UnionSet final_vertices(vertices.rows() + edges_to_break.size());
    for (auto [i1, i2]: edges_to_merge) {
        final_vertices.merge(i1, i2);
    }
    for (auto [i1, i2]: edges_to_break) {
        if (i1 != final_vertices.find(i1) || i2 != final_vertices.find(i2)) {
            face_change--;
        }
    }

    MatrixXd new_vertices(vertices.rows() + edges_to_break.size(), 3);
    MatrixXi new_indices(indices.rows() + face_change, 3);
    int new_idx_i = 0;
    for (int i = 0; i < vertices.rows(); i++) {
        new_vertices.row(i) = vertices.row(i);
    }

    std::unordered_map<std::pair<int, int>, int, PairHash> vertices_created;
    for (int i = 0; i < indices.rows(); i++) {
        bool deleted = false;
        int cnt = 0;
        int appended_idx[6];
        memset(appended_idx, 0xff, sizeof(int) * 6);

        for (int j = 0; j < 3; j++) {
            appended_idx[j] = final_vertices.find(indices(i, j));
            auto i1 = indices(i, (j + 1) % 3);
            auto i2 = indices(i, (j + 2) % 3);
            auto j1 = final_vertices.find(i1);
            auto j2 = final_vertices.find(i2);
            if (j1 == j2) {
                deleted = true;
                break;
            }

            auto p = std::make_pair(std::min(j1, j2), std::max(j1, j2));
            if (edges_to_break.contains(p)) {
                auto result = vertices_created.try_emplace(p, m_id);
                if (result.second) {
                    MatrixXd coord = (vertices.row(j1) + vertices.row(j2)) / 2;
                    new_vertices.row(m_id) = coord;
                    m_id++;
                }
                int idx = result.first->second;
                appended_idx[j + 3] = idx;
                cnt++;
            }
        }
        if (!deleted) {
            if (cnt == 0) {
                for (int j = 0; j < 3; j++) {
                    new_indices(new_idx_i, j) = appended_idx[j];
                }
                new_idx_i++;
            } else if (cnt == 1) {
                MatrixXi row1(1, 3), row2(1, 3);
                for (int j = 0; j < 3; j++) {
                    if (appended_idx[j + 3] != -1) {
                        row1<<appended_idx[j], appended_idx[(j + 1) % 3], appended_idx[j + 3];
                        row2<<appended_idx[j], appended_idx[j + 3], appended_idx[(j + 2) % 3];
                        break;
                    }
                }
                new_indices.row(new_idx_i++) = row1;
                new_indices.row(new_idx_i++) = row2;
            } else if (cnt == 2) {
                MatrixXi row1(1, 3), row2(1, 3), row3(1, 3);
                for (int j = 0; j < 3; j++) {
                    if (appended_idx[j + 3] == -1) {
                        row1<<appended_idx[j], appended_idx[3 + (j + 2) % 3], appended_idx[3 + (j + 1) % 3];
                        row2<<appended_idx[3 + (j + 2) % 3], appended_idx[(j + 1) % 3], appended_idx[3 + (j + 1) % 3];
                        row3<<appended_idx[(j + 1) % 3], appended_idx[(j + 2) % 3], appended_idx[3 + (j + 1) % 3];
                        break;
                    }
                }
                new_indices.row(new_idx_i++) = row1;
                new_indices.row(new_idx_i++) = row2;
                new_indices.row(new_idx_i++) = row3;
            } else {
                MatrixXi row1(1, 3), row2(1, 3), row3(1, 3), row4(1, 3);
                row1<<appended_idx[0], appended_idx[5], appended_idx[4];
                row2<<appended_idx[1], appended_idx[3], appended_idx[5];
                row3<<appended_idx[2], appended_idx[4], appended_idx[3];
                row4<<appended_idx[3], appended_idx[4], appended_idx[5];
                new_indices.row(new_idx_i++) = row1;
                new_indices.row(new_idx_i++) = row2;
                new_indices.row(new_idx_i++) = row3;
                new_indices.row(new_idx_i++) = row4;
            }
        }
    }

    indices = new_indices;
    vertices = new_vertices;
}
