#pragma once
#include <memory>
#include <string>
#include <cassert>

#include "Util.h"

#define Ptr std::unique_ptr

//Not Security Check
#ifndef NSC

#include "../Log/Log.h"
#define MAGIC_ASSERT(exp,msg) \
	if(!(exp)) { \
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "ASSERTION: " << #exp \
			<< "\n" << msg \
			<< "\nBackTrace:\n" \
			<< Magic::BackTraceToString(); \
		assert(exp); \
	}

#define IsPointer(Var) MAGIC_ASSERT(Var != nullptr,"\033[31;4mPointer Name: " << #Var << " => nullPtr \033[0m")

#else

#define IsPointer(Var)

#endif