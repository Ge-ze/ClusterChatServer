#ifndef REDIS_H
#define REDIS_H

#include<hiredis/hiredis.h>
#include<thread>
#include<functional>
using namespace std;

class Redis
{ 
public:
    Redis();
    ~Redis();
    // 连接redis服务器
    bool connect();

    //向指定channel通道发布消息
    bool publish(int channel,string message);

    //subscribe订阅channel消息
    bool subscribe(int channel);

    //取消channel订阅消息
    bool unsubscribe(int channel);

    //在独立线程中接受订阅通道中的消息
    void observer_channel_message();

    //初始化向业务层上报通道消息的回调对象
    void init_notify_handler(function<void(int,string)> func);

private:
    //同步上下文对象，负责publish消息
    redisContext *_publish_context;

    //同步上下文对象，负责subcribe消息
    redisContext *_subcribe_context;

    //回调操作，收到订阅的消息，给services层上报
    function<void(int,string)> _notify_message_handler;
}; 

#endif