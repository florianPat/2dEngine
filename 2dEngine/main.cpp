//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

#include "Window.h"
#include "Graphics2d.h"
#include "Color.h"
#include "Sound.h"

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();
	eg::Sound snd = window.getSnd();

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();

		gfx.render();

		snd.outputTest();

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
