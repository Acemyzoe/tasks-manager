#include "tasks.h"
#include <iostream>
#include <time.h>
#include "mqtttasks.h"

int test_Task()
{
    Tasks tasks;
    int task_type = 0;
    tasks.add_task([]()
                   { 
                    //    std::this_thread::sleep_for(std::chrono::seconds(1));
                   std::cout<< "task1"<< "-" << std::this_thread::get_id() << std::endl; },
                   task_type);
    tasks.add_task([]()
                   { 
                    //    std::this_thread::sleep_for(std::chrono::seconds(1));
                   std::cout << "task2" << "-" << std::this_thread::get_id() << std::endl; },
                   task_type);
    tasks.add_task([]()
                   { 
                    //    std::this_thread::sleep_for(std::chrono::seconds(1));
                     std::cout << "task3" << "-" << std::this_thread::get_id() << std::endl; },
                   task_type);

    vector<int> thread_num = tasks.get_thread_info();
    std::cout << "thread_num: " << thread_num[0] << std::endl;
    return 0;
}

int test_mqtt()
{
    Tasks tasks;
    int task_type = 1;
    tasks.add_task(sub, task_type);
    tasks.add_task(pub, task_type);

    vector<int> thread_num = tasks.get_thread_info();
    std::cout << "thread_num: " << thread_num[0] << std::endl;
    return 0;
}

int test_msg()
{
    Msgqueue msgqueue;
    msgqueue.put_msg(1, std::string("hello"));
    msgqueue.put_msg(2, std::string("world"));
    std::cout << msgqueue.get_msg() << std::endl;
    std::cout << msgqueue.get_msg() << std::endl;
    return 0;
}

int test_tw()
{
    auto res = tw::make_task(tw::root, test_Task);
    auto res2 = tw::make_task(tw::root, test_mqtt);
    tw::parallel exec(4);
    tw::sequential exec_serial;
    tw::task_pool<int> pool{res};
    // res->schedule(exec);
    // res2->schedule(exec);
    std::cout << "pool size: " << pool.size() << std::endl;
    for (int i = 0; i < 2; i++)
    {
        auto task = pool.next_task(); // task may be null if the pool size is exhausted
        if (task)
        {
            task->schedule_all(exec);
        }
    }
    return 0;
}

int main()
{
    // test_Task();
    // test_mqtt();
    // test_msg();
    test_tw();
    return 0;
}