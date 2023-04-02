//
// Created by tanpe on 2023/4/2.
//

#pragma once

#include <vector>
#include <Graph/Graph.hpp>

// 稀松图-一般使用邻接表表示
class SparseGraph : public Graph {
public:
    SparseGraph(int n, bool is_directed): Graph(is_directed) {
        g_.assign(n, {});
    }

    int GetNodeCount() const {
        return g_.size();
    }

    // 节点p和节点q新增连接
    void AddEdge(int p, int q) {
        assert(p >= 0 && p < g_.size());
        assert(q >= 0 && q < g_.size());

        // 检查是否已经相连 -- 去除平行边
        if (HasLink(p, q)) {
            return;
        }

        // 检查是否是自己 -- 去除自环边
        if (p == q) {
            return;
        }

        g_[p].push_back(q);

        // 如果不是有向图，还需要反向连接下
        if (!direct_ && !HasLink(q, p)) {
            g_[q].push_back(p);
        }
        edge_num_++;
    }

    bool HasLink(int p, int q) {
        assert(p >= 0 && p < g_.size());
        assert(q >= 0 && q < g_.size());
        for (const auto &it: g_[p]) {
            if (it == q) {
                return true;
            }
        }
        return false;
    }

    // 迭代器
    class Iterator {
        // 构造函数传入：图和节点
        Iterator(SparseGraph& graph, int v): graph_(graph) {
            assert(v >= 0 && v < graph.g_.size());
            v_ = v;
            index_ = 0;
        }

        int begin() {
            if (graph_.g_[v_].empty()) {
                return -1;
            }
            return graph_.g_[v_][0];

        }
        int next() {
            index_++;
            if (index_ < graph_.g_[v_].size()) {
                return graph_.g_[v_][index_];
            }
            return -1;
        }

        bool end() {
            return index_ >= graph_.g_.size();
        }

    private:
        SparseGraph& graph_;
        int v_;
        int index_;
    };


private:
    std::vector<std::vector<int>> g_;
};