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
	object.rot.y = 45.0f;
	const float fov = 90.0f;
	const float nearZ = 2.0f;
	const float farZ = 100.0f;
	eg::Vector3f cameraWorldPos = { 0.0f, 0.0f, 0.0f };
	eg::Vector3f cameraRot = { 0.0f, 0.0f, 0.0f };
	eg::Camera camera(cameraWorldPos, nearZ, farZ, cameraRot, fov, 900.0f, 600.0f);

	while (window.processEvents())
	{
		gfx.clear();

		if (window.getKeyboard().isKeyPressed('W'))
		{
			object.worldPos.z += 20.0f * dt;
		}
		if (window.getKeyboard().isKeyPressed('S'))
		{
			object.worldPos.z -= 20.0f * dt;
		}

		object.modelToWorldRotation(eg::TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS);
		object.modelToWorldTranslation(eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.cullBackfaces(camera.worldPos);
		object.transform(camera.worldToCameraTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.clipInCameraSpace(fov, nearZ, farZ);
		object.transform(camera.cameraToPerspectiveTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.doZDivide();
		object.transform(camera.perspectiveToScreenTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.drawSolid(gfx);
		object.removeAddedClippingPolygons();
		object.clearFlags();

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
