/*
 * @File: Timer.h
 * @Author: INotFound
 * @Date: 2020-03-13 21:33:59
 * @LastEditTime: 2020-03-14 00:14:28
 */
#pragma once
#include <functional>
#include "Core.h"
#include "Thread.h"
namespace Magic{
    /**
     * @brief: 定时器类
     */
    class Timer{
    public:
        /**
         * @brief: 构造函数
         * @param name 定时器名称
         * @param tickMs 定时器执行时间(毫秒)
         * @param callBack 定时器回调执行函数
         */
        Timer(const std::string& name,uint32_t tickMs,const std::function<void()>& callBack);
        /**
         * @brief: 运行定时器
         */
        void run();
        /**
         * @brief: 暂停定时器
         */
        void stop();
    private:
        /**
         * @brief: 定时器处理函数
         * @param err 错误码
         */
        void handle(const asio::error_code& err);
    private:
        std::string m_Name;
        Safe<Thread> m_Thread;
        uint32_t m_MilliSeconds;
        Safe<asio::io_context> m_Io;
        Safe<asio::steady_timer> m_Time;
        std::function<void()> m_CallBack;
        Safe<asio::io_context::work> m_IoWork;
    };
}