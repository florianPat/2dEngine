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