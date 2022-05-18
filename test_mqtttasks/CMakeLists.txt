
cmake_minimum_required(VERSION 3.5)

project(MAIN VERSION 1.0 LANGUAGES CXX C)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include_directories(${PROJECT_SOURCE_DIR}/include)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11 c_std_99)

# build options
option(EXAMPLES "Build examples?" ON)
option(TESTS "Build tests?" ON)

# TRANSWARP
add_compile_definitions(TRANSWARP_CPP11) 
install(FILES include/transwarp.h DESTINATION include)

# MQTT-C library
add_library(mqttc STATIC
    src/mqtt_pal.c
    src/mqtt.c
)
target_include_directories(mqttc PUBLIC include)
target_link_libraries(mqttc PUBLIC 
    $<$<C_COMPILER_ID:MSVC>:ws2_32>
)

# Build examples
if(EXAMPLES)
    find_package(Threads REQUIRED)
    add_executable(test_tw examples/test_tw.cpp examples/mqtttasks.c)
    target_link_libraries(test_tw Threads::Threads mqttc)

    add_executable(basic_with_three_tasks
        examples/basic_with_three_tasks.cpp)
    target_link_libraries(basic_with_three_tasks Threads::Threads)

    add_executable(test_threadpool examples/test_threadpool.cpp)
    target_link_libraries(test_threadpool Threads::Threads)

    add_executable(test_queue examples/test_queue.cpp)
    target_link_libraries(test_queue Threads::Threads)

    add_executable(demo examples/demo.cpp examples/mqtttasks.c)
    target_link_libraries(demo Threads::Threads mqttc)

endif()

if(TESTS)
    enable_testing()
    add_test(test_tw test_tw)
    add_test(test_transwarp basic_with_three_tasks)
    add_test(test_threadpool test_threadpool)
    add_test(test_queue test_queue)
endif()