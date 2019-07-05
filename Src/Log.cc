#include "Log.h"
namespace Log
{

uint32_t LogEvent::getLine() const{ return this->Line; }
uint64_t LogEvent::getTime() const{ return this->Time; }
uint64_t LogEvent::getElapse() const{ return this->Elapse; }
uint64_t LogEvent::getFiberId() const{ return this->FiberId; }
uint64_t LogEvent::getThreadId() const{ return this->ThreadId; }
const std::string& LogEvent::getFile() const{ return this->File; }
const std::string& LogEvent::getContext() const{ return this->Context; }

void ILogAppender::setFormatter(std::shared_ptr<LogFormatter> formatter){
    this->Formatter = formatter;
}

std::shared_ptr<LogFormatter> ILogAppender::getFormatter(){
    return this->Formatter;
}

Logger::Logger(const std::string &name) 
    :LogName(name){
}

void Logger::addILogAppender(std::shared_ptr<ILogAppender> logAppender){
    if (logAppender->getFormatter()){
        logAppender->setFormatter(this->Formatter);
    }
    this->ILogAppenders.push_back(logAppender);
}

void Logger::delILogAppender(std::shared_ptr<ILogAppender> logAppender){
    for (auto i = this->ILogAppenders.begin(); 
            i != this->ILogAppenders.end(); i++){
        if (*i == logAppender){
            this->ILogAppenders.erase(i);
        }
    }
}

void Logger::setFormatter(std::shared_ptr<LogFormatter> formatter){
    this->Formatter = formatter;
}
std::shared_ptr<LogFormatter> Logger::getFormatter(){
    return this->Formatter;
}

void Logger::log(LogLevel::Level level, std::shared_ptr<LogEvent> event){
    auto self = this->shared_from_this();
    for (auto &i : this->ILogAppenders){
        i->log(self, level, event);
    }
}
void Logger::debug(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::DEBUG, event);
}
void Logger::info(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::INFO, event);
}
void Logger::warn(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::WARN, event);
}
void Logger::error(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::ERROR, event);
}
void Logger::fatal(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::FATAL, event);
}

void LogFormatter::initFormat(const std::string &pattern){
    //cmd fmt type
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nomalString;
    std::string cmd;
    std::string fmt;
    uint32_t length = pattern.size();
    for (uint32_t i = 0; i < length; i++){
        cmd.clear();
        fmt.clear();
        if (pattern[i] != '%'){
            nomalString.append(1, pattern[i]);
            continue;
        }
        uint32_t n = i + 1;
        uint32_t Status = 0;
        uint32_t Index = 0;
        if (pattern[n] == '%' && n <= length){
            nomalString.append(1, '%');
            i = n;
            continue;
        }
        while (n < length){
            if (!Status && !std::isalpha(pattern[n]) 
                    && pattern[n] != '{' && pattern[n] != '}'){
                cmd = pattern.substr(i + 1, n - i - 1);
                break;
            }
            if (Status == 0){
                if (pattern[n] == '{'){
                    cmd = pattern.substr(i + 1, n - i - 1);
                    Status = 1;
                    Index = n;
                    n++;
                    continue;
                }
            }
            if (Status == 1){
                if (pattern[n] == '}'){
                    fmt = pattern.substr(Index + 1, n - Index - 1);
                    Status = 0;
                    n++;
                    break;
                }
            }
            n++;
            if (n == length){
                if (cmd.empty()){
                    cmd = pattern.substr(i + 1);
                }
            }
        }
        if (Status == 0){
            if (!nomalString.empty()){
                vec.push_back(std::make_tuple(nomalString, std::string(), 0));
                std::cout << "normalString:[" << nomalString.c_str() << "]" << std::endl;
                nomalString.clear();
            }
            vec.push_back(std::make_tuple(cmd, fmt, 1));
            std::cout << "cmd:[" << cmd.c_str() 
                << "]fmt:[" << fmt.c_str() << "]" << std::endl;
            i = n - 1;
        }
        else if (Status == 1){
            std::cout << "pattern error:" << pattern 
                << "-" << pattern.substr(i) << std::endl;
        }
    }
    if (!nomalString.empty()){
        vec.push_back(std::make_tuple(nomalString, std::string(), 0));
        std::cout << "normalString:[" << nomalString.c_str() 
            << "]" << std::endl;
        nomalString.clear();
    }
}

void StdOutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,std::shared_ptr<LogEvent> event){
    this->getFormatter()->format(std::cout, logger, level, event);
}
FileLogAppender::FileLogAppender(const std::string &path){
    this->reOpen();
}
void FileLogAppender::log(std::shared_ptr<Logger> logger, 
        LogLevel::Level level,
        std::shared_ptr<LogEvent> event){
    this->getFormatter()->format(this->OutPutFile, logger, level, event);
}

bool FileLogAppender::reOpen(){
    if (this->OutPutFile){
        OutPutFile.close();
    }
    this->OutPutFile.open(this->Path);
    return this->OutPutFile.is_open();
}

std::string LogFormatter::format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event){

}
//%m 消息体
//%p Level
//%r 启动时间
//%c log名称
//%t 线程id
//%n 回车换行
//%d 时间 
//%f 文件名
//%l 行号
void MessageFormatItem::format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event){
    os << event->getContext().c_str();    
}
void LevelFormatItem::format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event){
    os << LogLevel::toString().c_str();
}

} // namespace Log
