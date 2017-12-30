//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

#include "Window.h"
#include "graphics2d.h"

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();

	eg::RectF rect(200, 200, 400, 300);
	eg::FloatCircle circle(700, 400, 50);

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();

		gfx.putPixel(100, 100, 255, 255, 255);
		gfx.draw(rect);
		gfx.draw(circle);

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
