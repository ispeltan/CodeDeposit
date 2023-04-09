//
// Created by tanpe on 2023/3/22.
//

#include <iostream>

using namespace std;

#include "SortTest/sort_algorithm.hpp"

int main() {
    auto arr = SortTestTool::Instance().RandomArray(10, 1, 100);
    SortTestTool::Instance().PrintArray(arr);

    SortAlgorithm::SelectSort(arr);
    SortTestTool::Instance().PrintArray(arr);


    auto arr21 = SortTestTool::Instance().RandomArray(10000, 1, 100000);

    auto arr31 = arr21;
    auto arr32 = arr21;
    auto arr33 = arr21;
    SortTestTool::Instance().SpeedTime("BubbleSort_v1", &SortAlgorithm::BubbleSort_v1<int>, arr31);
    SortTestTool::Instance().SpeedTime("BubbleSort_v2", &SortAlgorithm::BubbleSort_v2<int>, arr32);
    SortTestTool::Instance().SpeedTime("BubbleSort_v3", &SortAlgorithm::BubbleSort_v3<int>, arr33);

    auto arr41 = arr21;
    SortTestTool::Instance().SpeedTime("SelectSort", &SortAlgorithm::SelectSort<int>, arr41);

    auto arr51 = arr21;
    SortTestTool::Instance().SpeedTime("MergeSort", &SortAlgorithm::MergeSort<int>, arr51);

    auto arr52 = arr21;
    SortTestTool::Instance().SpeedTime("MergeSortBU", &SortAlgorithm::MergeSortBU<int>, arr52);

    auto arr61 = arr21;
    SortTestTool::Instance().SpeedTime("QuickSort", &SortAlgorithm::QuickSort<int>, arr61);

    // 近乎有序的数组
    auto arr_near_sort = SortTestTool::Instance().RandomNearlySortArray<int>(10000, 1, 10000, 10);
    auto arr_near_sort_1 = arr_near_sort;
    SortTestTool::Instance().SpeedTime("arr_near_sort MergeSortBU", &SortAlgorithm::MergeSortBU<int>, arr_near_sort);
    SortTestTool::Instance().SpeedTime("arr_near_sort QuickSort", &SortAlgorithm::QuickSort<int>, arr_near_sort_1);

    // 大量重复的数组
    auto repeat_sort = SortTestTool::Instance().RandomArray<int>(10000, 1, 10);
    auto repeat_sort_1 = repeat_sort;
    auto repeat_sort_2 = repeat_sort;
    SortTestTool::Instance().SpeedTime("repeat_sort MergeSortBU", &SortAlgorithm::MergeSortBU<int>, repeat_sort);
    SortTestTool::Instance().SpeedTime("repeat_sort QuickSort_v2", &SortAlgorithm::QuickSort_v2<int>, repeat_sort_1);
    SortTestTool::Instance().SpeedTime("repeat_sort QuickSort_v3", &SortAlgorithm::QuickSort_v3<int>, repeat_sort_1);


    return 0;
}