#pragma once
#include "Msg.hpp"
#include <memory>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <map>
#include <mutex>
#include <utility>
/**
 * Queue is a thread-safe message queue.
 * 线程安全的消息队列
 * It supports one-way messaging and request-response pattern.
 * 支持单向消息和请求-回复模式
 */
class Queue
{
public:
    Queue();

    ~Queue();

    /**
     * Put Msg to the end of the queue.
     *
     * @param msg Msg to put to the queue.
     */
    void put(Msg &&msg);

    /**
     * Get message from the head of the queue.
     * Blocks until at least one message is available in the queue, or until timeout happens.
     * If get() returns due to timeout, returns a nullptr.
     *
     * @param timeoutMillis How many ms to wait for message until timeout happens.
     *                      0 = wait indefinitely.
     */
    std::unique_ptr<Msg> get(int timeoutMillis = 0);

    /**
     * Get message from the head of the queue.
     * Returns an empty pointer if no message is available.
     */
    std::unique_ptr<Msg> tryGet();

    /**
     * Make a request.
     * Call will block until response is given with respondTo().
     * If request() returns due to timeout, returns a nullptr.
     *
     * @param msg Request message. Is put to the queue so it can be retrieved from it with get().
     * @param timeoutMillis How many ms to wait for response until timeout happens.
     *                      0 = wait indefinitely.
     */
    std::unique_ptr<Msg> request(Msg &&msg, int timeoutMillis = 0);

    /**
     * Respond to a request previously made with request().
     * If the requestID has been found, return true.
     *
     * @param reqUid Msg UID of the request message.
     * @param responseMsg Response message. The requester will receive it as the return value of
     *                    request().
     */
    bool respondTo(MsgUID reqUid, Msg &&responseMsg);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

class Queue::Impl
{
    struct Request
    {
        Request(){};

        std::unique_ptr<Msg> response;
        std::condition_variable condVar;
    };

public:
    Impl()
        : queue_(), queueMutex_(), queueCond_(), responseMap_(), responseMapMutex_()
    {
    }

    void put(Msg &&msg)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            queue_.push(msg.move());
        }

        queueCond_.notify_one();
    }

    std::unique_ptr<Msg> get(int timeoutMillis)
    {
        std::unique_lock<std::mutex> lock(queueMutex_);

        if (timeoutMillis <= 0)
            queueCond_.wait(lock, [this]
                            { return !queue_.empty(); });
        else
        {
            // wait_for returns false if the return is due to timeout
            auto timeoutOccured = !queueCond_.wait_for(
                lock,
                std::chrono::milliseconds(timeoutMillis),
                [this]
                { return !queue_.empty(); });

            if (timeoutOccured)
                return nullptr;
        }

        auto msg = queue_.front()->move();
        queue_.pop();
        return msg;
    }

    std::unique_ptr<Msg> tryGet()
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        if (!queue_.empty())
        {
            auto msg = queue_.front()->move();
            queue_.pop();
            return msg;
        }
        else
        {
            return {nullptr};
        }
    }

    std::unique_ptr<Msg> request(Msg &&msg, int timeoutMillis)
    {
        Request req;

        // emplace the request in the map
        std::unique_lock<std::mutex> lock(responseMapMutex_);
        auto it = responseMap_.emplace(
                                  std::make_pair(msg.getUniqueId(), &req))
                      .first;

        put(std::move(msg));

        if (timeoutMillis <= 0)
            req.condVar.wait(lock, [&req]
                             { return req.response.get(); });
        else
        {
            // wait_for returns false if the return is due to timeout
            auto timeoutOccured = !req.condVar.wait_for(
                lock,
                std::chrono::milliseconds(timeoutMillis),
                [&req]
                { return req.response.get(); });

            if (timeoutOccured)
            {
                responseMap_.erase(it);
                return nullptr;
            }
        }

        auto response = std::move(it->second->response);
        responseMap_.erase(it);

        return response;
    }

    bool respondTo(MsgUID reqUid, Msg &&responseMsg)
    {
        std::unique_lock<std::mutex> lock(responseMapMutex_);
        auto it = responseMap_.find(reqUid);
        if (it == responseMap_.end())
            return false;

        it->second->response = responseMsg.move();
        it->second->condVar.notify_one();
        return true;
    }

private:
    // Queue for the Msgs
    std::queue<std::unique_ptr<Msg>> queue_;

    // Mutex to protect access to the queue
    std::mutex queueMutex_;

    // Condition variable to wait for when getting Msgs from the queue
    std::condition_variable queueCond_;

    // Map to keep track of which request IDs are associated with which request Msgs
    std::map<MsgUID, Request *> responseMap_;

    // Mutex to protect access to response map
    std::mutex responseMapMutex_;
};

Queue::Queue()
    : impl_(new Impl)
{
}

Queue::~Queue()
{
}

void Queue::put(Msg &&msg)
{
    impl_->put(std::move(msg));
}

std::unique_ptr<Msg> Queue::get(int timeoutMillis)
{
    return impl_->get(timeoutMillis);
}

std::unique_ptr<Msg> Queue::tryGet()
{
    return impl_->tryGet();
}

std::unique_ptr<Msg> Queue::request(Msg &&msg, int timeoutMillis)
{
    return impl_->request(std::move(msg), timeoutMillis);
}

bool Queue::respondTo(MsgUID reqUid, Msg &&responseMsg)
{
    return impl_->respondTo(reqUid, std::move(responseMsg));
}
