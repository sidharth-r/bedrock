#pragma once

#include "vmath.h"

/*
struct SFrameInfo
{
	double posX, posY, dirX, dirY, planeX, planeY, time, timeOld;
	double clipAngle;
};*/

struct SFrameInfo
{
	vmath::vec2d pos, dir, plane;
	double clipAngle, time, timeOld;
};