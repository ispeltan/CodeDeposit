//
// Created by tanpe on 2023/4/2.
//
#pragma once

#include <vector>
#include <cassert>

class UnionFind {
public:
    virtual int Find(int p) = 0;

    virtual bool IsConnected(int p, int q) = 0;

    virtual void Union(int p, int q) = 0;
};

class UnionFindV1: public UnionFind {
public:
    UnionFindV1(int n) {
        ids_.resize(n);
        for (int i = 0; i < n; ++i) {
            ids_[i] = i;
        }
    }

    int Find(int p) {
        assert(p >= 0 && p < ids_.size());
        return ids_[p];
    }

    void Union(int p, int q) {
        assert(p >= 0 && p < ids_.size());
        assert(q >= 0 && q < ids_.size());
        if (Find(p) == Find(q)) {
            return;
        }
        for (auto &it: ids_) {
            if (it == Find(q)) {
                it = Find(p);
            }
        }
    }

    bool IsConnected(int p, int q) {
        return Find(p) == Find(q);
    }

private:
    std::vector<int> ids_;
};


class UnionFindV2: public UnionFind {
public:
    UnionFindV2(int n) {
        parent_.resize(n);
        for (int i = 0; i < parent_.size(); ++i) {
            parent_[i] = i;
        }
    }

    int Find(int p) {
        assert(p >= 0 && p < parent_.size());
        while (p != parent_[p]) {
            p = parent_[p];
        }
        return p;
    }

    bool IsConnected(int p, int q) {
        return Find(p) == Find(q);
    }

    void Union(int p, int q) {
        auto p_root = Find(p);
        auto q_root = Find(q);
        if (p_root == q_root) {
            return;
        }
        parent_[q_root] = p_root;
    }


private:
    std::vector<int> parent_;
};

class UnionFindV3: public UnionFind {
public:
    UnionFindV3(int n) {
        parent_.resize(n);
        sz_.assign(n, 1);
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }
    int Find(int p) {
        assert(p >= 0 && p < parent_.size());
        while (p != parent_[p]) {
            p = parent_[p];
        }
        return p;
    }

    bool IsConnected(int p, int q) {
        return Find(p) == Find(q);
    }

    void Union(int p, int q) {
        auto p_root = Find(p);
        auto q_root = Find(q);
        if (p_root == q_root) {
            return;
        }
        if (sz_[p_root] > sz_[q_root]) {
            parent_[q_root] = parent_[p_root];
        } else {
            parent_[p_root] = parent_[q_root];
            sz_[q_root] += sz_[p_root];
        }
    }

private:
    std::vector<int> parent_;
    // 表示以i为根的元素个数
    std::vector<int> sz_;
};

class UnionFindV4: public UnionFind {
public:
    UnionFindV4(int n) {
        parent_.resize(n);
        rank_.assign(n, 1);
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }
    int Find(int p) {
        assert(p >= 0 && p < parent_.size());
        while (p != parent_[p]) {
            p = parent_[p];
        }
        return p;
    }

    bool IsConnected(int p, int q) {
        return Find(p) == Find(q);
    }

    void Union(int p, int q) {
        auto p_root = Find(p);
        auto q_root = Find(q);
        if (p_root == q_root) {
            return;
        }
       if (rank_[p_root] > rank_[p_root]) {
           parent_[q] = p_root;
       } else if (rank_[q_root] > rank_[p_root]) {
           parent_[p] = q_root;
       } else {
           parent_[p] = q_root;
           rank_[q_root]++;
       }
    }

private:
    std::vector<int> parent_;
    // 表示以i为根的层数
    std::vector<int> rank_;
};

class UnionFindV5: public UnionFind {
public:
    UnionFindV5(int n) {
        parent_.resize(n);
        rank_.assign(n, 1);
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }
    // 路径压缩
    int Find(int p, int) {
        assert(p >= 0 && p < parent_.size());
        while (p != parent_[p]) {
            parent_[p] = parent_[parent_[p]];
            p = parent_[p];
        }
        return p;
    }
    // 方式2
    int Find(int p) {
        assert(p >= 0 && p < parent_.size());
        if (p != parent_[p]) {
            parent_[p] = Find(parent_[p]);
        }
        return parent_[p];
    }

    bool IsConnected(int p, int q) {
        return Find(p) == Find(q);
    }

    void Union(int p, int q) {
        auto p_root = Find(p);
        auto q_root = Find(q);
        if (p_root == q_root) {
            return;
        }
        if (rank_[p_root] > rank_[p_root]) {
            parent_[q] = p_root;
        } else if (rank_[q_root] > rank_[p_root]) {
            parent_[p] = q_root;
        } else {
            parent_[p] = q_root;
            rank_[q_root]++;
        }
    }

private:
    std::vector<int> parent_;
    // 表示以i为根的层数
    std::vector<int> rank_;
};