#pragma once
#include "CActor.h"
#include "CPlayer.h"
#include "rmath.h"

class CEnemy : public CActor
{
	double attTimer;
public:
	CEnemy(SFrameInfo*, CTexture*, int, int);
	~CEnemy();
	void process(CPlayer*,vector<vector<int>>&);
};

