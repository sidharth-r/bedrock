#pragma once

#include "SFrameInfo.h"
#include "vmath.h"

class CEntity
{
public:
	CEntity();
	CEntity(SFrameInfo*);
	~CEntity();
	vmath::vec2d pos;
	SFrameInfo* FrameInfo;
};

