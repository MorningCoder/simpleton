//
// Created by lyc-fedora on 15-4-22.
//
//线程池类

#ifndef SIMPLETON_THREADPOOL_H
#define SIMPLETON_THREADPOOL_H

#include <vector>
#include <mutex>
#include <functional>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <deque>

using namespace std;

namespace  simpleton
{

//用于等待指定线程集线程结束
//仅供线程池使用
class ThreadJoiner
{
public:
    //构造时必须指定要等待的线程集合
    explicit ThreadJoiner(vector<thread>& threadSet)
    :_threads(threadSet)
    { }

    //不可拷贝或移动
    ThreadJoiner(const ThreadJoiner&) = delete;
    ThreadJoiner& operator=(const ThreadJoiner&) = delete;
    ThreadJoiner(ThreadJoiner&&) = delete;
    ThreadJoiner& operator=(ThreadJoiner&&) = delete;

    //析构时等待所有线程结束
    ~ThreadJoiner()
    {
        for(auto& th : _threads)
        {
            if(th.joinable())
                th.join();
        }
    }
private:
    //一个vector引用！
    //用于引用指定的线程集合对象并且在析构时join该对象内部所有线程
    vector<thread> _threads;
};

class ThreadPool
{
public:

    ThreadPool(unsigned int max = thread::hardware_concurrency());

    //不可拷贝或移动
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool()
    {
        _isDone = true;
    }

    //提交一个函数对象在线程池中运行

    //默认情况下会阻塞调用线程
    void Submit(const function<void()>&);
    void Submit(function<void()>&& );

    //重载函数：如果需要阻塞则返回-1
    bool TrySubmit(const function<void()>&);
    bool TrySubmit(function<void()>&& );

private:
    //本进程池运行的工作线程函数
    void workerThread();

    //线程的等待器
    ThreadJoiner _joiner;
    //条件变量和互斥锁
    condition_variable _cond;
    mutex _mtx;
    //最大线程数目
    //默认为hardware_currency()返回值
    unsigned int _maxNum;
    //是否关闭池标志
    atomic<bool> _isDone;
    //存储线程对象
    vector<thread> _threads;
    //存储任务队列
    deque<function<void()>> _taskQueue;
};
}

#endif //SIMPLETON_THREADPOOL_H
