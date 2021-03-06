//
// Created by lyc-fedora on 15-5-1.
//
//接受器内部使用accept接受新连接
//使用一个Socket对象保存监听socket
//使用一个分派器负责新连接到来事件


#ifndef SIMPLETON_ACCEPTOR_H
#define SIMPLETON_ACCEPTOR_H

#include "Socket.h"
#include "Dispatcher.h"
#include "Reactor.h"
#include "Callbacks.h"

using namespace std;

namespace simpleton
{
class Acceptor
{

public:
    //构造函数完成服务端的初始化工作
    //包括新建监听套接字、绑定地址等
    Acceptor(Reactor* reactor,EndPoint end);
    ~Acceptor();

    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;

    //设置为监听模式并向Reactor中注册可读事件
    //因为需要先设置回调再开始监听
    void Listen();

    //设置新连接到来时的回调
    void SetNewConnCallback(const AcceptorCallback& callback)
    {
        _acceptorCallback = callback;
    }

private:
    //有一个自己定义的处理新连接到来时的回调用于接受连接
    void handleRead();

    //用于reactor引用将自己的新连接事件添加进去
    Reactor* _reactor;
    //用于处理连接的监听套接字
    Socket _socket;
    //自己的事件分派器用于负责监听套接字的事件分发
    Dispatcher _dispatcher;
    //新连接到来时的回调
    AcceptorCallback _acceptorCallback;
    //是否处于监听模式
    bool _isListening;
};
}

#endif //SIMPLETON_ACCEPTOR_H
