#pragma once

#include "SFrameInfo.h"
#include "vmath.h"

class CEntity
{
public:
	CEntity();
	CEntity(unsigned int, SFrameInfo*);
	~CEntity();
	vmath::vec2d pos;
	SFrameInfo* FrameInfo;
	unsigned int id;
};

