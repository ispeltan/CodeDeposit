//
// Created by tanpe on 2023/4/2.
//
#pragma once

#include <Graph/Graph.hpp>
#include <vector>


// 稠密图一般都用邻接矩阵表示
class DenseGraph : public Graph {
public:
    DenseGraph(int n, bool direct) : Graph(n, direct) {
        for (int i = 0; i < n; ++i) {
            std::vector<bool> item;
            item.assign(n, false);
            g_.emplace_back(item);
        }
        this->direct_ = direct;
    }

    // 获取节点个数
    virtual int GetNodeCount() const {
        return g_.size();
    }


    // 节点p和节点q新增连接
    virtual void AddEdge(int p, int q) {
        assert(p >= 0 && p < g_.size());
        assert(q >= 0 && q < g_.size());
        if (HasLink(p, q)) {
            return;
        }
        if (p == q) {
            return;
        }
        g_[p][q] = true;
        if (!direct_) {
            g_[q][p] = true;
        }
        edge_num_++;
    }

    // 节点p和节点q是否相连
    virtual bool HasLink(int p, int q) {
        assert(p >= 0 && p < g_.size());
        assert(q >= 0 && q < g_.size());
        return g_[p][q];
    }

    virtual std::string Show() const {
        std::stringstream ss{};
        int i = 0;
        for (const auto &it: g_) {
            ss << "node[" << i++ << "] ";
            for (const auto &has: it) {
                ss << has << " ";
            }
            ss << std::endl;
        }
        return ss.str();
    }

    virtual std::string DepthFirstTraverse() {
        std::stringstream ss{};
        for (int i = 0; i < g_.size(); ++i) {
            ss << DoDFS(i);
        }
        ss << std::endl;
        return ss.str();
    }

    // 迭代器
    class Iterator {
    public:
        // 构造函数传入：图和节点
        Iterator(DenseGraph &graph, int v) : graph_(graph) {
            assert(v >= 0 && v < graph.g_.size());
            v_ = v;
            index_ = -1;
        }

        int begin() {
            index_ = -1;
            return next();
        }

        int next() {
            index_++;
            while (index_ < graph_.g_[v_].size() && !graph_.g_[v_][index_]) {
                index_++;
            };
            if (index_ < graph_.g_[v_].size()) {
                return index_;
            }
            return -1;
        }

        bool end() {
            return index_ >= graph_.g_[v_].size() || index_ < 0;
        }

    private:
        DenseGraph &graph_;
        int v_;
        int index_;
    };

private:
    std::string DoDFS(int index) {
        std::stringstream ss{};
        for (int i = 0; i < g_[index].size(); ++i) {
            if (g_[index][i] && !visited_[i]) {
                ss << i << " ";
                visited_[i] = true;
                ss << DoDFS(i);
            }
        }
        return ss.str();
    }

private:
    std::vector<std::vector<bool>> g_;
};


