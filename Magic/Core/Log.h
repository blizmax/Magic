#pragma once
#include <map>
#include <list>
#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "Core.h"
#include "Util.h"
#include "Mutex.h"



namespace Magic{

enum class LogLevel{
    LogDebug   =1,
    LogInfo    =2,
    LogWarn    =3,
    LogError   =4,
    LogFatal   =5
};
const char* ToString(const LogLevel level);

class LogEvent{
public:
    LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t fiberId,
    uint64_t threadId,const std::string& file,const std::string& LogName,const std::string& threadName);
    uint32_t getLine()                  const;
    uint64_t getTime()                  const;
    uint64_t getElapse()                const;
    uint64_t getFiberId()               const;
    uint64_t getThreadId()              const;
    const std::string  getContent()     const;
    const std::string& getFile()        const;
    const std::string& getLogName()     const;
    const std::string& getThreadName()  const;
    std::stringstream& getStream();
private:
    uint32_t m_Line     =0;
    uint64_t m_Time     =0;
    uint64_t m_Elapse   =0;
    uint64_t m_FiberId  =0;
    uint64_t m_ThreadId =0;
    std::string m_File;
    std::string m_LogName;
    std::string m_ThreadName;
    std::stringstream m_StringStream;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem();
    virtual void format(std::ostream &os,const LogLevel  level,const MagicPtr<LogEvent>& event) =0;
};

class LogFormatter{
public:
    explicit LogFormatter(const std::string& pattern);
    void format(std::ostream &os,const LogLevel  level,const MagicPtr<LogEvent>& event);
private:
    std::vector<MagicPtr<ILogFormatItem>> m_Items;
};


class ILogAppender{
    friend class Logger;
public:
    virtual ~ILogAppender();
    virtual void log(LogLevel level,MagicPtr<LogEvent>& event) =0;
protected:
    MagicPtr<LogFormatter> m_Formatter;
};

class StdOutLogAppender :public ILogAppender{
public:
    void log(LogLevel level,MagicPtr<LogEvent>& event) override;
};

class FileLogAppender :public ILogAppender{
public:
    explicit FileLogAppender(const std::string &path);
    void log(LogLevel level,MagicPtr<LogEvent>& event) override;
    bool reOpen();
private:
	MutexType m_Mutex;
    std::string m_Path;
    std::ofstream m_FileStream;
};

class Logger {
    friend class LoggerManager;
public:
    explicit Logger(const std::string &name = "root");
    void addILogAppender(MagicPtr<ILogAppender>& logAppender);
    void delILogAppender(MagicPtr<ILogAppender>& logAppender);
    void setFormatter(const std::string &pattern);
    void setLevel(LogLevel);
    LogLevel getLevel() const;
    const std::string& getLogName() const;
    void log(LogLevel level,MagicPtr<LogEvent>& event);
private:
	MutexType m_Mutex;
    std::string m_LogName;
    std::string m_Formatter;
	LogLevel m_Level;
	std::list<MagicPtr<ILogAppender>> m_ILogAppenders;
};

class LoggerManager{
public:
    LoggerManager();
    MagicPtr<Logger>& getRoot();
    MagicPtr<Logger>& getLogger(const std::string& name);
private:
    MagicPtr<Logger> m_Root;
    std::map<std::string,MagicPtr<Logger>> m_Loggers;
};
typedef Singleton<LoggerManager>        LoggerMgr;

class LogWrap{
public:
    LogWrap(MagicPtr<Logger>& logger,const LogLevel level,MagicPtr<LogEvent>&& event);
    std::stringstream& get();
    ~LogWrap();
private:
    MagicPtr<Logger>& m_Logger;
    LogLevel m_Level;
    MagicPtr<LogEvent> m_Event;
};


}