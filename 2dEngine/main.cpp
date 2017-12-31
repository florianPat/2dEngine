//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

#include "Window.h"
#include "Graphics2d.h"

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();

	eg::RectF rect(-10, -40, 20, 50);
	eg::FloatCircle circle(850, 540, 100);
	eg::Texture texture("dib.bmp");
	eg::Sprite sprite(texture);

	sprite.pos.x = -10.0f;
	sprite.pos.y = 400.0f;

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();

		gfx.draw(rect);
		gfx.draw(circle);
		gfx.draw(sprite);

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
