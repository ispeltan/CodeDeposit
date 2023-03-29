//
// Created by tanpe on 2023/3/27.
//

#pragma once

#include <vector>
#include <cassert>
#include <string>
#include <sstream>

// 最大堆

template<class T>
class MaxHeap {
public:
    MaxHeap() {

    }
    // 数组堆化（heapify）
    MaxHeap(std::vector<T>& data) {
        data_ = data;
        // 从第一个非叶子节点开始逐一shiftdown
        auto not_leaf_index = (data_.size() - 1)/ 2;
        for (int i = not_leaf_index; i >= 0; --i) {
            ShiftDown(i);
        }
    }

    // 入队
    void Enqueue(T t) {
        data_.push_back(t);
        ShiftUp(data_.size() - 1);
    }

    // 出队
    T Dequeue() {
        assert(!Empty());
        T ret = data_[0];
        // 将最后一个元素和第一个元素进行交换，并删除最后一个元素
        data_[0] = data_[data_.size() - 1];
        data_.pop_back();

        // 对第一个元素进行shift down
        ShiftDown(0);
        return ret;
    }

    size_t Size() {
        return data_.size();
    }

    T Front() {
        assert(!Empty());
        return data_[0];
    }

    bool Empty() {
        return data_.empty();
    }

    std::string Print() {
        std::stringstream ss;
        for (int i = 0; i < data_.size(); ++i) {
            ss << data_[i];
            if (i < data_.size() - 1) {
                ss << ",";
            }
        }
        ss << std::endl;
        return ss.str();
    }

private:
    // 将其与父节点进行比较
    // 如果比父节点大，则交换，一直到根节点位置
    // 如果比父节点小，则退出
    void ShiftUp(int index) {
        while (index > 0) {
            auto parent_index = (index - 1) / 2;
            if (data_[parent_index] < data_[index]) {
                std::swap(data_[parent_index], data_[index]);
                index = parent_index;
            } else {
                return;
            }
        }
    }

    // 将其与子节点比较
    // 如果没有子节点，则退出
    // 如果只有左节点，则比较，若比左节点小，则交换位置，并继续
    // 如果左右子节点都存在，则和较大的节点比较，若比其大，则交换位置，并继续
    // 上述情况以外的情况均退出
    void ShiftDown(int index) {
        while (true) {
            auto left = 2 * index + 1;
            auto right = 2 * index + 2;
            auto cmp_index = left;
            if (right < data_.size()) {
                cmp_index = data_[left] > data_[right] ? left : right;
            } else if (left < data_.size()) {
                cmp_index = left;
            } else {
                break;
            }
            if (data_[cmp_index] > data_[index]) {
                std::swap(data_[cmp_index], data_[index]);
                index = cmp_index;
            } else {
                break;
            }
        }
    }

private:
    std::vector<T> data_;


};