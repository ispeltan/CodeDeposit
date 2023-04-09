//
// Created by tanpe on 2023/4/6.
//

#pragma once

#include "Edge.hpp"
#include <vector>
#include <cassert>

template<typename Weight>
class WeightedDenseGraph {
public:
    WeightedDenseGraph(int n, bool directed = false) {
        for (int i = 0; i < n; ++i) {
            auto&& vec = std::vector<Edge<Weight>*>(n, nullptr);
            g_.resize(n, vec);
        }
        directed_ = directed;
        edge_cnt_ = 0;
    }

    ~WeightedDenseGraph() {
        for (auto &it: g_) {
            for (auto &i: it) {
                if (i != nullptr) {
                    delete i;
                    i = nullptr;
                }
            }
        }
    }

    void AddEdge(int n, int m, Weight w) {
        assert(n >= 0 && n < g_.size());
        assert(m >= 0 && m < g_.size());
        if (n == m) {
            return;
        }
        if (HasEdge(n, m)) {
            return;
        }
        g_[n][m] = new Edge(n, m, w);
        if (!directed_) {
            g_[m][n] = new Edge(m, n, w);
        }
        edge_cnt_++;
    }


    bool HasEdge(int n, int m) {
        return g_[n][m] != nullptr;
    }

    int GetEdgeCnt() const {
        return edge_cnt_;
    }

    int GetNodeCnt() const {
        return g_.size();
    }

    void Show() const {
        int i = 0;
        for (auto &it: g_) {
            std::cout << "[" << i++ << "] ";
            for (auto &i: it) {
                if (i == nullptr) {
                    std::cout << "null \t";
                } else {
                    std::cout << (*i) << "\t";
                }
            }
            std::cout << std::endl;
        }
    }

    class Iterator {
    public:
        Iterator(WeightedDenseGraph &graph, int n) : graph_(graph), n_(n) {
            assert(n >= 0 && n < graph_.g_.size());
            index_ = -1;
        }

        Edge<Weight> *begin() {
            index_ = -1;
            return next();
        }

        bool end() {
            return index_ >= graph_.GetNodeCnt();
        }

        Edge<Weight> *next() {
            index_++;
            while (index_ < graph_.GetNodeCnt() && graph_.g_[n_][index_] == nullptr) {
                index_++;
            }
            if (index_ < graph_.GetNodeCnt()) {
                return graph_.g_[n_][index_];
            }
            return nullptr;
        }

    private:
        WeightedDenseGraph &graph_;
        int n_;
        int index_;
    };

private:
    bool directed_;
    std::vector<std::vector<Edge<Weight>*>> g_;
    int edge_cnt_;
};