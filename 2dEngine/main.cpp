//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

#include "Window.h"
#include "Graphics2d.h"
#include "Color.h"
#include "SoundSystem.h"
#include "Sound.h"
#include "Audio.h"

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();
	eg::SoundSystem soundSystem = window.getSndSys();

	eg::Sound sound("AllStars.wav");
	eg::Audio audio(sound, eg::Audio::PlayDur::LOOPED);
	soundSystem.play(&audio);

	float seconds = 0.0f;

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();

		seconds += 1 / 500.0f;
		float volume = lerp(0.0f, 1.0f, seconds);
		if (volume > 1.0f)
			volume = 1.0f;
		audio.setVolume(volume);

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}
	/*
	eg::Game game(window, "LevelName.tmx");

	{
	while(window.isOpen())
	{
	eg::Level currentLevel(levelName);

	std::string nextLevel = currentLevel.Go();

	while(nextLevel != "")
	std::string nextLevel = currentLevel.Go();
	else
	{
	window.close();
	break;
	}
	}
	}
	*/

	return 0;
}
