#pragma once

#include <dsound.h>
#include <vector>
#include "Audio.h"

namespace eg
{
	class SoundSystem
	{
		friend class Window;
	private:
		LPDIRECTSOUNDBUFFER secondaryBuffer = nullptr;
		int bytesPerSample = sizeof(short) * 2;
		int samplesPerSecound;
		uint sampleIndex = 0;
		int bufferSize;

		std::vector<Audio*> audios;
		std::vector<std::vector<Audio*>::iterator> deleteList;
	private:
		SoundSystem() = default;
		SoundSystem(HWND windowHandle, int samlesPerSecound = 48000, int bufferSize = 48000 * sizeof(short) * 2);
	public:
		void play(Audio* audio);
		void update();
	};
}