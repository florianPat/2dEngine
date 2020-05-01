#pragma once

#include "Graphics2d.h"
#include <random>

class Stars3D
{
	const float spread;
	const float speed;
	const int nStars;
	float* starX;
	float* starY;
	float* starZ;

	std::uniform_real_distribution<float> dist;
	std::uniform_real<float> distZ;
	std::mt19937 rng;
public:
	Stars3D(int nStars, float spread, float speed);
	virtual ~Stars3D();
	void updateAndRender(eg::Graphics2d& gfx, float dt);
private:
	void initStar(int i);
};