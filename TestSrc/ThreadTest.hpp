//
// Created by tanpe on 2023/3/15.
//
#pragma once

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <shared_mutex>
#include <functional>
#include <future>
#include "Tools.hpp"
#include "ThreadTest/ThreadPoolManager.hpp"
#include "ThreadTest/ConsumerModle.hpp"

using namespace std;

void WaitTest() {
    auto time = std::chrono::steady_clock::now();
    using double_ms = std::chrono::duration<double, std::milli>;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto time2 = std::chrono::steady_clock::now();

    std::cout << "delta = " << std::chrono::duration_cast<double_ms>(time2 - time).count() << std::endl;
}

void DurationTest() {
    auto time = std::chrono::steady_clock::now();
    int a = 0;
    for (volatile int i = 0; i < 1000000000; ++i) {
        ++a;
    }
    std::cout << "a = " << a << std::endl;

    // 时间差测试
    auto time_now = std::chrono::steady_clock::now();
    auto delta = time_now - time;
    using double_ms = std::chrono::duration<double, std::milli>;
    std::cout << "delta time = " << std::chrono::duration_cast<double_ms>(delta).count() << std::endl;
}


// call once test
void Func() {
    cout << "call func" << endl;
}

void CallOnce() {
    static std::once_flag flag;
    std::call_once(flag, Func);
};

class CallTest {
public:
    void Test(int a, double b) {
        cout << "a = " << a << "b = " << b << endl;
    }
};


int Test() {
    {
        auto th = thread([]() {
            this_thread::sleep_for(std::chrono::milliseconds(1000));
            cout << "thread" << endl;
        });
        th.detach();
    }

    for (int i = 0; i < 5; ++i) {
        auto th = thread(CallOnce);
        th.detach();
    }

    CallTest test;
    for (int i = 0; i < 5; ++i) {
        auto th = thread(&CallTest::Test, &test, i, i * 1.0);
        th.detach();
    }

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    return 0;
}

class Worker {
public:
    void DoWork(int id) {
        cout << "DoWork id = " << id << " threadId[" << this_thread::get_id() << "]" << endl;
        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "---Loading " << (i + 1) * 10 << "% completed---" << endl;
        }
        cout << "all completed " << endl;
    }

    static void DoStaticWork(int id) {
        cout << "DoWork id = " << id << " threadId[" << this_thread::get_id() << "]" << endl;

        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "---Loading " << (i + 1) * 10 << "% completed---" << endl;
        }
        cout << "all completed " << endl;
    }

    static int DoAsyncWork(int id) {
        cout << "DoWork id = " << id << " threadId[" << this_thread::get_id() << "]" << endl;

        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "---Loading " << (i + 1) * 10 << "% completed---" << endl;
        }
        cout << "all completed " << endl;
        return 101;
    }

};


void CreateThread() {
    thread th([]() {
        Worker::DoStaticWork(2);
    });
    ThreadPoolManager::Instance()->AddThread(std::move(th));
//    cout << "join able " << th.joinable() <<endl;
//    cout << "Do Other things" << endl;
//    th.join();
//    th.detach();
}

void Async() {
    std::future<int> ret = async([]() {
        return Worker::DoAsyncWork(3);
    });
//    this_thread::sleep_for(chrono::seconds (2));

    while (true) {

        auto flag = ret.wait_for(chrono::seconds(1));
        if (flag == future_status::ready) {
            cout << "completed" << endl;
            break;
        } else if (flag == future_status::deferred) {
            cout << "no completed" << endl;
        } else {
            cout << "timeout" << endl;
        }

    }
    cout << "ret = " << ret.get() << endl;
}

void AsyncDefer() {
    std::future<int> ret = async(launch::deferred, []() {
        return Worker::DoAsyncWork(3);
    });
    cout << "ret = " << ret.get() << endl;
}

void Promise() {
    promise<int> promise_ret;
    thread th([&promise_ret]() {
        auto ret = Worker::DoAsyncWork(4);
        promise_ret.set_value(ret);
    });
    future<int> fu = promise_ret.get_future();
    cout << "ret = " << fu.get() << endl;
    th.join();
}


void FutureCopy() {

    shared_future<int> fu = async([]() {
        return Worker::DoAsyncWork(5);
    });
    auto f1 = fu;
    auto f2 = f1;
    // 允许多次调用
    int ret = f1.get();
    cout << "ret = " << f1.get() << endl;
}

shared_mutex mutex_;
std::vector<int> vec;

void TestDataCmp() {

    thread th1([&]() {
        unique_lock<shared_mutex> lock(mutex_);
        for (int i = 0; i < 10; i++) {
            cout << this_thread::get_id() << "B";
            this_thread::sleep_for(chrono::milliseconds(500));
            vec.push_back(i);
        }
    });

    thread th2([&]() {
        for (int i = 0; i < 10; i++) {
            unique_lock<shared_mutex> lock(mutex_);
            cout << this_thread::get_id() << "A";
            this_thread::sleep_for(chrono::milliseconds(500));
            vec.push_back(-i);
        }
    });
    ThreadPoolManager::Instance()->AddThread(std::move(th1));
    ThreadPoolManager::Instance()->AddThread(std::move(th2));

}


