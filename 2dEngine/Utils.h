#pragma once

#include "globalDefs.h"
#include <stdint.h>
#include <stdio.h>

#define arrayCount(x) sizeof(x) / sizeof(x[0])

#define Kilobyte(x) x * 1024ll
#define Megabyte(x) Kilobyte(x) * 1024ll
#define Gigabyte(x) Megabyte(x) * 1024ll

static constexpr float PIf = 3.1415927f;

static constexpr float PiOver180 = PIf / 180;
static constexpr float _180OverPi = 180 / PIf;

namespace utils
{
	uint32_t getGUID();
	float lerp(float v0, float v1, float t);
	float degreesToRadians(float degree);
	float radiansToDegrees(float radians);
	template<typename... Args>
	void logF(const char* string, Args&&... args)
    {
#ifdef _DEBUG
       printf(string, args...);
#endif
    }
    template<typename... Args>
	void logFBreak(const char* string, Args&&... args)
    {
#ifdef _DEBUG
		printf(string, args...);
        InvalidCodePath;
#endif
    }
	inline void log(const char* string)
	{
#ifdef _DEBUG
		printf(string);
#endif
	}
	inline void logBreak(const char* string)
	{
#ifdef _DEBUG
		log((string));
		InvalidCodePath;
#endif
	}
}