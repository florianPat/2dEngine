//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

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

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
