//
// Created by tanpe on 2023/4/6.
//
#pragma once
#include <vector>
#include "Edge.hpp"
#include <assert.h>

// 有权图-稀松图（邻接表）
template<typename Weight>
class WeightedSparseGraph {
public:
    WeightedSparseGraph(int n, bool directed = false) {
        g_.resize(n, std::vector<Edge<Weight>*>());
        directed_ = directed;
        edge_cnt_ = 0;
    }
    ~WeightedSparseGraph() {
        for (auto& it: g_) {
            for (auto& i: it) {
                delete i;
            }
        }

    }

    // 添加n->m
    void AddEdge(int n, int m, Weight w) {
        assert(n >= 0 && n < g_.size());
        assert(m >= 0 && m < g_.size());
        if (n == m) {
            return;
        }
        if (HasEdge(n, m)) {
            return;
        }
        g_[n].push_back(new Edge(n, m, w));
        if (!directed_) {
            g_[m].push_back(new Edge(m, n, w));
        }
        edge_cnt_++;
    }
    void Show() {
        for (int i = 0; i < g_.size(); ++i) {
            std::cout << "[" << i << "] ";
            for (auto& it: g_[i]) {
                std::cout << *it << "\t";
            }
            std::cout << std::endl;
        }
    }
    int GetNodeCnt() const {
        return g_.size();
    }
    int GetEdgeCnt() const {
        return edge_cnt_;
    }

    class Iterator {
    public:
        Iterator(WeightedSparseGraph& graph, int n): graph_(graph), n_(n) {
            assert(n >= 0 && n < graph.GetNodeCnt());
            index_ = 0;
        }
        // 返回边信息的指针
        Edge<Weight>* begin() {
            index_ = 0;
            if (graph_.g_[n_].empty()) {
                return nullptr;
            }
            return graph_.g_[n_][0];
        }
        bool end() {
            return index_ >= graph_.g_[n_].size();
        }

        Edge<Weight>* next() {
            index_++;
            if (index_ < graph_.g_[n_].size()) {
                return graph_.g_[n_][index_];
            }
            return nullptr;
        }

    private:
        WeightedSparseGraph& graph_;
        int n_;
        int index_;
    };

private:
    bool directed_;

    bool HasEdge(int n, int m) {
        for (const auto& it: g_[n]) {
            if (it->Other(n) == m) {
                return true;
            }
        }
        return false;
    }

    // 一共有多少边
    int edge_cnt_;
    std::vector<std::vector<Edge<Weight>*>> g_;
};

