#pragma once

#include <Windows.h>

namespace eg
{
	class Clock
	{
		LARGE_INTEGER start;
		LONGLONG performceCounterFrequency;
	public:
		Clock(LONGLONG performceCounterFrequency);
		float getElapsedTime() const;
		float restart();
	private:
		LARGE_INTEGER getTime() const;
	};
}