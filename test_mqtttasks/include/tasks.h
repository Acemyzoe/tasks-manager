#pragma once
#ifndef TAKS_H
#define TAKS_H

#include "ThreadPool.hpp"
#include "Queue.hpp"
#include "Msg.hpp"
#include "transwarp.h"
#include <iostream>
using namespace std;
namespace tw = transwarp;

#define TASK_TYPE_SERIAL 0 // 单线程
#define TASK_TYPE_ASYNC 1  // 异步线程池

#ifndef THREAD_NUM // 线程数量
#define THREAD_NUM 4
#endif
// tasks class
class Tasks
{
public:
    Tasks()
    {
        pool_ = new ThreadPool(THREAD_NUM);
        thread_num_ = THREAD_NUM;
    };
    ~Tasks()
    {
        delete pool_;
    };

    // 获取线程池信息
    vector<int> get_thread_info()
    {
        vector<int> thread_info;
        thread_info.push_back(pool_->thrCount());
        thread_info.push_back(pool_->idlCount());
        return thread_info;
    };

    // 添加任务
    template <class F, class... Args>
    void add_task(F &&f, Args &&...args, int task_type_)
    {
        if (task_type_ == TASK_TYPE_ASYNC)
        {
            auto res = pool_->enqueue(std::forward<F>(f), std::forward<Args>(args)...);
        }
        else if (task_type_ == TASK_TYPE_SERIAL)
        {
            // std::thread t(std::forward<F>(f), std::forward<Args>(args)...);
            // t.join();
            auto res = tw::make_task(tw::root, std::forward<F>(f), std::forward<Args>(args)...);
            res->schedule(executor_serial);
        }
        else
        {
            std::cout << "task_type error" << std::endl;
        }
    };

private:
    ThreadPool *pool_;
    int thread_num_;
    int task_type_;
    tw::sequential executor_serial;
    tw::parallel executor_parallel{4};
};

class Twtasks
{
public:
    Twtasks(){};
    ~Twtasks(){};

    // 添加任务
    template <class F, class... Args>
    void add_task(F &&f, Args &&...args, int task_type_)
    {
        auto res = tw::make_task(tw::root, std::forward<F>(f), std::forward<Args>(args)...);
        if (task_type_ == TASK_TYPE_ASYNC)
        {
            // tw::parallel executor_async{THREAD_NUM};
            // res->schedule(executor_async);
        }
        else if (task_type_ == TASK_TYPE_SERIAL)
        {
            // tw::sequential executor_serial;
            // res->schedule(executor_serial);
        }
        else
        {
            std::cout << "task_type error" << std::endl;
        }
    };

    // 获取线程池信息
    vector<int> get_thread_info()
    {
        vector<int> thread_info;
        thread_info.push_back(THREAD_NUM);
        return thread_info;
    };

private:
    int task_type_;
};

#endif