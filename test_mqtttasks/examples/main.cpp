
#include "mqtttasks.h"
#include <transwarp.h>
#include "ThreadPool.hpp"
#include "Tester.hpp"

#include <fstream>
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>
using namespace std;
namespace tw = transwarp;

// 线程池测试
void test_mqtt_threadpool()
{
    ThreadPool pool(4);
    pool.enqueue(sub);
    pool.enqueue(pub);
}

// transwarp测试
void test_transwarp()
{
    // Building the task graph
    auto parent1 = tw::make_task(tw::root, sub)
                       ->named("sub");
    auto parent2 = tw::make_task(tw::root, pub)
                       ->named("pub");
    auto child = tw::make_task(
                     tw::consume, [](int a, int b)
                     { return a + b; },
                     parent1, parent2)
                     ->named("return");

    // child->add_listener_all(std::make_shared<tw::timer>());// 测试timer
    // child->add_listener_all(std::make_shared<tw::releaser>());// 测试releaser

    tw::parallel executor{4}; // 并行执行
    child->schedule_all(executor);

    tw::sequential executor2; // 串行执行
    // child->schedule_all(executor2);

    std::ofstream{"test_transwarp.dot"} << tw::to_string(child->edges());
}

// mqtt测试(lambda表达式)
namespace mqtt
{
    void test_mqtt_transwarp()
    {
        auto subtask = tw::make_task(
                           tw::root, []()
                           { struct mqtt_client client; 
                           const char *addr;
                            const char *port;
                            addr = "localhost";
                            port = "1234";
                            int sockfd = open_nb_socket(addr, port);
                        uint8_t sendbuf[2048];                        
                        uint8_t recvbuf[1024];   
                       mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), subscribe_callback);
                       uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
                       mqtt_connect(&client, NULL, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
                       pthread_t client_daemon;
                       if (pthread_create(&client_daemon, NULL, client_refresher, &client)) {
                           fprintf(stderr, "Error creating client daemon thread\n");
                       }
                       mqtt_subscribe(&client, "datetime", 0); 
                       sleep(10); 
                       return 0; })
                           ->named("sub");

        auto pubtask = tw::make_task(
                           tw::root, []()
                           {
            struct mqtt_client client;
            const char *addr;
            const char *port;
            addr = "localhost";
            port = "1234";
            int sockfd = open_nb_socket(addr, port);
            uint8_t sendbuf[2048];
            uint8_t recvbuf[1024];
            mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);
            uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
            mqtt_connect(&client, NULL, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
            pthread_t client_daemon;
            if (pthread_create(&client_daemon, NULL, client_refresher, &client))
            {
                fprintf(stderr, "Error creating client daemon thread\n");
            }
            for (int i = 0; i < 10; i++)
            {
                char buf[100];
                sprintf(buf, "temperature: %d", i);
                mqtt_publish(&client, "datetime", buf, strlen(buf) + 1, 1);
                sleep(1);
            }
                        return 0; })
                           ->named("pub");

        auto child = tw::make_task(
                         tw::consume, [](int a, int b)
                         { return a + b; },
                         subtask, pubtask)
                         ->named("return");

        tw::parallel executor{4};
        child->schedule_all(executor);
        std::cout << child->get() << std::endl;
        std::ofstream{"test_mqtt_transwarp.dot"} << tw::to_string(child->edges());
    }
}

int main()
{
    Tester tester("Test");

    tester.addTest(test_transwarp, "Test test_transwarp");
    // tester.addTest(test_mqtt_threadpool, "Test test_mqtt_threadpool");
    // tester.addTest(mqtt::test_mqtt_transwarp, "Test test_mqtt_transwarp");
    tester.runTests();
    return 0;
}