template<class T>
class Concept {
public:
    Concept(T &t) : t_(t) {
        t_.lock();
    }

    ~Concept() {
        t_.unlock();
    }

    T &t_;
};

class ConceptTest {
public:
    void lock() {
        cout << "lock" << endl;
    }

    void unlock() {
        cout << "unlock" << endl;
    }
};


void mTestDataCmp() {
    mutex m_mutex_;
    std::vector<int> m_vec;
    promise<int> ret1, ret2;
    thread th2([&]() {
        for (int i = 0; i < 10; i++) {
            unique_lock<mutex> lock(m_mutex_, defer_lock);
            lock.lock();
            cout << this_thread::get_id() << "A";
            this_thread::sleep_for(chrono::milliseconds(500));
            m_vec.push_back(-i);
        }
        ret2.set_value(1);
    });

    thread th1([&]() {
        unique_lock<mutex> lock(m_mutex_, try_to_lock);
        if (lock.owns_lock()) {
            cout << "lock1" << endl;
//            lock.lock();
            cout << "lock2" << endl;


            for (int i = 0; i < 10; i++) {
                cout << this_thread::get_id() << "B";
                this_thread::sleep_for(chrono::milliseconds(500));
                m_vec.push_back(i);
            }
            ret1.set_value(1);
        } else {
            cout << "wait" << endl;
        }

    });
    auto fu1 = ret1.get_future();
    auto fu2 = ret2.get_future();
    cout << "********************" << endl;
    if (fu1.get() == 1 && fu2.get() == 1) {
        for (auto &&info: m_vec) {
            cout << info << " ";
        }
    }
    ThreadPoolManager::Instance()->AddThread(std::move(th1));
    ThreadPoolManager::Instance()->AddThread(std::move(th2));
}

void TestDataCmpLock() {
    mutex m_mutex_;
    mutex m_mutex_2;
    recursive_mutex rec_mutex;
    std::vector<int> m_vec;
    promise<int> ret1, ret2;
    thread th2([&]() {
        for (int i = 0; i < 10; i++) {
//            unique_lock<mutex> lock(m_mutex_);
//            unique_lock<mutex> lock2(m_mutex_2);
            scoped_lock(m_mutex_, m_mutex_2);
            cout << this_thread::get_id() << "A";
            this_thread::sleep_for(chrono::milliseconds(500));
            m_vec.push_back(-i);
        }
        ret2.set_value(1);
    });

    auto func = [&]() {
        rec_mutex.lock();
        this_thread::sleep_for(chrono::milliseconds(500));
        rec_mutex.unlock();
    };

    thread th1([&]() {
        scoped_lock(m_mutex_, m_mutex_2);
//        unique_lock<recursive_mutex> lock3(rec_mutex);
//        rec_mutex.lock();
//        rec_mutex.lock();
//        func();
//        rec_mutex.unlock();
        for (int i = 0; i < 10; i++) {
//            unique_lock<recursive_mutex> lock3(rec_mutex);
//            rec_mutex.lock();
            cout << this_thread::get_id() << "B";
            this_thread::sleep_for(chrono::milliseconds(500));
            m_vec.push_back(i);
        }
        ret1.set_value(1);
    });
    auto fu1 = ret1.get_future();
    auto fu2 = ret2.get_future();
    cout << "********************" << endl;
    if (fu1.get() == 1 && fu2.get() == 1) {
        for (auto &&info: m_vec) {
            cout << info << " ";
        }
    }
    ThreadPoolManager::Instance()->AddThread(std::move(th1));
    ThreadPoolManager::Instance()->AddThread(std::move(th2));

}

#include "ThreadTest/MTVector.hpp"

int main01() {
    {
        ConceptTest test1;
        Concept<ConceptTest> test(test1);
    }
//    mTestDataCmp();
    TestDataCmpLock();
    return 0;
}


int main02() {
    MTVector<int> vec;
    auto th1 = thread([&]() {
        for (int i = 0; i < 10; ++i) {
            auto acc = vec.WriteAccess();
            acc.vec_.push_back(i);
            cout << "Push" << i << endl;
        }
    });
    auto th2 = thread([&]() {
        for (int i = 0; i < 10; ++i) {
            auto acc = vec.WriteAccess();
            acc.vec_.push_back(-i);
            cout << "Push" << -i << endl;
        }
    });
    auto th3 = thread([&]() {
        auto acc = vec.ReadAccess();
        for (auto &&it: acc.vec_) {
            cout << "3read" << it << endl;
        }
        cout << endl;
    });
    auto th4 = thread([&]() {
        auto acc = vec.ReadAccess();
        for (auto &&it: acc.vec_) {
            cout << "4read" << it << endl;
        }
        cout << endl;
    });
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    for (auto &&it: vec.vec_) {
        cout << it << " ";
    }

    return 0;
}


