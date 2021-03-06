/*
 * @File: WebSocketServlet.h
 * @Author: INotFound
 * @Date: 2020-03-16 00:01:06
 * @LastEditTime: 2020-04-06 19:53:14
 */
#pragma once
#include <unordered_map>
#include "WebSocketMessage.h"

#include "../Core.h"
#include "../Mutex.h"

namespace Magic{
namespace Http{
    class WebSocketServlet{
    public:
        explicit WebSocketServlet(const std::string& name);
        const std::string getName() const;
        virtual void onOpen() =0;
        virtual void onClose() =0;
        virtual void onError() =0;
        virtual void onMessage(const Safe<WebSocketMessage>& msg) =0;
    private:
        std::string m_Name;
    };

    class WebSocketServletDispatch{
        typedef RWMutex::WriteLock RWMutexWriteLock;
    public:
        WebSocketServletDispatch();
        const Safe<WebSocketServlet>& getServlet(const std::string& path);
        void addWebSocketServlet(const std::string& path,Safe<WebSocketServlet>& servlet);
    private:
        RWMutex m_Mutex;
        std::unordered_map<std::string,Safe<WebSocketServlet>> m_Servlets;
    };
}
}