#include "Clock.h"

namespace eg
{
	Clock::Clock(LONGLONG performceCounterFrequency) : performceCounterFrequency(performceCounterFrequency)
	{
		QueryPerformanceCounter(&start);
	}

	float Clock::getElapsedTime() const
	{
		LARGE_INTEGER end = getTime();
		float result = ((end.QuadPart - start.QuadPart) / (float)performceCounterFrequency);
		return result;
	}

	float Clock::restart()
	{
		float result = getElapsedTime();
		QueryPerformanceCounter(&start);
		return result;
	}

	LARGE_INTEGER Clock::getTime() const
	{
		LARGE_INTEGER result;
		QueryPerformanceCounter(&result);
		return result;
	}
}