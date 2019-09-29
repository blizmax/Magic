#pragma once
#include <assert.h>
#include "Log.h"
#include "Util.h"
#include "Thread.h"
#include "Adapter/Adapter.h"

#define  MAGIC_LOG_LEVEL(logger,level) \
                if(level >= logger->getLevel()) \
                    Magic::LogWrap(logger,level,MagicPtr<Magic::LogEvent>(new Magic::LogEvent(__LINE__,static_cast<uint64_t>(time(nullptr)),1,Magic::GetFiberId(),Magic::GetThreadId(),__FILE__,logger->getLogName(),Magic::Thread::GetName()))).get()
                
#define MAGIC_LOG_DEBUG(logger)       MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogDebug)

#define MAGIC_LOG_INFO(logger)        MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogInfo)

#define MAGIC_LOG_WARN(logger)        MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogWarn)

#define MAGIC_LOG_ERROR(logger)       MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogError)

#define MAGIC_LOG_FATAL(logger)       MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogFatal)

#define MAGIC_LOG_ROOT()              Magic::LoggerMgr::GetInstance()->getRoot()

#define MAGIC_LOG_NAME(Name)          Magic::LoggerMgr::GetInstance()->getLogger(Name)

#define MAGIC_ASSERT(exp,msg) \
	if(!(exp)) { \
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "ASSERTION: " << #exp \
			<< "\n=>" << msg \
			<< "\nBackTrace:\n" \
			<< Magic::BackTraceToString(); \
		assert(exp); \
	}