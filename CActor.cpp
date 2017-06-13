#include "CActor.h"
#include <math.h>

CActor::CActor(SFrameInfo* f,CTexture* tex, int a, int b)
{
	FrameInfo = f;
	texture = tex;
	pos.x = a;
	pos.y = b;
}

CActor::~CActor()
{
}

void CActor::draw()
{
	vmath::vec2d v = pos - FrameInfo->pos;
	double magVec = vmath::magnitude(v);
	double magDir = vmath::magnitude(FrameInfo->dir);

	double angle = vmath::angle(v, FrameInfo->dir);

	double a = vmath::angle({ 0, -4 }, { -1, -3 }), b = vmath::angle({ 0, -4 }, { 1, -3 });

	if (angle > FrameInfo->clipAngle)
		return;



	texture->render(pos.x,pos.y);
}

