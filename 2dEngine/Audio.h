#pragma once

#include "Sound.h"

namespace eg
{
	class Audio
	{
		friend class SoundSystem;
	public:
		enum class PlayDur
		{
			ONCE = 1,
			LOOPED = 2
		};
	private:
		const Sound& sound;
		long long playPos = 0;
		const std::vector<std::vector<short>>& samples;
		float volume = 1.0f;
		const int nSamples;
	public:
		PlayDur playDur;
	public:
		Audio(const Sound& sound, PlayDur playDur);
		void setVolume(float volume);
		float getVolume() const;
	};
}