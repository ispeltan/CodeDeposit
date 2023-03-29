//
// Created by tanpe on 2023/3/28.
//

#pragma
#include <vector>

template<class T>
class HeapSort {
public:
    // 原地堆排序
    void DoHeapSort(std::vector<T>& data) {
        // 先进行heapify
        Heapify(data, data.size());

        // 将第一个元素和最后一个元素进行交换，然后对左侧的数组heapify,即:将第一个元素进行ShiftDown
        for (int i = data.size() - 1; i > 0; --i) {
            std::swap(data[0], data[i]);
            ShiftDown(data, 0, i);
        }

    }
private:
    // 对数组的0-n中的index元素进行shiftdown
    void ShiftDown(std::vector<T>& data, int index, int n) {
        while (true) {
            auto left_index = 2 * index + 1;
            auto right_index = 2 * index + 2;
            auto cmp_index = left_index;
            if (left_index >= n) {
                break;
            }
            if (right_index < n && data[right_index] > data[left_index]) {
                cmp_index = right_index;
            }
            if (data[index] < data[cmp_index]) {
                std::swap(data[index], data[cmp_index]);
                index = cmp_index;
            } else {
                break;
            }
        }
    }

    // 对数组的0-n的data进行堆化
    void Heapify(std::vector<T>& data, int n) {
        assert(n <= data.size());
        auto not_leaf_index = (n - 1) / 2;
        for (int i = not_leaf_index; i >= 0; --i) {
            ShiftDown(data, i, n);
        }
    }
};
