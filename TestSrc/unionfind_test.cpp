//
// Created by tanpe on 2023/4/2.
//

#include "UnionFInd/UnionFind.hpp"
#include <iostream>
#include <chrono>
#include <memory>
using namespace std;

void Test(std::shared_ptr<UnionFind> uf, int n) {
    srand(time(nullptr));
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
        int q = rand() % n;
        int p = rand() % n;
        uf->Union(p, q);
    }
    for (int i = 0; i < n; ++i) {
        int q = rand() % n;
        int p = rand() % n;
        uf->IsConnected(p, q);
    }
    auto end = chrono::steady_clock::now();

    auto t = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "n = " << n  << " t = " << t.count() / 1000.0 << "s" << endl;
}
int main() {
    int n = 100000000;
    auto v1 = std::make_shared<UnionFindV1>(n);
    auto v2 = std::make_shared<UnionFindV2>(n);
    auto v3 = std::make_shared<UnionFindV3>(n);
    auto v4 = std::make_shared<UnionFindV4>(n);
    auto v5 = std::make_shared<UnionFindV5>(n);



//    Test(v1, n);
//    Test(v2, n);
    Test(v3, n);
    Test(v4, n);
    Test(v5, n);

    return 0;
}