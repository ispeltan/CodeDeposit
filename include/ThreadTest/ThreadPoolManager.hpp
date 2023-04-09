//
// Created by tanpe on 2023/4/9.
//

#pragma once
#include <thread>
#include <vector>

class ThreadPoolManager {
public:
    static ThreadPoolManager *Instance() {
        static ThreadPoolManager instance;
        return &instance;
    }

    void AddThread(std::thread &&th) {
        threadPools.emplace_back(std::move(th));
    }

    std::vector<std::thread> threadPools;

    ~ThreadPoolManager() {
        for (auto &&th: threadPools) {
            th.join();
        }
    }
};
