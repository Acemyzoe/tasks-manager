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
    std::cout << "threadpool_size: " << thread_num[0] << std::endl;
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

#include "json/nlohmann/json.hpp"
using json = nlohmann::json;
int test_msg()
{
    string msg = "{\"name\":\"test\",\"age\":\"20\"}";
    string msg2 = R"({"happy": true, "pi": 3.141})";

    json j = json::parse(msg2);
    string msg3 = j.dump();

    json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {{"everything", 42}}},
        {"list", {1, 0, 2}},
        {"object", {{"currency", "USD"}, {"value", 42.99}}}};
    string msg4 = j2.dump();

    Msgqueue msgqueue;
    msgqueue.put_msg(1, msg2);
    msgqueue.put_msg(2, msg3);
    msgqueue.put_msg(3, msg4);
    std::cout << msgqueue.get_msg() << std::endl;
    std::cout << msgqueue.get_msg() << std::endl;
    std::cout << msgqueue.get_msg() << std::endl;
    return 0;
}

int test_tw()
{
    for (int i = 0; i < 10; i++)
    {

        auto res = tw::make_task(
            tw::root, [&i]()
            { std::cout << "task"
                        << i << "-" << std::this_thread::get_id() << std::endl;return 1; });

        tw::parallel exec{4};
        res->schedule(exec);
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