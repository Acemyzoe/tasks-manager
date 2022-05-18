#include "tasks.h"
#include <iostream>
#include <time.h>
namespace tw = transwarp;
int main()
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