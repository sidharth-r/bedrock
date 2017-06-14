#pragma once

#include "SFrameInfo.h"

class CPlayer
{
	int health;
	double attTimer;
	SFrameInfo* frameInfo;
public:
	CPlayer(SFrameInfo*);
	~CPlayer();
	int getHealth();
	void applyDamage(int);
	void attack();
};

