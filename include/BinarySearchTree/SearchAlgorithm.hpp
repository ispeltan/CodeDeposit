//
// Created by tanpe on 2023/3/28.
//
#pragma once
#include <vector>

class SearchAlgorithm {
public:
    template<class T>
    static int BinarySearch(const std::vector<T>& array, T target) {
        if (array.empty()) {
            return -1;
        }
        int l = 0, r = array.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (array[mid] == target) {
                return mid;
            }
            if (array[mid] > target) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return -1;
    }
    // 方法2: 递归
    template<class T>
    static int RecursiveBinarySearch(const std::vector<T>& array, T target) {
        return DoBinarySearch(array, 0, array.size() - 1, target);
    }

    // floor
    template<class T>
    static int FloorSearch(const std::vector<T>& array, T target) {
        int l = 0, r = array.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (array[mid] == target) {
                r = mid - 1;
            } else if (array[mid] > target) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        if (r + 1 >= 0 && array[r + 1] == target) {
            return r + 1;
        }
        return r;
    }
    // ceil
    template<class T>
    static int CeilSearch(const std::vector<T>& array, T target) {
        int l = 0, r = array.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (array[mid] == target) {
                l = mid + 1;
            } else if (array[mid] < target) {
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        if (l - 1 < array.size() && array[l - 1] == target) {
            return  l - 1;
        }
        return l;
    }

private:
    template<class T>
    static int DoBinarySearch(const std::vector<T>& array, int l, int r, T target) {
        if (l > r) {
            return -1;
        }
        int mid = l + (r - l) / 2;
        if (array[mid] == target) {
            return mid;
        } else if (array[mid] > target) {
            return DoBinarySearch(array, l, mid - 1, target);
        } else {
            return DoBinarySearch(array, mid + 1, r, target);
        }
    }
};