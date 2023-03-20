//
// Created by tanpe on 2023/3/20.
//

#pragma once
#include <atomic>
#include <vector>
#include <iostream>


template<class T, std::size_t N>
class LockFreeQueue {
    class Element {
    public:
        std::atomic<bool> has_{false};
        T data_;
    };

public:
    LockFreeQueue() : data_(N) {
        write_index_.store(0);
        read_index_.store(0);
    }

    ~LockFreeQueue() {
        write_index_.store(0);
        read_index_.store(0);
        data_.clear();
    }

    bool Push(const T &t) {
        std::size_t write_index = 0;
        std::size_t read_index = 0;
        Element *element = nullptr;
        do {
            write_index = write_index_.load(std::memory_order_relaxed);
            read_index = read_index_.load(std::memory_order_relaxed);
            // 如果写满了
            if (write_index >= read_index + N) {
                return false;
            }
            // 如果有值，不能写入
            std::size_t index = write_index % N;
            element = &data_[index];
            if (element->has_.load(std::memory_order_relaxed)) {
                return false;
            }
        } while (!write_index_.compare_exchange_weak(write_index,
                                                     write_index + 1,
                                                     std::memory_order_release,
                                                     std::memory_order_relaxed));
        if (!element) {
            return false;
        }
        write_index = write_index_.load(std::memory_order_relaxed);
        read_index = read_index_.load(std::memory_order_relaxed);

        // 溢出保护
        if (write_index + 1 < 0) {
            while (!write_index_.compare_exchange_weak(write_index,
                                                       write_index % N + N,
                                                       std::memory_order_release,
                                                       std::memory_order_relaxed));
            while (!read_index_.compare_exchange_weak(read_index,
                                                     read_index % N,
                                                       std::memory_order_release,
                                                       std::memory_order_relaxed));
        }
        element->data_ = std::move(t);
        element->has_.store(true, std::memory_order_release);
        return true;
    }

    bool Pop(T& t) {
        std::size_t write_index = 0;
        std::size_t read_index = 0;
        Element *element = nullptr;
        do {
            write_index = write_index_.load(std::memory_order_relaxed);
            read_index = read_index_.load(std::memory_order_relaxed);
            if (read_index >= write_index) {
                return false;
            }
            // 如果无值，不能pop
            std::size_t index = read_index % N;
            element = &data_[index];
            if (!element->has_.load(std::memory_order_relaxed)) {
                return false;
            }
        } while (!read_index_.compare_exchange_weak(read_index,
                                                     read_index + 1,
                                                     std::memory_order_release,
                                                     std::memory_order_relaxed));
        if (!element) {
            return false;
        }
        write_index = write_index_.load(std::memory_order_relaxed);
        read_index = read_index_.load(std::memory_order_relaxed);

        // 溢出保护
        if (read_index + 1 < 0) {
            while (!write_index_.compare_exchange_weak(write_index,
                                                       write_index % N + N,
                                                       std::memory_order_release,
                                                       std::memory_order_relaxed));
            while (!read_index_.compare_exchange_weak(read_index,
                                                      read_index % N,
                                                      std::memory_order_release,
                                                      std::memory_order_relaxed));
        }
        t = std::move(element->data_);
        element->has_.store(false, std::memory_order_release);
        return true;
    }

    T Front() {
        auto write_index = write_index_.load(std::memory_order_relaxed);
        auto read_index = read_index_.load(std::memory_order_relaxed);
        if (read_index >= write_index) {
            return {};
        }
        // 如果无值
        std::size_t index = read_index % N;
        Element* element = &data_[index];
        if (!element->has_.load(std::memory_order_relaxed)) {
            return {};
        }
        return element->data_;
    }

private:

    std::atomic<std::size_t> write_index_;
    std::atomic<std::size_t> read_index_;
    std::vector<Element> data_;
};