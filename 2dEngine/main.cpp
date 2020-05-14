#include "Window.h"
#include "Graphics2d.h"
#include "Color.h"
#include "SoundSystem.h"
#include "Sound.h"
#include "Audio.h"
#include "Graphics3d.h"
#include <random>

int main()
{
	const float dt = 1 / 60.0f;
	eg::Window window(900, 600, "Title", dt);
	eg::Graphics2d& gfx = window.getGfx();
	eg::SoundSystem soundSystem = window.getSndSys();

	while (window.processEvents())
	{
		gfx.clear();

		gfx.drawTriangle({ 100, 50 }, { 50, 150 }, { 150, 250 }, Colors::Yellow);

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
