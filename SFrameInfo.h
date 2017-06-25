#pragma once

#include "vmath.h"

#include <stdio.h>

struct SFrameInfo
{
	int sWidth, sHeight;
	vmath::vec2d pos, dir, plane;
	double clipAngle, time, timeOld, frameTime;

	SDL_Renderer* renderer;
	SDL_Window* window;
};