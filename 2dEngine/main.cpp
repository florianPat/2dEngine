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
	//object.rot.y = 45.0f;
	const float fov = 90.0f;
	const float nearZ = 2.0f;
	const float farZ = 100.0f;
	eg::Vector3f cameraWorldPos = { 0.0f, 0.0f, 0.0f };
	eg::Vector3f cameraRot = { 0.0f, 0.0f, 0.0f };
	eg::Camera camera(cameraWorldPos, nearZ, farZ, cameraRot, fov, 900.0f, 600.0f);
	
	std::vector<eg::Material> materials;
	materials.push_back({});
	std::vector<eg::Light> lights;
	eg::Light ambientLight = eg::Light{ eg::LightType::AMBIENT, Color(10, 10, 10) };
	lights.push_back(ambientLight);
	eg::Light directionalLight = {};
	directionalLight.lightType = eg::LightType::DIRECTIONAL;
	directionalLight.diffuseIntensity = Color(255, 255, 0);
	directionalLight.dir = eg::Vector3f(-1.0f, -1.0f, 0.0f).normalize();
	lights.push_back(directionalLight);
	eg::Light pointLight = {};
	pointLight.lightType = eg::LightType::POINT;
	pointLight.diffuseIntensity = Color(0, 255, 255);
	pointLight.pos = eg::Vector3f(0.0f, 1000.0f, 0.0f);
	pointLight.attentuationLinear = 1.0f;
	lights.push_back(pointLight);

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
		if (window.getKeyboard().isKeyPressed('Q'))
		{
			object.rot.y -= 60.0f * dt;
		}
		if (window.getKeyboard().isKeyPressed('E'))
		{
			object.rot.y += 60.0f * dt;
		}

		object.modelToWorldRotation(eg::TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS);
		object.modelToWorldTranslation(eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.cullBackfaces(camera.worldPos);
		//object.doConstantShading();
		//object.doFlatShading(lights);
		object.doGouradShading(lights);
		object.transform(camera.worldToCameraTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.clipInCameraSpace(fov, nearZ, farZ);
		object.transform(camera.cameraToPerspectiveTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.doZDivide();
		object.transform(camera.perspectiveToScreenTransform, eg::TransformCase::TRANSFORM_COORDS_ONLY);
		object.drawSolid(gfx, materials);
		object.removeAddedClippingPolygons();
		object.clearFlags();

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
