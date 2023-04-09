//
// Created by tanpe on 2023/4/2.
//

#include <iostream>
#include "Graph/GraphTool.hpp"
#include "Graph/WeightedGraphTool.hpp"
#include <Tools.hpp>
#include <memory>

using namespace std;

int Test1() {

    int node_size = 5;
    auto sp = std::make_shared<SparseGraph>(node_size, false);
    auto dp = std::make_shared<DenseGraph>(node_size, false);

    int n = 10;
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        int p = rand() % node_size;
        int q = rand() % node_size;
        sp->AddEdge(p, q);
        dp->AddEdge(p, q);
    }

    cout << sp->Show();
    cout << dp->Show();

    typename SparseGraph::Iterator iter(*sp, 0);
    for (auto it = iter.begin(); !iter.end(); it = iter.next()) {
        cout << it << ",";
    }
    std::cout << std::endl;

    std::cout << sp->DepthFirstTraverse();

    std::cout << dp->DepthFirstTraverse();

    std::cout << "***" << std::endl;
    GraphTool<SparseGraph> sgt(*sp);
    GraphTool<DenseGraph> dgt(*dp);
    std::cout << sgt.Dfs();
    std::cout << dgt.Dfs();
    return 0;
}

void Test2() {
    auto sp = std::make_shared<SparseGraph>(7, false);
    auto dp = std::make_shared<DenseGraph>(7, false);
    GraphTool<SparseGraph> sgt(*sp);
    GraphTool<DenseGraph> dgt(*dp);
    const std::string fileName = Tools::GetCurrentDirName(__FILE__) + "graph.txt";
    sgt.FormGraphByFile(fileName);
    dgt.FormGraphByFile(fileName);

    cout << sp->Show();
    cout << dp->Show();
    cout << "***dfs***\n";
    cout << sgt.Dfs();
    cout << dgt.Dfs();
    cout << "GetConnectedNum = " << sgt.GetConnectedNum() << endl;
    cout << "GetConnectedNum = " << dgt.GetConnectedNum() << endl;

    cout << "ids = " << sgt.ShowIds() << endl;
    cout << "ids = " << dgt.ShowIds() << endl;

    cout << sgt.PathToQFromP(0, 4);

    cout << dgt.PathToQFromP(2, 4);

    cout << "***bfs***\n";
    cout << sgt.Bfs() << endl;
    cout << dgt.Bfs() << endl;

    cout << "ShortestPathLength = " << sgt.ShortestPathLength(2, 4) << endl;
    cout << "ShortestPathLength = " << dgt.ShortestPathLength(2, 4) << endl;

    cout << sgt.ShortestPathToQFromP(2, 4);
    cout << dgt.ShortestPathToQFromP(2, 4);

}

void Test3() {
    WeightedSparseGraph<double> s_graph(8, false);
    WeightedGraphTool<WeightedSparseGraph<double>, double> sgt(s_graph);

    WeightedDenseGraph<double> d_graph(8, false);
    WeightedGraphTool<WeightedDenseGraph<double>, double> dgt(d_graph);

    const std::string fileName = Tools::GetCurrentDirName(__FILE__) + "weighted_graph.txt";
    sgt.FormGraphByFile<double>(fileName);
    dgt.FormGraphByFile<double>(fileName);
    s_graph.Show();
    d_graph.Show();

    WeightedSparseGraph<double>::Iterator s_it(s_graph, 0);
    for (auto i = s_it.begin(); !s_it.end(); i = s_it.next()) {
        if (i != nullptr) {
            std::cout << i->m_ << ", " << i->n_ << ", " << i->weight_ << std::endl;
        }
    }

    WeightedDenseGraph<double>::Iterator d_it(d_graph, 0);
    for (auto i = d_it.begin(); !d_it.end(); i = d_it.next()) {
        if (i != nullptr) {
            std::cout << i->m_ << ", " << i->n_ << ", " << i->weight_ << std::endl;
        }
    }


    auto s_ret = sgt.MinSpanTree();
    auto d_ret = dgt.MinSpanTree();
    cout << "********prim********\n";
    for (auto &it: s_ret) {
        cout << it << " ";
    }
    cout << endl;

    for (auto &it: d_ret) {
        cout << it << " ";
    }
    cout << endl;

    auto s_ret2 = sgt.MinSpanTree(0);
    auto d_ret2 = dgt.MinSpanTree(0);
    cout << "*********index prim*******\n";
    for (auto &it: s_ret2) {
        cout << it << " ";
    }
    cout << endl;

    for (auto &it: d_ret2) {
        cout << it << " ";
    }
    cout << endl;

    auto s_ret3 = sgt.MinSpanTree(0, 0);
    auto d_ret3 = dgt.MinSpanTree(0, 0);
    cout << "********* Kruskal*******\n";
    for (auto &it: s_ret3) {
        cout << it << " ";
    }
    cout << endl;

    for (auto &it: d_ret3) {
        cout << it << " ";
    }
    cout << endl;
}

void Test4() {
    WeightedSparseGraph<int> s_graph(5, true);
    WeightedGraphTool<WeightedSparseGraph<int>, int> sgt(s_graph);

    WeightedDenseGraph<int> d_graph(5, true);
    WeightedGraphTool<WeightedDenseGraph<int>, int> dgt(d_graph);

    const std::string fileName = Tools::GetCurrentDirName(__FILE__) + "directed_weighted_graph.txt";
    sgt.FormGraphByFile<int>(fileName);
    dgt.FormGraphByFile<int>(fileName);
    s_graph.Show();
    d_graph.Show();

    auto sret = sgt.ShortestPathFromVToW(0, 4);
    for (auto &it: sret) {
        cout << "[" << it.n_ << "," << it.m_ << "," << it.weight_ << "]" << "->";
    }
    cout << endl;
    auto dret = sgt.ShortestPathFromVToW(0, 4);
    for (auto &it: dret) {
        cout << "[" << it.n_ << "," << it.m_ << "," << it.weight_ << "]" << "->";
    }
    cout << endl;
}


void Test5() {
    WeightedSparseGraph<int> s_graph(5, true);
    WeightedGraphTool<WeightedSparseGraph<int>, int> sgt(s_graph);

    WeightedDenseGraph<int> d_graph(5, true);
    WeightedGraphTool<WeightedDenseGraph<int>, int> dgt(d_graph);

    const std::string fileName = Tools::GetCurrentDirName(__FILE__) + "nagative_graph.txt";
    sgt.FormGraphByFile<int>(fileName);
    dgt.FormGraphByFile<int>(fileName);
    s_graph.Show();
    d_graph.Show();

    auto sret = sgt.NegativeShortestPathFromVToW(0, 4);
    for (auto &it: sret) {
        cout << "[" << it.n_ << "," << it.m_ << "," << it.weight_ << "]" << "->";
    }
    cout << endl;

    auto dret = sgt.NegativeShortestPathFromVToW(0, 4);
    for (auto &it: dret) {
        cout << "[" << it.n_ << "," << it.m_ << "," << it.weight_ << "]" << "->";
    }
    cout << endl;

}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}