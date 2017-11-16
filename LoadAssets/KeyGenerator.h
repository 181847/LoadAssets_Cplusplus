#pragma once
#include <stdarg.h>
#include <stdio.h>
template<int BufferSize>
class KeyGenerator
{
public:
	KeyGenerator();
	~KeyGenerator();

	const char * __cdecl KeyGenerator<BufferSize>::format(const char * fmt, ...);
	const char * format(const char * string);
	const char * format(int i);

	const char * lastResult();

private:
	char  buffer[BufferSize];
};



template<int BufferSize>
KeyGenerator<BufferSize>::KeyGenerator()
{
}

template<int BufferSize>
KeyGenerator<BufferSize>::~KeyGenerator()
{
}

template<int BufferSize>
const char * KeyGenerator<BufferSize>::format(const char * string)
{
	sprintf_s(buffer, BufferSize, "%s", string);
	return buffer;
}

template<int BufferSize>
const char * KeyGenerator<BufferSize>::format(int i)
{
	sprintf_s(buffer, BufferSize, "%d", i);
	return buffer;
}

template<int BufferSize>
const char * KeyGenerator<BufferSize>::lastResult()
{
	return buffer;
}

template<int BufferSize>
const char * __cdecl KeyGenerator<BufferSize>::format(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, BufferSize, fmt, args);
	va_end(args);


	return buffer;
}