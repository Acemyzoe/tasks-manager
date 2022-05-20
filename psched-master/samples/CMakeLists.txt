add_executable(multiple_periodic_tasks multiple_periodic_tasks.cpp)
target_link_libraries(multiple_periodic_tasks PRIVATE psched::psched)

add_executable(mydemo mydemo.cpp)
target_link_libraries(mydemo PRIVATE psched::psched)