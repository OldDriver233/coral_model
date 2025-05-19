#ifndef RANDOM_H
#define RANDOM_H
#include <random>

class Random {
public:
    std::mt19937 engine;

    Random() {
        std::random_device r;
        engine.seed(r());
    }
    ~Random() = default;
    Random(const Random&) = default;

    explicit Random(unsigned int seed) {
        engine.seed(seed);
    }

    double rand_double(double _min, double _max) {
        std::uniform_real_distribution dist(_min, _max);
        return dist(this->engine);
    }

    int rand_int(int _min, int _max) {
        std::uniform_int_distribution dist(_min, _max);
        return dist(this->engine);
    }
};

#endif //RANDOM_H
