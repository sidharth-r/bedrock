#include "CPlayer.h"

#include <stdio.h>

CPlayer::CPlayer(SFrameInfo* f)
{
	frameInfo = f;
	health = 100;
	attTimer = 0;
}


CPlayer::~CPlayer()
{
}

int CPlayer::getHealth()
{
	return health;
}

void CPlayer::applyDamage(int d)
{
	health -= d;
}

void CPlayer::attack()
{
	if (frameInfo->time - attTimer > 700)
	{
		attTimer = frameInfo->time;
		printf_s("att");
	}
}
