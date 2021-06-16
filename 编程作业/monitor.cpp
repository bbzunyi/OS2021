//
//  main.cpp
//  os_pa 3.3
//
//  Created by bb on 2021/5/14.
//  Copyright © 2021 mac. All rights reserved.
//
#include <iostream>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <boost.h>
class mesa_monitor : boost::noncopyable {
public:
    typedef boost::unique_lock<mesa_monitor> lock_type;
    friend class lock_type;
    mesa_monitor() : m_notify(0) {}
public:
    void lock() const {
        m_mutex.lock();
        m_notify = 0;  // 进入管程时要把m_notify归0
    }
    void unlock() const {
        notify_impl(m_notify);
        m_mutex.unlock();
    }
    bool try_lock() const {
        bool ret = m_mutex.try_lock();
        if (ret) {
            m_notify = 0;
        }
        return ret;
    }
    void wait() const {
        notify_impl(m_notify);
        m_cond.wait(m_mutex);
        m_notify = 0;
    }
    void notify_one() {
        if (m_notify != -1) {
            ++m_notify;
        }
    }
    
    void notify_all() {
        m_notify = -1;
    }
    
private:
    void notify_impl(int nnotify) const {
        if (nnotify != 0) {
            if (nnotify = -1) {
                m_cond.notify_all();
                return;
            } else {
                while (nnotify > 0) {
                    m_cond.notify_one();
                    --nnotify;
                }
            }
        }
    }
private:
    mutable boost::condition_variable_any m_cond;
    mutable boost::mutex m_mutex;
    mutable int m_notify;
};

template <typename T>
class threadsafe_queue : mesa_monitor {
    std::queue<T> m_data;
public:
    threadsafe_queue() {}
    void pop(T& val) {
        mesa_monitor::lock_type lk(*this);
        while (m_data.empty()) {
            wait();
        }
        val = m_data.front();
        m_data.pop();
    }
    void push(const T& val) {
        mesa_monitor::lock_type lk(*this);
        m_data.push(val);
        notify_one();
    }
};
