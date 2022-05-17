#include "tasks.h"

void Tasks::set_stop_flag(bool flag)
{
    stop_flag = flag;
}

void Tasks::set_pool(ThreadPool *pool)
{
    this->pool = pool;
}

void Tasks::set_queue(Queue *queue)
{
    this->queue = queue;
}

void Tasks::set_thread_num(int num)
{
    thread_num = num;
}

void Tasks::set_task_num(int num)
{
    task_num = num;
}

void Tasks::set_task_time(int time)
{
    task_time = time;
}

void Tasks::set_task_interval(int interval)
{
    task_interval = interval;
}

void Tasks::set_task_type(int type)
{
    task_type = type;
}

void Tasks::set_task_msg(Msg *msg)
{
    task_msg = msg;
}

void Tasks::set_task_msg_type(int type)
{
    task_msg_type = type;
}

void Tasks::set_task_msg_content(char *content)
{
    task_msg_content = content;
}

void Tasks::set_task_msg_content_len(int len)
{
    task_msg_content_len = len;
}

void Tasks::set_task_msg_qos(int qos)
{
    task_msg_qos = qos;
}

void Tasks::set_task_msg_retain(bool retain)
{
    task_msg_retain = retain;
}

void Tasks::set_task_msg_topic(char *topic)
{
    task_msg_topic = topic;
}

void Tasks::set_task_msg_topic_len(int len)
{
    task_msg_topic_len = len;
}

void Tasks::set_task_msg_payload(char *payload)
{
    task_msg_payload = payload;
}

int Tasks::get_thread_num()
{
    return thread_num;
}

int Tasks::get_task_num()
{
    return task_num;
}