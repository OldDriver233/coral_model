#ifndef CORAL_H
#define CORAL_H
#include <Eigen/Dense>
#include <unordered_set>
#include "../utils/pair_hash.h"
#include "../utils/random.h"

using Eigen::MatrixXd;
using Eigen::MatrixXi;

struct Coral{
public:
    MatrixXd vertices;
    MatrixXi indices;
    double alpha = .1;
    double s_min = 0, s_max = 1;
    double d_min = .1, d_max = .6;
    double l_branch = .4;
    double theta = M_PI / 3;
    Random random;

    Coral() = default;
    Coral(const Coral&) = default;
    ~Coral() = default;

    void init();
    void grow(double);
private:
    int m_id;
    std::unordered_set<std::pair<int, int>, PairHash> edges;
    std::unordered_set<int> branch_front;
};

#endif //CORAL_H
