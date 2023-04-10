//
// Created by tanpe on 2023/3/28.
//
#pragma once

#include <vector>
#include <cassert>
#include <sstream>

// 索引最小堆
template<class T>
class IndexMinHeap {
public:


    // 不指定索引堆大小的堆
    IndexMinHeap() = default;

    // 指定索引堆大小的堆
    IndexMinHeap(int n) {
        data_.assign(n, T());
        reverses_.assign(n, -1);
    }

    // 向索引堆插入元素, 按照插入顺序递增索引
    void Insert(T t) {
        data_.push_back(t);
        reverses_.push_back(indexes_.size());
        indexes_.push_back(indexes_.size());
        ShiftUp(indexes_.size() - 1);
    }
    // 向索引堆中插入元素，按照索引插入
    void Insert(int index, T t) {
        assert(index >= 0 && index < data_.size());
        data_[index] = t;
        indexes_.push_back(index);
        reverses_[index] = indexes_.size() - 1;
        ShiftUp(indexes_.size() - 1);
    }

    // 获取索引为index的元素值
    T GetItem(int index) {
        assert(index >= 0 && index < data_.size());
        return data_[index];
    }

    // 获取根节点的索引值
    int ExtractMinIndex() {
        assert(!Empty());
        int ret = indexes_[0];
        std::swap(indexes_[0], indexes_[indexes_.size() - 1]);
        // i为堆的下标 index[i] 即元素下标，reverse[index[i]] 即元素在堆中的位置
        reverses_[indexes_[0]] = 0;
        reverses_[indexes_[indexes_.size() - 1]] = -1;
        indexes_.pop_back();
        ShiftDown(0);
        return ret;
    }

    // 获取根节点的数据
    T ExtractMinItem() {
        assert(!Empty());
        T ret = data_[indexes_[0]];
        std::swap(indexes_[0], indexes_[indexes_.size() - 1]);
        reverses_[indexes_[0]] = 0;
        reverses_[indexes_[indexes_.size() - 1]] = -1;
        indexes_.pop_back();
        ShiftDown(0);
        return ret;
    }

    // 改变索引为index的数据
    bool ChangeItem(int index, T t) {
        // 方式1：找到索引index
//        for (int i = 0; i < indexes_.size(); ++i) {
//            if (indexes_[i] == index) {
//                // 修改值
//                data_[index] = t;
//                // 替换树中任一元素，需要Up和Down各执行一次。次序无先后
//                ShiftDown(i);
//                ShiftUp(i);
//                return true;
//            }
//        }
        // 方式2： 使用reverse
        auto i = reverses_[index];
        if (i < 0) {
            return false;
        }
        data_[index] = t;
        ShiftDown(i);
        ShiftUp(i);
        return true;

        return false;
    }

    // 返回大小
    std::size_t Size() {
        return data_.size();
    }

    // 是否为空
    bool Empty() {
        return indexes_.empty();
    }

    std::string PrintData() {
        std::stringstream ss;
        for (auto &&it: data_) {
            ss << it << " ";
        }
        ss << std::endl;
        return std::string(ss.str());
    }

    std::string PrintIndex() {
        std::stringstream ss;
        for (auto &&it: indexes_) {
            ss << it << " ";
        }
        ss << std::endl;
        return std::string(ss.str());
    }
    std::string PrintReverse() {
        std::stringstream ss;
        for (auto &&it: reverses_) {
            ss << it << " ";
        }
        ss << std::endl;
        return std::string(ss.str());
    }


private:
    void ShiftDown(int index) {
        while (true) {
            auto left_index = index * 2 + 1;
            auto right_index = index * 2 + 2;
            auto cmp_index = left_index;
            if (left_index >= indexes_.size()) {
                break;
            }
            if (right_index < indexes_.size()) {
                if (data_[indexes_[right_index]] < data_[indexes_[left_index]]) {
                    cmp_index = right_index;
                }
            }
            if (data_[indexes_[index]] > data_[indexes_[cmp_index]]) {
                std::swap(indexes_[cmp_index], indexes_[index]);
                reverses_[indexes_[cmp_index]] = cmp_index;
                reverses_[indexes_[index]] = index;
                index = cmp_index;
            } else {
                break;
            }
        }
    }

    void ShiftUp(int index) {
        while (index > 0) {
            auto parent_index = (index - 1) / 2;
            if (data_[indexes_[parent_index]] > data_[indexes_[index]]) {
                std::swap(indexes_[parent_index], indexes_[index]);
                reverses_[indexes_[parent_index]] = parent_index;
                reverses_[indexes_[index]] = index;
                index = parent_index;
            } else {
                break;
            }
        }

    }

    void Heapify() {
        auto not_leaf_index = (indexes_.size() - 1) / 2;
        for (int i = not_leaf_index; i >= 0; --i) {
            ShiftDown(i);
        }
    }

    std::vector<T> data_;
    std::vector<int> indexes_;
    std::vector<int> reverses_;

};
