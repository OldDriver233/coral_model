#ifndef PAIR_HASH_H
#define PAIR_HASH_H

struct PairHash {
    template<class T, class U>
    std::size_t operator()(const std::pair<T, U> &p) const {
        auto h1 = std::hash<T>{}(p.first);
        auto h2 = std::hash<T>{}(p.second);
        return h1 ^ h2;
    }
};
#endif //PAIR_HASH_H
