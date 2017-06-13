#pragma once

#include "CEntity.h"
#include "CTexture.h"

class CActor : private CEntity
{
	CTexture* texture;
public:
	CActor(SFrameInfo*,CTexture*,int,int);
	~CActor();
	void draw();
};

