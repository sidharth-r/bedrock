#pragma once

#include "CEntity.h"
#include "CTexture.h"

class CActor : public CEntity
{
#ifdef DEBUG
	double tAngle, tScale;
#endif
public:
	CTexture* texture;
	CActor(SFrameInfo*,CTexture*,int,int);
	~CActor();
	void draw();
#ifdef DEBUG
	void debugVars();
#endif
};

