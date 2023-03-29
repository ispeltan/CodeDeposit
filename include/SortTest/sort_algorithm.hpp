//
// Created by tanpe on 2023/3/22.
//
#pragma once

#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <functional>
#include <chrono>

class SortTestTool {
public:
    static SortTestTool &Instance() {
        static SortTestTool tool;
        return tool;
    }

    template<class T>
    std::vector<T> RandomArray(std::size_t size, T min, T max) {
        assert(min <= max);
        srand(time(nullptr));
        std::vector<T> result{};
        for (int i = 0; i < size; ++i) {
            result.emplace_back(min + rand() % (int) (max - min + 1) * 1.0);
        }
        return result;
    }

    // 近乎有序的随机数组
    template<class T>
    std::vector<T> RandomNearlySortArray(std::size_t size, T min, T max, int num) {
        assert(min <= max);
        std::vector<T> result{};
        for (int i = 0; i < size; ++i) {
            result.emplace_back(min + rand() % (int) (max - min + 1) * 1.0);
        }

        srand(time(nullptr));
        for (int i = 0; i < num; ++i) {
            int pos = rand() % size;
            int pos2 = 1;
            while (pos == pos2) {
                pos = rand() % size;
            }
            std::swap(result[pos], result[pos2]);
        }
        return result;
    }

    template<class T>
    void PrintArray(std::vector<T> array) {
        int count = 0;
        for (const auto &it: array) {
            if (++count == array.size()) {
                std::cout << it;
            } else {
                std::cout << it << ", ";
            }
        }
        std::cout << std::endl;
    }

    template<class T>
    void SpeedTime(std::string name, std::function<void(std::vector<T> &)> sort, std::vector<T> &array) {
        auto start_time = GetTickTime();
        sort(array);
        auto end_time = GetTickTime();
        assert(SortSucc(array));

        std::cout << name << ": speed time = " << end_time - start_time << "ms" << std::endl;
    }

    template<class T>
    bool SortSucc(const std::vector<T> &array) {
        if (array.size() <= 1) {
            return true;
        }
        for (int i = 0; i < array.size() - 1; ++i) {
            if (array[i] > array[i + 1]) {
                return false;
            }
        }
        return true;
    }

    std::int64_t GetTickTime() {
        auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
        auto dp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
        return dp.count();
    }

private:
    SortTestTool() {

    }
};

class SortAlgorithm {
public:
    template<class T>
    static void SelectSort(std::vector<T> &array) {
        // 选择排序，逐一找队伍中最小值放在前面，寻找起点依次往后移动
        for (int i = 0; i < array.size() - 1; ++i) {
            int max_index = i;
            for (int j = i + 1; j < array.size(); ++j) {
                if (array[j] < array[max_index]) {
                    max_index = j;
                }
            }
            if (i != max_index) {
                std::swap(array[i], array[max_index]);
            }
        }
    }

    template<class T>
    static void InsertSort_v1(std::vector<T> &array) {
        // 插入排序
        for (int i = 1; i < array.size(); ++i) {
            // 将array[i] 在[0, i - 1]中找到合适的位置
            // 方法是向前逐一比较，如果前面的数值比当前数值大，则交换
            // 比较起点： a[j] 和a[j - 1] (j = i);
            // 比较终点： a[j] > a[j - 1] 或者 j = 1
            for (int j = i; j > 0; --j) {
                if (array[j] < array[j - 1]) {
                    std::swap(array[j], array[j - 1]);
                } else {
                    break;
                }
            }
        }
    }

    template<class T>
    static void InsertSort_v2(std::vector<T> &array) {
        // 插入排序优化：减少交换
        for (int i = 1; i < array.size(); ++i) {
            T tmp = array[i];
            for (int j = i; j > 0; --j) {
                if (array[j - 1] > tmp) {
                    array[j] = array[j - 1];
                } else {
                    if (i != j) {
                        array[j] = tmp;
                    }
                    break;
                }
            }

        }
    }

    // 冒泡排序
    template<class T>
    static void BubbleSort_v1(std::vector<T> &array) {
        // 浮出水面：往右
        for (int i = array.size() - 1; i > 0; --i) {
            for (int j = 0; j < i; ++j) {
                if (array[j] > array[j + 1]) {
                    std::swap(array[j], array[j + 1]);
                }
            }
        }
    }

    template<class T>
    static void BubbleSort_v2(std::vector<T> &array) {
        // 沉入湖底：往左
        for (int i = 0; i < array.size() - 1; ++i) {
            for (int j = array.size() - 1; j > i; --j) {
                if (array[j - 1] > array[j]) {
                    std::swap(array[j - 1], array[j]);
                }
            }

        }
    }

    template<class T>
    static void BubbleSort_v3(std::vector<T> &array) {
        // 优化
        // 某一次未发生交换，则后续无需再进行交换
        bool has_swapped = false;
        for (int i = 0; i < array.size() - 1; ++i) {
            for (int j = array.size() - 1; j > i; --j) {
                if (array[j - 1] > array[j]) {
                    std::swap(array[j - 1], array[j]);
                    has_swapped = true;
                }
            }
            if (!has_swapped) {
                break;
            }
        }
    }

    // 希尔排序
    template<class T>
    static void ShellSort(std::vector<T> &array) {
        int gap = array.size();
        while (gap > 1) {
            gap = gap / 3 + 1;
            for (int i = gap; i < array.size(); i += gap) {
                int tmp = array[i];
                for (int j = i; j - gap > 0; j -= gap) {
                    if (array[j] > array[j - gap]) {
                        array[j] = array[j - gap];
                    } else {
                        array[j] = tmp;
                        break;
                    }
                }
            }
        }
    }


