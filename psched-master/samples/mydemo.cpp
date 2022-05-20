#include "psched.h"
#include <iostream>
using namespace psched;
int test() {
  PriorityScheduler<
      threads<3>, queues<3, maintain_size<100, discard::oldest_task>>,
      aging_policy<task_starvation_after<std::chrono::milliseconds, 250>,
                   increment_priority_by<1>>>
      scheduler;

  Task a(
      // Task action
      [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        std::cout << std::this_thread::get_id() << ": a" << std::endl;
      },
      // Task post-completion callback
      [](const TaskStats &stats) {
        std::cout << "[Task a] ";
        std::cout << "Waiting time = " << stats.waiting_time() << "ms; ";
        std::cout << "Burst time = " << stats.burst_time() << "ms; ";
        std::cout << "Turnaround time = " << stats.turnaround_time() << "ms\n";
      });

  Task b(
      // Task action
      [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(390));
        std::cout << std::this_thread::get_id() << ": b" << std::endl;
      });
  scheduler.schedule<priority<0>>(a);
  scheduler.schedule<priority<1>>(b);
  scheduler.schedule<priority<2>>(b);
  return 0;
}

int function() {
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::cout << std::this_thread::get_id() << std::endl;
  return 0;
}

class mytask {
  static const int threadnum = 4;
  PriorityScheduler<
      threads<threadnum>, queues<3, maintain_size<100, discard::oldest_task>>,
      aging_policy<task_starvation_after<std::chrono::milliseconds, 250>,
                   increment_priority_by<1>>>
      scheduler;

public:
  mytask() {}
  ~mytask() {}
  template <class F, class... Args> void add_task(F &&f, Args &&...args) {
    Task b(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    scheduler.schedule<priority<0>>(b);
  }
};

int main() {
  //   task(function);
  mytask t;
  //   t.add_task(function);
  for (int i = 0; i < 10; i++) {
    t.add_task(function);
  }
  return 0;
}