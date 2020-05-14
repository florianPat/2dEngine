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

	std::vector<eg::Vector2i> polygonStart = { {-150, 150}, {0, 50}, {150, 150}, {50, 0}, {150, -150}, {0, -50}, {-150, -150},
		{-50, 0} };
	float increment = 3.1415f / 6;
	float angle = increment;
	float scale = 1.0f;

	while (window.processEvents())
	{
		gfx.clear();

		auto polygon = polygonStart;

		for (auto it = polygon.begin(); it != polygon.end(); ++it)
		{
			float x = it->x * scale;
			float y = it->y * scale;
			float tempX = (x * cosf(angle) + y * -sinf(angle));
			y = (x * sinf(angle) + y * cosf(angle));
			x = tempX;
			x += gfx.width / 2;
			y *= -1.0f;
			y += gfx.height / 2;
			it->x = (int32_t)x;
			it->y = (int32_t)y;
		}

		gfx.drawPolyline(polygon, Colors::Yellow);

		angle += increment * dt;
		scale = sinf(angle);

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
