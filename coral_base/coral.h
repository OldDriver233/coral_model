#ifndef CORAL_H
#define CORAL_H
#include <Eigen/Dense>
#include <unordered_set>
#include "../utils/pair_hash.h"

using Eigen::MatrixXd;
using Eigen::MatrixXi;

struct Coral{
public:
    MatrixXd vertices;
    MatrixXi indices;
    double alpha = .02;
    double s_min = .583, s_max = 1;
    double d_min = .05, d_max = .6;

    Coral() = default;
    Coral(const Coral&) = default;
    ~Coral() = default;

    void init();
    void grow(double);
private:
    int m_id;
    std::unordered_set<std::pair<int, int>, PairHash> edges;
};

#endif //CORAL_H
