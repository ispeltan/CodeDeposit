//
// Created by tanpe on 2023/3/27.
//

#include "Heap/MaxHeap.hpp"
#include "Heap/HeapSort.hpp"
#include "Heap/IndexMaxHeap.hpp"
#include <iostream>
#include <functional>

int main() {
    MaxHeap<int> heap;
    srand(time(nullptr));
    for (int i = 0; i < 20; ++i) {
        heap.Enqueue(rand() % 20);
    }
    std::cout << heap.Print();

    for (int i = 0; i < 20; ++i) {
        std::cout << heap.Dequeue() << " ";
    }
    std::cout << "\n*********" << std::endl;

    std::vector<int> data{};
    for (int i = 0; i < 20; ++i) {
        data.push_back(rand() % 20);
    }
    MaxHeap<int>heap2(data);
    std::cout << heap2.Print();
    for (int i = 0; i < 20; ++i) {
        std::cout << heap2.Dequeue() << " ";
    }
    std::cout << "\n*********" << std::endl;
    std::vector<int> data2{};
    for (int i = 0; i < 20; ++i) {
        data2.push_back(rand() % 20);
    }
    HeapSort<int>().DoHeapSort(data2);
    for (int i = 0; i < 20; ++i) {
        std::cout << data2[i] << " ";
    }
    std::cout << "\n*****heap3****" << std::endl;
    IndexMaxHeap<int> heap3;
    for (int i = 0; i < 5; ++i) {
        heap3.Insert(rand() % 20);
    }
    std::cout << heap3.PrintData();
    std::cout << heap3.PrintIndex();
    std::cout << heap3.PrintReverse();
    std::cout << "\n*********" << std::endl;
    heap3.ChangeItem(2, 111);
    std::cout << heap3.PrintData();
    std::cout << heap3.PrintIndex();
    std::cout << heap3.PrintReverse();

    return 0;
}