//
// Created by tanpe on 2023/3/19.
//
#pragma once

#include <mutex>
#include <thread>
#include <future>
#include <list>
#include <queue>


class ConsumerModel {
public:
    ConsumerModel(int n) {
        for (int i = 0; i < n; ++i) {
            threads_.push_back(std::thread([&](){

                while (true) {
                    std::unique_lock<std::mutex> lock(mux_);
                    cv_.wait(lock, [&]()->bool {
                        return !messages_.empty() || stop_.load();
                    });
                    if (stop_.load()) {
                        break;
                    }

                    if (!messages_.empty()) {
                        auto message = messages_.front();
                        messages_.pop();
                        lock.unlock();
                        message();
                    }

                }

            }));
        }
    }
    ~ConsumerModel() {
        for (auto&& it: threads_) {
            it.join();
        }
    }

    void AddMessage(std::function<void()>&& func) {
        std::unique_lock<std::mutex> lock(mux_);
        messages_.push(func);
        cv_.notify_all();
    }
    void Stop() {
        stop_.store(true);
        cv_.notify_all();
    }

private:
    std::list<std::thread> threads_;
    std::mutex mux_;
    std::condition_variable cv_;
    std::atomic<bool> stop_ = false;
    std::queue<std::function<void()>> messages_;
};