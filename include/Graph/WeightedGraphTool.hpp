//
// Created by tanpe on 2023/4/6.
//

#pragma once

#include "WeightedSparseGraph.hpp"
#include "WeightedDenseGraph.hpp"
#include <fstream>
#include <sstream>
#include <queue>
#include <Heap/IndexMinHeap.hpp>
#include <UnionFInd/UnionFind.hpp>

template<typename Graph, typename Weight>
class WeightedGraphTool {
public:
    WeightedGraphTool(Graph &graph) : graph_(graph) {
        marked_.assign(graph_.GetNodeCnt(), false);
        edge_to_.assign(graph_.GetNodeCnt(), nullptr);
        index_heap_ = IndexMinHeap<Edge<Weight>>(graph_.GetNodeCnt());
    }

    // 从文件中生成图
    template<class T>
    void FormGraphByFile(const std::string filename) {
        std::fstream file(filename);
        assert(file.is_open());
        // 读取第一行 获得node总数
        std::string line{};
        assert(std::getline(file, line));
        std::stringstream ss(line);
        int n = 0;
        ss >> n;
        assert(n == graph_.GetNodeCnt());

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            int p = 0, q = 0;
            T w;
            ss >> p >> q >> w;
            graph_.AddEdge(p, q, w);
        }
    }

    // 获取最小生成树(lazy Prim)
    std::vector<Edge<Weight>> MinSpanTree() {
        std::vector<Edge<Weight>> ret{};
        Visit(0);
        while (!pq_.empty()) {
            auto it = pq_.top();
            pq_.pop();
            if (marked_[it.n_] == marked_[it.m_]) {
                continue;
            }
            ret.push_back(it);
            if (!marked_[it.m_]) {
                Visit(it.m_);
            } else {
                Visit(it.n_);
            }

        }
        return ret;
    }

    // Prim算法的优化——索引堆
    std::vector<Edge<Weight>> MinSpanTree(int) {
        marked_.assign(graph_.GetNodeCnt(), false);
        edge_to_.assign(graph_.GetNodeCnt(), nullptr);
        std::vector<Edge<Weight>> ret{};
        Visit(0, 0);
        while (!index_heap_.Empty()) {
            auto v = index_heap_.ExtractMinIndex();
            ret.push_back(*edge_to_[v]);
            Visit(v, 0);
        }
        return ret;
    }

    // Kruskal算法
    std::vector<Edge<Weight>> MinSpanTree(int, int) {
        std::priority_queue<Edge<Weight>, std::vector<Edge<Weight>>, std::greater<Edge<Weight>>> pq;
        for (int v = 0; v < graph_.GetNodeCnt(); ++v) {
            typename Graph::Iterator it(graph_, v);
            for (auto i = it.begin(); !it.end(); i = it.next()) {
                if (i->m_ < i->n_)
                    pq.push(*i);
            }
        }
        UnionFindV1 uf(graph_.GetNodeCnt());
        std::vector<Edge<Weight>> ret;

        while (!pq.empty() && ret.size() < graph_.GetNodeCnt() - 1) {
            auto it = pq.top();
            pq.pop();
            if (uf.IsConnected(it.n_, it.m_)) {
                continue;
            }
            ret.push_back(it);
            uf.Union(it.m_, it.n_);
        }
        return ret;
    }

    // 单源最短路径算法——Dijkstra
    std::vector<Edge<Weight>> ShortestPathFromVToW(int s, int w) {
        marked_.assign(graph_.GetNodeCnt(), false);
        from_.assign(graph_.GetNodeCnt(), nullptr);
        distTo_.assign(graph_.GetNodeCnt(), Weight());
        index_weight_heap_ = IndexMinHeap<Weight>(graph_.GetNodeCnt());

        distTo_[s] = Weight();
        marked_[s] = true;
        index_weight_heap_.Insert(s, distTo_[s]);

        while (!index_weight_heap_.Empty()) {
            auto v = index_weight_heap_.ExtractMinIndex();
            marked_[v] = true;
            typename Graph::Iterator it(graph_, v);
            for (auto i = it.begin(); !it.end(); i = it.next()) {
                int w = i->Other(v);
                if (!marked_[w]) {
                    if (from_[w] == nullptr || distTo_[v] + i->weight_ < distTo_[w]) {
                        distTo_[w] = distTo_[v] + i->weight_;
                        from_[w] = i;
                        index_weight_heap_.Insert(w, distTo_[w]);
                    }
                }
            }
        }

        std::stack<Edge<Weight>*> stack{};
        while (from_[w]) {
            stack.push(from_[w]);
            w = from_[w]->n_;
        }
        std::vector<Edge<Weight>> ret{};
        while (!stack.empty()) {
            auto it = stack.top();
            stack.pop();
            ret.push_back(*it);
        }

        return ret;
    }

    // 负权边和Bellman-Ford算法
    std::vector<Edge<Weight>> NegativeShortestPathFromVToW(int v, int w) {
        from_.assign(graph_.GetNodeCnt(), nullptr);
        distTo_.assign(graph_.GetNodeCnt(), Weight());
        distTo_[v] = Weight();
        for (int i = 1; i < graph_.GetNodeCnt(); ++i) {

            // 松弛操作
            for (int j = 0; j < graph_.GetNodeCnt(); ++j) {
                typename Graph::Iterator it(graph_, j);
                for (auto e = it.begin(); !it.end(); e = it.next()) {
                    // 如果还没有到过B端点或者 A端点到起点的距离+这个边的长度<B端点到起点的距离
                    // 则应该选择这条边作为最短路径之一
                    if (!from_[e->m_] || distTo_[e->n_] + e->weight_ < distTo_[e->m_]) {
                        distTo_[e->m_] = distTo_[e->n_] + e->weight_;
                        from_[e->m_] = e;
                    }
                }
            }
        }

        // 检验是否有负权环
        if (DetectNegativeCycle()) {
            return {};
        }

        std::stack<Edge<Weight>*> stack{};
        while (from_[w]) {
            stack.push(from_[w]);
            w = from_[w]->n_;
        }
        std::vector<Edge<Weight>> ret{};
        while (!stack.empty()) {
            auto it = stack.top();
            stack.pop();
            ret.push_back(*it);
        }

        return ret;

    }


