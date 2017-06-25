#pragma once

#include "SFrameBuffer.h"
#include "vmath.h"

#include <stdio.h>

struct SFrameInfo
{
	int sWidth, sHeight;
	vmath::vec2d pos, dir, plane;
	double clipAngle, time, timeOld, frameTime;
	SFrameBuffer frameBuffer;

	SDL_Renderer* renderer;
	SDL_Window* window;
};