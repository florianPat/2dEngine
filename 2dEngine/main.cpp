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
	eg::Mat4x4 rotMat = eg::Mat4x4::rotateY(45.0f);
	eg::Vector3f cameraWorldPos = { 0.0f, 0.0f, 0.0f };
	eg::Vector3f cameraRot = { 0.0f, 0.0f, 0.0f };
	eg::Camera camera(cameraWorldPos, 1.0f, 100.0f, cameraRot, 90.0f, 900.0f, 600.0f);

	while (window.processEvents())
	{
		gfx.clear();

		object.transform(rotMat, eg::TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS, false);
		object.modelToWorldTranslation(eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.cullBackfaces(camera.worldPos);
		object.transform(camera.worldToCameraTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY, false);
		object.transform(camera.cameraToPerspectiveTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY, false);
		object.doZDivide();
		object.transform(camera.perspectiveToScreenTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY, false);
		object.drawSolid(gfx);
		object.clearFlags();

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
