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
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();
	eg::SoundSystem soundSystem = window.getSndSys();

	eg::Vector2i p0 = { 100, 100 };

	while (window.processEvents())
	{
		gfx.clear();

		gfx.drawLine(p0, window.getMouse().pos, Colors::White);

		eg::Vector2i p2 = { -100, -100 };

		if (gfx.clipLine(p2, p0))
			gfx.drawLine(p2, p0, Colors::Cyan);

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
