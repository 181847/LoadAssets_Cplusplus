#pragma once
#include <exception>
class  SimpleException: std::exception
{
public:
	const char* expr;
	const char* file;
	const int line;

public:
	SimpleException(const char *e, const char* f, const int line)
		:expr(e), file(f), line(line)
	{
	}

	inline std::string SimpleException::ToString()
	{
		char lineString[256];
		std::string returnval;

		sprintf_s(lineString, "\nFile:\t\t%s\nLine:\t\t%d\nExperssion:\t%s", file, line, expr, 256);

		returnval += lineString;
		return returnval;
	}
};





#if ASSERTIONS_ENABLED
#define ASSERT(expr) \
	if(expr){}\
	else\
	{\
		throw SimpleException( #expr, __FILE__, __LINE__);\
	}
#else
#define ASSERT(expr)//不求值
#endif

//这个宏适用于一些代码没有实现的时候，抛出这个异常，防止忘记一些代码的实现
#define THROW_UNIMPLEMENT_EXCEPTION(pChar)\
	{\
		throw SimpleException(pChar, __FILE__, __LINE__);\
	}
