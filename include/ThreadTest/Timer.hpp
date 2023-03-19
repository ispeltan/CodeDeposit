//
// Created by tanpe on 2023/3/19.
//
#pragma once

#include <chrono>
#include <set>
#include <functional>
#include <thread>
#include <iostream>
#include <list>
#include <ThreadTest/ConsumerModle.hpp>

class TimerBase {
public:
    std::int64_t id_;
    std::time_t poll_;
    std::time_t expire_;
    using Callback = std::function<void()>;
};

bool operator<(const TimerBase &left, const TimerBase &right) {
    if (left.expire_ < right.expire_) {
        return true;
    } else if (left.expire_ > right.expire_) {
        return false;
    }
    return left.id_ < right.id_;
}

class Timer : public TimerBase {
public:
    Callback call_;
};

class TimerManager {
public:
    TimerManager(int workers = 4) {
        workers_ = std::make_unique<ConsumerModel>(workers);
        thread_ = std::thread([&]() {
            while (true) {
                std::this_thread::sleep_for(chrono::milliseconds(10));
                auto now = GetTick();
                std::list<TimerBase::Callback> works{};
                std::unique_lock<std::mutex> lock(mux_);
                while (!timers_.empty()) {
                    if (timers_.begin()->expire_ > now) {
                        break;
                    }
                    auto timer = timers_.begin();
                    auto update = *timer;
                    timers_.erase(timer);
                    if (update.poll_ > 0) {
                        update.expire_ = now + update.poll_;
                        timers_.insert(update);
                    }
                    works.push_back(update.call_);
                }
                lock.unlock();
                for (auto &&work: works) {
                    workers_->AddMessage(std::move(work));
                }
            }
        });
    }
    ~TimerManager() {
        thread_.join();
    }
    std::int64_t AddTimer(std::time_t duration, std::time_t poll, const TimerBase::Callback &call) {
        if (++id_ < 0) {
            id_ = 0;
        };
        Timer timer;
        timer.call_ = call;
        timer.id_ = id_;
        timer.expire_ = GetTick() + duration;
        timer.poll_ = poll;
        std::unique_lock<std::mutex> lock(mux_);
        timers_.insert(timer);
        return id_.load();
    }
    bool StopTimer(std::int64_t id) {
        std::unique_lock<std::mutex> lock(mux_);
        for (auto it = timers_.begin(); it != timers_.end(); ++it) {
            if (it->id_ == id) {
                timers_.erase(it);
                return true;
            }
        }
        return false;
    }


    std::time_t GetTick() {
        auto cn_p = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
        auto cn_d = chrono::duration_cast<chrono::milliseconds>(cn_p.time_since_epoch());
        return cn_d.count();
    }

private:
    std::mutex mux_;
    std::set<Timer, std::less<>> timers_;
    std::thread thread_;
    static std::atomic<std::int64_t> id_;
    std::unique_ptr<ConsumerModel> workers_;
};

std::atomic<std::int64_t> TimerManager::id_ = 0;