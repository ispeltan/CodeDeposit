//
// Created by tanpe on 2023/3/23.
//
#pragma once
#include <atomic>

class SpinLock {
public:
    void lock() {
        while(flag.test_and_set(std::memory_order_relaxed));
    }
    void unlock() {
        flag.clear(std::memory_order_relaxed);
    }
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};