//
// Created by tanpe on 2023/4/6.
//
#pragma once
#include <iostream>
template<typename Weight>
class Edge {
public:
    // 相连的两个边：n-m
    int n_;
    int m_;

    // 权重
    Weight weight_;
    Edge() = default;

    Edge(int n, int m, Weight w) {
        n_ = n;
        m_ = m;
        weight_ = w;
    }

    int Other(int n) {
        if (n == n_) {
            return m_;
        }
        if (n == m_) {
            return n_;
        }
        return -1;
    }

    friend std::ostream& operator<<(std::ostream& out, const Edge& edge) {
        out << "F:" << edge.n_ << " T:" << edge.m_ << " W:" << edge.weight_;
        return out;
    }

    bool operator<(const Edge& e) const {
        return this->weight_ < e.weight_;
    }
    bool operator>(const Edge& e) const {
        return this->weight_ > e.weight_;
    }
};