#include "CPlayer.h"


CPlayer::CPlayer()
{
	health = 100;
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
