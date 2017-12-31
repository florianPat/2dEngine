//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

#include "Window.h"
#include "Graphics2d.h"
#include "Color.h"

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();

	eg::RectF rect(-10, -40, 20, 50);
	eg::FloatCircle circle(850, 540, 100);
	eg::Texture texture("dib.bmp");
	eg::Sprite sprite(texture);
	eg::ORectF oRect(50, 50, 100, 100, 45, eg::Vec2f{ 0.0f, 0.0f });

	sprite.pos.x = -10.0f;
	sprite.pos.y = 400.0f;

	while (window.isOpen())
	{
		window.processMessages();

		oRect.setAngle(oRect.getAngle() + 5.0f);

		gfx.clear();

		gfx.draw(rect);
		gfx.draw(circle);
		gfx.draw(sprite);
		gfx.draw(oRect);

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
