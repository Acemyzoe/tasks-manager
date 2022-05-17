#pragma once
#ifndef TAKS_H
#define TAKS_H

#include "ThreadPool.hpp"
#include "Queue.hpp"
#include "Msg.hpp"
#include "transwarp.h"
using namespace std;
namespace tw = transwarp;

// tasks class
class Tasks
{
public:
    Tasks(ThreadPool *pool, Queue *queue);
    ~Tasks();
    void set_stop_flag(bool flag);
    bool get_stop_flag();
    void set_pool(ThreadPool *pool);
    void set_queue(Queue *queue);
    void set_thread_num(int num);
    void set_task_num(int num);
    void set_task_time(int time);
    void set_task_interval(int interval);
    void set_task_type(int type);
    void set_task_msg(Msg *msg);
    void set_task_msg_type(int type);
    void set_task_msg_content(char *content);
    void set_task_msg_content_len(int len);
    void set_task_msg_qos(int qos);
    void set_task_msg_retain(bool retain);
    void set_task_msg_topic(char *topic);
    void set_task_msg_topic_len(int len);
    void set_task_msg_payload(char *payload);

    int get_thread_num();
    int get_task_num();
    int get_task_time();
    int get_task_interval();
    int get_task_type();
    Msg *get_task_msg();
    int get_task_msg_type();
    char *get_task_msg_content();
    int get_task_msg_content_len();
    int get_task_msg_qos();
    bool get_task_msg_retain();
    char *get_task_msg_topic();
    int get_task_msg_topic_len();
    char *get_task_msg_payload();

private:
    ThreadPool *pool;
    Queue *queue;
    bool stop_flag;
    int thread_num;
    int task_num;
    int task_time;
    int task_interval;
    int task_type;
    Msg *task_msg;
    int task_msg_type;
    char *task_msg_content;
    int task_msg_content_len;
    int task_msg_qos;
    bool task_msg_retain;
    char *task_msg_topic;
    int task_msg_topic_len;
    char *task_msg_payload;
};

#endif