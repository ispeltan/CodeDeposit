//
// Created by tanpe on 2023/3/18.
//

#pragma once
#include <shared_mutex>
#include <vector>
#include <mutex>

// 访问者模式，线程安全的数组
template<class T>
class MTVector {
public:
    std::vector<T> vec_;
    std::shared_mutex mux_;

    class Accessor {
    public:
        Accessor(int write, MTVector& vec): vec_(vec.vec_), write_guard_(vec.mux_) {

        }
        Accessor(MTVector& vec): vec_(vec.vec_), read_guard_(vec.mux_) {

        }
        std::vector<T>& vec_;

    private:
        std::unique_lock<std::shared_mutex> write_guard_;
        std::shared_lock<std::shared_mutex> read_guard_;
    };
public:
    // 写
    Accessor WriteAccess() {
        return {1, *this};
    }

    // 读
    Accessor ReadAccess() {
        return {*this};
    }

};

