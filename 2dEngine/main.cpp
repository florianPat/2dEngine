//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

//NOTE: Headers
#include "Window.h"
#include "graphics2d.h"

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();

		gfx.putPixel(100, 100, 255, 255, 255);

		gfx.drawRect(50, 50, 150, 150, 255, 255, 255);

		gfx.render();

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
