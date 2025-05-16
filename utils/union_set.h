//
// Created by 孙诗超 on 25-5-15.
//
#include <vector>

#ifndef UNION_SET_H
#define UNION_SET_H
class UnionSet {
public:
    std::vector<int> parent;
    std::vector<int> rank;

    explicit UnionSet(int num) {
        for (int i = 0; i < num; i++) {
            parent.push_back(i);
            rank.push_back(0);
        }
    }

    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    void merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (rank[x] > rank[y]) parent[y] = x;
        else {
            if (rank[x] == rank[y]) rank[y]++;
            parent[x] = y;
        }
    }
};
#endif //UNION_SET_H
