//
// Created by tanpe on 2023/4/2.
//

#pragma once
#include <cassert>
#include <vector>
#include <sstream>


class Graph {
public:
    Graph(int n, bool direct) {
        this->direct_ = direct;
        this->visited_.assign(n, false);
    }

    // 获取节点个数
    virtual int GetNodeCount() const = 0;

    // 获取边个数
    virtual int GetEdgeNum() const {
        return edge_num_;
    }

    // 节点p和节点q新增连接
    virtual void AddEdge(int p, int q) = 0;

    // 节点p和节点q是否相连
    virtual bool HasLink(int p, int q) = 0;

    // 打印信息
    virtual std::string Show() const = 0;

    // 深度优先遍历
    virtual std::string DepthFirstTraverse() = 0;

protected:
    // 是否为有向图
    bool direct_ = false;

    // 边的个数
    int edge_num_ = 0;

    // 是否被遍历过
    std::vector<bool> visited_;
};