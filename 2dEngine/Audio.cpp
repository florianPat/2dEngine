#include "Audio.h"

eg::Audio::Audio(const Sound & sound, PlayDur playDur) : sound(sound), playDur(playDur), samples(sound.getSamples()), nSamples(sound.getNSamples())
{
}

void eg::Audio::setVolume(float volume)
{
	if (volume > 1.0f || volume < 0.0f)
	{
		utilsLogBreak("Volume needs to be between 0 and 1!");
	}
	else
		this->volume = volume;
}

float eg::Audio::getVolume() const
{
	return volume;
}
