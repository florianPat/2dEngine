#include "Window.h"
#include "Graphics2d.h"
#include "Color.h"
#include "SoundSystem.h"
#include "Sound.h"
#include "Audio.h"
#include "Graphics3d.h"
#include "PlyLoader.h"

int main()
{
	const float dt = 1 / 60.0f;
	eg::Window window(900, 600, "Title", dt);
	eg::Graphics2d& gfx = window.getGfx();
	eg::SoundSystem soundSystem = window.getSndSys();

	eg::Object object = eg::PlyLoader::load("cube.ply", { 0.0f, 0.0f, 0.0f }, eg::Vector3f(), { 1.0f, 1.0f, 1.0f });
	object.worldPos.z = 5.0f;
	eg::Vector3f cameraWorldPos = { 0.0f, 0.0f, 0.0f };
	eg::Vector3f cameraRot = { 0.0f, 0.0f, 0.0f };
	eg::Camera camera(cameraWorldPos, 1.0f, 100.0f, cameraRot, 90.0f, 900.0f, 600.0f);

	while (window.processEvents())
	{
		gfx.clear();

		object.modelToWorldTranslation();

		object.transform(camera.worldToCameraTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY, false);
		object.transform(camera.cameraToPerspectiveTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY, false);
		object.doZDivide();
		object.transform(camera.perspectiveToScreenTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY, false);

		std::vector<eg::Vector2i> polyline;
		for (uint32_t i = 0; i < object.nPolygons; ++i)
		{
			polyline.push_back(object.polygons[i].transformedCoords[0]);
			polyline.push_back(object.polygons[i].transformedCoords[1]);
			polyline.push_back(object.polygons[i].transformedCoords[2]);
			gfx.drawPolyline(polyline, Colors::White);
			polyline.clear();
		}

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
