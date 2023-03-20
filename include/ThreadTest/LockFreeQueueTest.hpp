//
// Created by tanpe on 2023/3/21.
//

#pragma once

#include <ThreadTest/LockFreeQueue.hpp>
#include <thread>

class LockFreeQueueTest {
public:
    static int Test() {
        std::atomic <uint16_t> seq;
        seq = 0;
        std::atomic<int> finish_producer;
        static constexpr int MULTI = 10;
        static constexpr int PRODUCER = 10;
        static constexpr int CONSUMER = 10;
        static constexpr int INT64_SIZE = 65536;
        static constexpr int QUEUE_SIZE = 1024;
        LockFreeQueue<uint16_t, QUEUE_SIZE> queue_;

        std::atomic<int> counter[INT64_SIZE] = {0};
        finish_producer = 0;

        auto produce = [&]() {
            for (int i = 0; i < INT64_SIZE * MULTI; ++i) {
                uint16_t s = seq++;
                while (!queue_.Push(s));
            }
            finish_producer++;
        };
        auto consume = [&]() {
            std::uint16_t s = 0;
            while (finish_producer < PRODUCER) {
                if (queue_.Pop(s)) {
                    counter[s]++;
                }
            }
            while (queue_.Pop(s)) {
                counter[s]++;
            }

        };

        std::vector<std::thread> producers(PRODUCER);
        std::vector<std::thread> consumers(CONSUMER);

        for (auto &it: producers) {
            it = std::thread(produce);
        }
        for (auto &it: consumers) {
            it = std::thread(consume);
        }

        for (auto &it: producers) {
            it.join();
        }
        for (auto &it: consumers) {
            it.join();
        }

        for (int i = 0; i < INT64_SIZE; i++) {
            if (counter[i] != MULTI * PRODUCER) {
                std::cout << "race " << i << " " << counter[i] << std::endl;
            }
        }
        return 0;
    }
};