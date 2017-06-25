#include "CActor.h"
#include <math.h>

CActor::CActor()
{

}

CActor::CActor(unsigned int i, SFrameInfo* f,CTexture* tex , int a, int b)
{
	id = i;
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
#ifdef _DEBUG
	tAngle = angle;
#endif

	if (angle > FrameInfo->clipAngle)
		return;

	double camX = angle / FrameInfo->clipAngle;

	int screenY = FrameInfo->sHeight / 2;
	int screenX = (camX + 1) * FrameInfo->sWidth / 2;

#ifdef _DEBUG
	/*double p1 = pos * FrameInfo->plane;
	printf_s("\npos*plane			: %f\n", p1);
	printf_s("angle()			: %f\n", vmath::angle(pos, FrameInfo->plane));
	printf_s("angle_raw()			: %f\n", vmath::angle_raw(pos, FrameInfo->plane));
	printf_s("tanangle()			: %f\n", tan(vmath::angle(pos, FrameInfo->plane)));*/
#endif

	double diffZ = vmath::magnitude(pos - FrameInfo->pos);	
	diffZ < 1 ? 1 : diffZ;
	double scale = 1 / diffZ;

#ifdef _DEBUG
	tScale = scale;
#endif

	texture->resizeQuad(scale);

	texture->render(screenX,screenY);
}

#ifdef _DEBUG
void CActor::debugVars()
{
	printf_s("Angle				: %f\n", tAngle);
	printf_s("Scale				: %f\n", tScale);
}
#endif