private:
    void Visit(int v) {
        assert(v >= 0 && v < graph_.GetNodeCnt());
        marked_[v] = true;
        typename Graph::Iterator it(graph_, v);
        for (auto i = it.begin(); !it.end(); i = it.next()) {
            if (!marked_[i->Other(v)]) {
                pq_.push(*i);
            }
        }
    }

    void Visit(int v, int) {
        marked_[v] = true;
        typename Graph::Iterator it(graph_, v);
        for (auto i = it.begin(); !it.end(); i = it.next()) {
            // 获取v点相连的顶点w
            int w = i->Other(v);
            if (!marked_[w]) {
                // 如果还没有索引w的边入堆，则直接入堆
                if (!edge_to_[w]) {
                    index_heap_.Insert(w, *i);
                    edge_to_[w] = i;
                    // 如果已经有索引w的边入堆了，则此时的权值比之前存的小，则更新，否则不更新
                } else if (edge_to_[w]->weight_ > i->weight_) {
                    index_heap_.ChangeItem(w, *i);
                    edge_to_[w] = i;
                }
            }
        }
    }

    // 判断是否有环
    bool DetectNegativeCycle() {
        for (int j = 0; j < graph_.GetNodeCnt(); ++j) {
            typename Graph::Iterator it(graph_, j);
            for (auto e = it.begin(); !it.end(); e = it.next()) {
                if (!from_[e->m_] || distTo_[e->n_] + e->weight_ < distTo_[e->m_]) {
                   return true;
                }
            }
        }
        return false;
    }

    Graph &graph_;

    // 标记为同一集合
    std::vector<bool> marked_;
    std::priority_queue<Edge<Weight>, std::vector<Edge<Weight>>, std::greater<Edge<Weight>>> pq_;

    // 索引堆
    IndexMinHeap<Edge<Weight>> index_heap_;
    // 索引对应的边
    std::vector<Edge<Weight> *> edge_to_;


    // 以下是最短路径算法需要的数据结构
    std::vector<Edge<Weight>*> from_; // 节点i的上一个节点
    std::vector<Weight> distTo_; // 从源节点到节点i的距离（权值）
    IndexMinHeap<Weight> index_weight_heap_; // 按照权重排序的最大堆
};