    // 归并排序
    template<class T>
    static void MergeSort(std::vector<T> &array) {
        DoMergeSort(array, 0, array.size() - 1);
    }

    // 自底向上的归并排序
    template<class T>
    static void MergeSortBU(std::vector<T> &array) {
        // 逐一分组，从每组1个到每组2个到每组4个，依次类推
        for (int gl = 1; gl <= array.size(); gl += gl) {
            // 每两个组进行一次归并 [i, i + gl -1] 和[i + gl, i + gl + gl]
            for (int i = 0; i + gl < array.size(); i += gl + gl) {
                Merge(array, i, i + gl - 1, std::min(i + gl + gl - 1, (int) array.size() - 1));
            }
        }
    }

    // 快速排序
    template<class T>
    static void QuickSort(std::vector<T> &array) {
        srand(time(nullptr));
        DoQuickSort(array, 0, array.size() - 1);
    }

    // 快速排序
    template<class T>
    static void QuickSort_v2(std::vector<T> &array) {
        srand(time(nullptr));
        DoQuickSort_v2(array, 0, array.size() - 1);
    }

    // 三路快速排序
    template<class T>
    static void QuickSort_v3(std::vector<T> &array) {
        srand(time(nullptr));
        DoQuickSort_v3(array, 0, array.size() - 1);
    }


private:
    template<class T>
    static void DoMergeSort(std::vector<T> &array, int l, int r) {
        if (l >= r) {
            return;
        }
        int mid = (l + r) / 2;
        DoMergeSort(array, l, mid);
        DoMergeSort(array, mid + 1, r);
//        Merge(array, l, mid, r);
        Merge_2(array, l, mid, r);

    }

    template<class T>
    static void Merge(std::vector<T> &array, int l, int mid, int r) {
        std::vector<T> tmp(r - l + 1);
        int left = l;
        int right = mid + 1;
        for (int i = l; i <= r; ++i) {
            // 这里有个暗含逻辑：一方越界的时候，另一方一定还没有越界
            if (left > mid) {
                tmp[i - l] = array[right];
                right++;
            } else if (right > r) {
                tmp[i - l] = array[left];
                left++;
            } else if (array[left] < array[right]) {
                tmp[i - l] = array[left];
                left++;
            } else {
                tmp[i - l] = array[right];
                right++;
            }
        }

        int index = 0;
        for (int i = l; i <= r; ++i) {
            array[i] = tmp[index++];
        }
    }

    template<class T>
    static void Merge_2(std::vector<T> &array, int l, int mid, int r) {
        std::vector<T> tmp(r - l + 1);
        int left = l;
        int right = mid + 1;
        int index = 0;
        // 这里有个暗含逻辑：一方越界的时候，另一方一定还没有越界
        while (left <= mid && right <= r) {
            if (array[left] < array[right]) {
                array[index++] = array[left++];
            } else {
                array[index++] = array[right++];
            }
        }
        while (left <= mid) {
            array[index++] = array[left++];
        }
        while (right <= r) {
            array[index++] = array[right++];
        }

        index = 0;
        for (int i = l; i <= r; ++i) {
            array[i] = tmp[index++];
        }
    }

    template<class T>
    static void DoQuickSort(std::vector<T> &array, int l, int r) {
        if (l >= r) {
            return;
        }
        // [part + 1, i] <= arr[part]
        // [i + 1, j - 1] > arr[part]
        // 优化点：随机选择一个位置的数作为枢轴
        int part = rand() % (r - l + 1) + l;
        swap(array[part], array[l]);

        int i = l;
        int j = l + 1;
        while (j <= r) {
            if (array[j] > array[l]) {
                j++;
            } else {
                std::swap(array[i + 1], array[j]);
                i++;
                j++;
            }
        }

        std::swap(array[l], array[i]);
        DoQuickSort(array, l, i - 1);
        DoQuickSort(array, i + 1, r);
    }

    template<class T>
    static void DoQuickSort_v2(std::vector<T> &array, int l, int r) {
        if (l >= r) {
            return;
        }
        // 优化点：随机选择一个位置的数作为枢轴
        int part = rand() % (r - l + 1) + l;
        swap(array[part], array[l]);

        // [l + 1, i] <= arr[part]
        // [j, r] >= arr[part]
        int i = l + 1;
        int j = r;

        while (true) {
            while (i <=r && array[i] < array[l]) {
                i++;
            }
            while (j > l + 1 && array[j] > array[l]) {
                j--;
            }
            if (i > j) {
                break;
            }
            swap(array[i], array[j]);
            i++;
            j--;
        }

        std::swap(array[l], array[j]);
        DoQuickSort_v2(array, l, j - 1);
        DoQuickSort_v2(array, j + 1, r);
    }

    template<class T>
    static void DoQuickSort_v3(std::vector<T> &array, int l, int r) {
        if (l >= r) {
            return;
        }
        // 优化点：随机选择一个位置的数作为枢轴
        int part = rand() % (r - l + 1) + l;
        std::swap(array[part], array[l]);

        // [l + 1, lt] < arr[part]
        // [lt + 1, gt - 1] == arr[part]
        // [gt, r] > arr[part]
        int lt = l;
        int gt = r + 1;
        int i = l + 1;
        while (i < gt) {
            if (array[i] == array[l]) {
                i++;
            } else if (array[i] < array[l]) {
                std::swap(array[lt + 1], array[i]);
                lt++;
                i++;
            } else {
                std::swap(array[gt - 1], array[i]);
                gt--;
            }
        }

        std::swap(array[l], array[lt]);
        DoQuickSort_v3(array, l, lt - 1);
        DoQuickSort_v3(array, gt,  r);
    }


};