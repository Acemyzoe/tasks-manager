/*
 * Copyright (C) 2011-2022 sgcc Inc.
 * All right reserved.
 * 文件名称：Singleton.h
 * 摘    要：单例模板类
 * 作者：GuoJun  完成日期：2022/05/13
 */

//单例模板类
//使用一个代理类 token，子类构造函数需要传递token类才能构造
// example:
// class DerivedSingle : public Singleton<DerivedSingle>
// { DerivedSingle(token){}
//   ~DerivedSingle(){} };
template <typename T>
class Singleton
{
public:
    static T &instance();

    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton) = delete;

protected:
    struct token
    {
    };
    Singleton() {}
};

#include <memory>
template <typename T>
T &Singleton<T>::instance()
{
    static const std::unique_ptr<T> instance{new T{token{}}};
    return *instance;
}