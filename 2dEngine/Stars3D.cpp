#include "Stars3D.h"

#include <random>
#include <math.h>

Stars3D::Stars3D(int nStars, float spread, float speed) : spread(spread), speed(speed), nStars(nStars), dist(-1.0f, 1.0f),
														  distZ(0.0001f, 1.0f), rng()
{
	starX = new float[nStars];
	starY = new float[nStars];
	starZ = new float[nStars];

	for (int i = 0; i < nStars; ++i)
	{
		initStar(i);
	}
}

Stars3D::~Stars3D()
{
	delete[] starX;
	delete[] starY;
	delete[] starZ;
}

void Stars3D::updateAndRender(eg::Graphics2d & gfx, float dt)
{
	float fovAngle = 70.0f;
	float tanHalfFOV = tanf((fovAngle*(float)std::_Pi / 180.0f) / 2.0f);

	for (int i = 0; i < nStars; ++i)
	{
		starZ[i] -= dt;

		if (starZ[i] <= 0.0f)
		{
			initStar(i);
		}

		const float halfWidth = 450.0f;
		const float halfHeight = 300.0f;

		int x = (int)(starX[i] / (starZ[i] * tanHalfFOV) * halfWidth + halfWidth);
		int y = (int)((starY[i] * -1.0f) / (starZ[i]* tanHalfFOV) * halfHeight + halfHeight);

		if (x >= 0 && x < (halfWidth * 2) && y >= 0 && y < (halfHeight * 2))
			gfx.putPixel(x, y, Colors::White);
		else
			initStar(i);
	}
}

void Stars3D::initStar(int i)
{
	starX[i] = dist(rng);
	starY[i] = dist(rng);
	starZ[i] = distZ(rng);
}
