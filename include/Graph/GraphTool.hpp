//
// Created by tanpe on 2023/4/2.
//

#pragma once
#include <Graph/DenseGraph.hpp>
#include <Graph/SparseGraph.hpp>
#include <fstream>
#include <stack>
#include <queue>


template<typename G>
class GraphTool {
public:
    GraphTool(G& g): g_(g) {
        visited_.assign(g_.GetNodeCount(), false);
        ids_.assign(g_.GetNodeCount(), -1);
        from_.assign(g_.GetNodeCount(), -1);
        order_.assign(g_.GetNodeCount(), -1);
    }
    // 从文件中生成图
    void FormGraphByFile(const std::string filename) {
        std::fstream file(filename);
        assert(file.is_open());
        // 读取第一行 获得node总数
        std::string line{};
        assert(std::getline(file, line));
        std::stringstream ss(line);
        int n = 0;
        ss >> n;
        assert(n == g_.GetNodeCount());

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            int p = 0, q = 0;
            ss >> p >> q;
            g_.AddEdge(p, q);
        }
    }

    // 深度优先遍历
    std::string Dfs() {
        visited_.assign(g_.GetNodeCount(), false);
        ids_.assign(g_.GetNodeCount(), -1);
        std::stringstream ss{};
        for (int i = 0; i < g_.GetNodeCount(); ++i) {
            if (!visited_[i]) {
                connected_num_++;
                ss << DoDfs(i);
            }
        }
        ss << std::endl;
        return ss.str();
    }

    // 路径 从p节点到q节点的路径
    std::string PathToQFromP(int p, int q) {
        visited_.assign(g_.GetNodeCount(), false);
        from_.assign(g_.GetNodeCount(), -1);
        std::stringstream ss{};
        DoDfs(p);

        // 没有从p到到q的路径
        if (from_[q] < 0) {
            return {};
        }

        std::stack<int> s;
        s.push(q);
        while (from_[q] != p) {
            s.push(from_[q]);
            q = from_[q];
        }
        s.push(p);
        while (!s.empty()) {
            ss << s.top();
            if (s.size() > 1) {
                ss << "->";
            }
            s.pop();
        }
        ss << std::endl;
        return ss.str();
    }

    // 广度优先遍历
    std::string Bfs() {
        visited_.assign(g_.GetNodeCount(), false);
        from_.assign(g_.GetNodeCount(), -1);
        std::stringstream ss{};
        for (int i = 0; i < g_.GetNodeCount(); ++i) {
            if (!visited_[i]) {
                ss << DoBfs(i);
            }
        }
        return ss.str();
    }

    // 寻找最短路径
    std::string ShortestPathToQFromP(int p, int q) {
        visited_.assign(g_.GetNodeCount(), false);
        from_.assign(g_.GetNodeCount(), -1);
        order_.assign(g_.GetNodeCount(), -1);
        DoBfs(p);

        std::stringstream ss{};
        std::stack<int> s;
        s.push(q);
        while (from_[q] != p) {
            s.push(from_[q]);
            q = from_[q];
        }
        s.push(p);
        while (!s.empty()) {
            ss << s.top();
            if (s.size() > 1) {
                ss << "->";
            }
            s.pop();
        }
        ss << std::endl;
        return ss.str();
    }

    // 最短路径长度
    int ShortestPathLength(int p, int q) {
        visited_.assign(g_.GetNodeCount(), false);
        from_.assign(g_.GetNodeCount(), -1);
        order_.assign(g_.GetNodeCount(), -1);
        DoBfs(p);
        return order_[q];
    }


    // 求连通分量的个数
    int GetConnectedNum() {
        return connected_num_;
    }

    std::string ShowIds() {
        std::stringstream ss{};
        for (auto& it: ids_) {
            ss << it << " ";
        }
        ss << std::endl;
        return ss.str();
    }

private:
    std::string DoDfs(int v) {
        typename G::Iterator it(g_, v);
        std::stringstream ss{};
        ids_[v] = connected_num_;
        for (auto i = it.begin(); !it.end(); i = it.next()) {
            if (visited_[i]) {
                continue;
            }
            visited_[i] = true;
            from_[i] = v;
            ss << i << " ";
            ss << DoDfs(i);
        }
        return ss.str();
    }

    // 对某个节点的关联元素进行广度优先遍历
    std::string DoBfs(int v) {
        std::queue<int>q;
        std::stringstream ss{};
        q.push(v);
        visited_[v] = true;
        order_[v] = 0;
        while (!q.empty()) {
            auto item = q.front();
            q.pop();
            ss << item << " ";
            typename G::Iterator it(g_, item);
            for (auto i = it.begin(); !it.end(); i = it.next()) {
                if (!visited_[i]) {
                    q.push(i);
                    visited_[i] = true;
                    from_[i] = item;
                    order_[i] = order_[item] + 1;
                }
            }
        }
        return ss.str();
    }

private:
    G& g_;
    int connected_num_ = 0;
    std::vector<bool> visited_;
    std::vector<int> from_;
    std::vector<int> ids_;
    std::vector<int> order_;
};