#pragma once

#include <dsound.h>

namespace eg
{
	class Sound
	{
		friend class Window;
	private:
		LPDIRECTSOUNDBUFFER secondaryBuffer = nullptr;
		int bytesPerSample = sizeof(short) * 2;
		int samplesPerSecound;
		uint sampleIndex = 0;
		int hz = 256;
		int wavePeriod;
		int bufferSize;
		short volume = 3000;
	private:
		Sound() = default;
		Sound(HWND windowHandle, int samlesPerSecound = 48000, int bufferSize = 48000 * sizeof(short) * 2);
	public:
		void outputTest();
	};
}