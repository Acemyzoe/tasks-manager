#ifndef TRANSWARP_MINIMUM_TASK_SIZE
#define TRANSWARP_MINIMUM_TASK_SIZE
#endif
#include <transwarp.h>
#include <iostream>

namespace tw = transwarp;

int main()
{
    auto task1 = tw::make_task(tw::root, []
                               { return 0; });
    std::cout << "task impl size = " << sizeof(*task1) << std::endl;
    auto task2 = tw::make_value_task(1);
    std::cout << "value task size = " << sizeof(*task2) << std::endl;
    task1->schedule_all();
    task2->schedule_all();
    std::cout << "task1 = " << task1->get() << std::endl;
    std::cout << "task2 = " << task2->get() << std::endl;
}
