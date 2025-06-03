#ifndef CORAL_H
#define CORAL_H
#include <Eigen/Dense>
#include <unordered_set>
#include "../utils/pair_hash.h"
#include "../utils/random.h"

using Eigen::MatrixXd;
using Eigen::MatrixXi;

struct Coral {
    MatrixXd vertices;
    MatrixXi indices;
    double alpha = .1;
    double s_min = 0.01, s_max = 1;
    double d_min = .1, d_max = .6;
    double l_branch = .5;
    double theta = std::numbers::pi / 3;
    Random random;

    Coral();

    Coral(const Coral &) = default;

    ~Coral() = default;

    void grow(double);

private:
    int m_id;

    std::unordered_set<std::pair<int, int>, PairHash> edges;
    std::unordered_map<int, MatrixXd> branch_front;
};

#endif //CORAL_H
