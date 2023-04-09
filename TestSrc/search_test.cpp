//
// Created by tanpe on 2023/3/28.
//

#include "BinarySearchTree/SearchAlgorithm.hpp"
#include "BinarySearchTree/BinarySearchTree.hpp"
#include "SortTest/sort_algorithm.hpp"
#include <iostream>

using namespace std;

int main01() {
    auto arr = SortTestTool::Instance().RandomArray(10, 1, 10);
    SortAlgorithm::QuickSort_v3(arr);

    SortTestTool::Instance().PrintArray(arr);

    cout << SearchAlgorithm::BinarySearch(arr, 5) << endl;
    cout << SearchAlgorithm::RecursiveBinarySearch(arr, 5) << endl;
    cout << SearchAlgorithm::FloorSearch(arr, 2) << endl;
    cout << SearchAlgorithm::CeilSearch(arr, 2) << endl;

    return 0;
}

int main() {
    BinarySearchTree<int, double> tree;
    auto ret = tree.Insert(7, 9.0);
    auto ret2 = tree.Insert(1, 10.2);
    auto ret3 = tree.Insert(6, 1.0);
    auto ret4 = tree.Insert(4, 1.0);
    auto ret5 = tree.Insert(2, 1.0);
    auto ret6 = tree.Insert(1, 1.0);
    auto ret7 = tree.Insert(5, 1.0);

    cout << "ret: " << ret.first << ", " << ret.second.first << ", " << ret.second.second << endl;
    cout << "ret2: " << ret2.first << ", " << ret2.second.first << ", " << ret2.second.second << endl;
    cout << "ret3: " << ret3.first << ", " << ret3.second.first << ", " << ret3.second.second << endl;
    cout << "ret4: " << ret4.first << ", " << ret4.second.first << ", " << ret4.second.second << endl;
    cout << "ret5: " << ret5.first << ", " << ret5.second.first << ", " << ret5.second.second << endl;
    cout << "ret6: " << ret6.first << ", " << ret6.second.first << ", " << ret6.second.second << endl;
    cout << "ret7: " << ret7.first << ", " << ret7.second.first << ", " << ret7.second.second << endl;


    cout << tree.PreOrder() << endl;
    cout << tree.InOrder() << endl;
    cout << tree.PostOrder() << endl;
    cout << tree.LevelOrder() << endl;

    cout << "**********\n";
    cout << tree.Contain(11) << endl;
    cout << tree.Contain(7) << endl;
    cout << tree.Contain(2) << endl;

    auto sch1 = tree.Search(11);
    auto sch2 = tree.Search(7);
    auto sch3 = tree.Search(2);

    cout << (sch1? sch1->first: -1000) << "-" << (sch1? sch1->second: -1000) << endl;
    cout << (sch2? sch2->first: -1000) << "-" << (sch2? sch2->second: -1000) << endl;
    cout << (sch3? sch3->first: -1000) << "-" << (sch3? sch3->second: -1000) << endl;

    cout << "**********\n";
    auto max = tree.Maximum();
    auto min = tree.Minimum();
    cout << "max = " << (max? max->first: -1000) << endl;
    cout << "min = " << (min? min->first: -1000) << endl;

    cout << "**********\n";
    tree.DelMax();
    tree.DelMin();
    cout << tree.LevelOrder() << endl;

    cout << "******remove****\n";
    tree.Remove(6);
    tree.Remove(2);
    cout << tree.LevelOrder() << endl;

    cout << "******floor****\n";
    auto floor = tree.Floor(3);
    cout << (floor? floor->first: -1000) << "-" << (floor? floor->second: -1000) << endl;

    cout << "******ceil****\n";
    auto ceil = tree.Ceil(6);
    cout << (ceil? ceil->first: -1000) << "-" << (ceil? ceil->second: -1000) << endl;

    cout << "******cnt****\n";
    tree.Insert(3, 1);
    tree.Insert(13, 1);
    tree.Insert(31, 1);
    tree.Insert(103, 1);
    tree.CompleteCnt();
    cout << tree.LevelOrder() << endl;
    cout << tree.Rank(3) << endl;
    cout << tree.Rank(4) << endl;
    cout << tree.Rank(5) << endl;
    cout << tree.Rank(31) << endl;

    cout << "******Select****\n";
    auto select = tree.Select(1);
    cout << (select? select->first: -1000) << "-" << (select? select->second: -1000) << endl;


    return 0;
}