int main03() {
    condition_variable_any cv;
    shared_mutex mux;
    bool ready = false;

    auto th1 = thread([&]() {
        unique_lock<shared_mutex> lock(mux);
        cv.wait(lock, [&]() -> bool {
            return ready;
        });
        cout << this_thread::get_id() << "do" << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));

    });
    auto th2 = thread([&]() {
        unique_lock<shared_mutex> lock(mux);
        cv.wait(lock, [&]() -> bool {
            return ready;
        });
        cout << this_thread::get_id() << "do" << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));

    });


    this_thread::sleep_for(chrono::milliseconds(500));
    ready = true;
//    cv.notify_one();
//    cv.notify_one();

    cv.notify_all();
    cout << "A" << endl;
    th2.join();
    cout << "B" << endl;

    th1.join();
    cout << "C" << endl;

//    ready = true;
    cv.notify_one();


    return 0;
}

std::atomic<int> i = 0;

void TestFunc() {
    for (int a = 0; a < 10000; a++) {
        i++;
    }
    cout << "Th[" << this_thread::get_id() << "] Do Work i = " << endl;
    this_thread::sleep_for(std::chrono::milliseconds(2000));
}

int main04() {
    ConsumerModel test(5);
    for (int i = 0; i < 5; ++i) {
        test.AddMessage(TestFunc);
    }
    this_thread::sleep_for(std::chrono::milliseconds(1000) * 5);
    test.Stop();
    cout << "i = " << i << endl;
    return 0;
}

#include "ThreadTest/Timer.hpp"

int main05() {
    atomic<int> a = 9;
    int old = 9;
//    cout << a.fetch_add(9) << "," << a << endl;
    cout << a.compare_exchange_strong(old, 3) << "," << old << "," << a << endl;
    cout << a.compare_exchange_weak(old, 3) << "," << old << "," << a << endl;
    cout << a.exchange(7) << ", " << a << endl;

    return 0;
}


std::time_t g_now = 0;

void TimerTestFunc() {
    cout << "Th[" << this_thread::get_id() << "] Do Work time[" << Tools::GetTick() - g_now << "]" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void TimerTestFunc2() {
    cout << "Th[" << this_thread::get_id() << "] Do Work2 time[" << Tools::GetTick() - g_now << "]" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int main07() {
    g_now = Tools::GetTick();
    auto manager = std::make_unique<TimerManager>(20);
    auto id1 = manager->AddTimer(2000, 1000, TimerTestFunc);
    auto id2 = manager->AddTimer(5000, 2000, TimerTestFunc2);

    this_thread::sleep_for(std::chrono::milliseconds(30000));
    cout << "del " << manager->StopTimer(id2) << endl;


    return 0;
}


#include "ThreadTest/LockFreeQueue.hpp"
#include "ThreadTest/LockFreeQueueTest.hpp"

int main0001() {
    LockFreeQueueTest::Test();
    return 0;
}

#include "ThreadTest/SpinLock.hpp"

SpinLock test_spin_lock;
mutex test_mutex;

void SpinTest(int size, int sleep_time) {
    int a = 0;
    auto start = Tools::GetTick();
    auto th1 = thread([&]() {
        for (int i = 0; i < size; ++i) {
            std::unique_lock<SpinLock> lock(test_spin_lock);
            this_thread::sleep_for(std::chrono::microseconds(sleep_time));
            a++;
        }
    });
    auto th2 = thread([&]() {
        for (int i = 0; i < size; ++i) {
            std::unique_lock<SpinLock> lock(test_spin_lock);
            this_thread::sleep_for(std::chrono::microseconds(sleep_time));
            a++;
        }
    });
    th1.join();
    th2.join();
    auto end = Tools::GetTick();
    std::cout << "SpinTest a = " << a << " time = " << end - start << std::endl;
}

void MutexTest(int size, int sleep_time) {
    int a = 0;
    auto start = Tools::GetTick();
    auto th1 = thread([&]() {
        for (int i = 0; i < size; ++i) {
            std::unique_lock<mutex> lock(test_mutex);
            this_thread::sleep_for(std::chrono::microseconds(sleep_time));
            a++;
        }
    });
    auto th2 = thread([&]() {
        for (int i = 0; i < size; ++i) {
            std::unique_lock<mutex> lock(test_mutex);
            this_thread::sleep_for(std::chrono::microseconds(sleep_time));
            a++;
        }
    });
    th1.join();
    th2.join();
    auto end = Tools::GetTick();
    std::cout << "MutexTest a = " << a << " time = " << end - start << std::endl;
}

int main() {
    int size = 100000;
    int sleep_time = 1000;
    MutexTest(size, sleep_time);
    SpinTest(size, sleep_time);
    return 0;
}


