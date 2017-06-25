#pragma once

#include "CEntity.h"
#include "CTexture.h"

class CActor : public CEntity
{
#ifdef _DEBUG
	double tAngle, tScale;
#endif
public:
	CTexture* texture;
	CActor();
	CActor(unsigned int, SFrameInfo*, CTexture*, int, int);
	~CActor();
	void draw();
#ifdef _DEBUG
	void debugVars();
#endif
};

