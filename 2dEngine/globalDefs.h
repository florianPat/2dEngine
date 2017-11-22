#pragma once

#include <stdint.h>

#define global_variable static
#define local_persist static

#define bool32_t int32_t

#include <assert.h> //For assert!

#define InvalidCodePath assert(!"Invalid Code Path")