#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <intrin.h>

#define global_variable static
#define local_persist static

typedef int32_t bool32_t;
typedef uint32_t uint;
typedef uint64_t ulonglong;
typedef uint16_t ushort;
typedef uint8_t uchar;

#include <assert.h> //For assert!

#define InvalidCodePath assert(!"Invalid Code Path")

#define utilsLog(string) std::cerr << "Log: Function: " << __FUNCTION__ << ", Line: " << __LINE__ << ", Message: " << string << std::endl
#define utilsLogBreak(string) { utilsLog(string); __debugbreak(); }

#define arrayCount(x) sizeof(x) / sizeof(x[0])

#define Kilobyte(x) x * 1024ll
#define Megabyte(x) Kilobyte(x) * 1024ll
#define Gigabyte(x) Megabyte(x) * 1024ll

inline float lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}