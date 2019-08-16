#pragma once
#include <map>
#include <string>
#include <memory> 
#include "../Thread/Mutex.h"
#include "../Util/Singleton.h"
namespace Magic{
class Logger;

class LoggerManager{
public:
    LoggerManager();
    std::unique_ptr<Logger>& getRoot();
    std::unique_ptr<Logger>& getLogger(const std::string& name);
private:
    Mutex m_Mutex;
    std::unique_ptr<Logger> m_Root;
    std::map<std::string,std::unique_ptr<Logger>> m_Loggers;
};

typedef Singleton<LoggerManager>        LoggerMgr;

}

#define MAGIC_LOG_ROOT()                    Magic::LoggerMgr::GetInstance()->getRoot()

#define MAGIC_LOG_NAME(Name)        Magic::LoggerMgr::GetInstance()->getLogger(Name)
