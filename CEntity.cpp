#include "CEntity.h"

CEntity::CEntity()
{
}

CEntity::CEntity(unsigned int i, SFrameInfo* f)
{
	id = i;
	FrameInfo = f;
}

CEntity::~CEntity()
{
}
