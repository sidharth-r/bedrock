#include "CEnemy.h"

CEnemy::CEnemy()
{
}

CEnemy::CEnemy(unsigned int i, SFrameInfo* f, CTexture* tex, int a, int b) : CActor(i, f, tex, a, b)
{
	id = i;
	attTimer = 0;
}


CEnemy::~CEnemy()
{
}

void CEnemy::process(CPlayer* player, vector<vector<int>> &wMap)
{
	if (player->getHealth() <= 0)
		return;

	vmath::vec2d playerPos = FrameInfo->pos;
	double dist = vmath::magnitude(playerPos - pos);
	if (dist > 0.5)
	{
		if (!rmath::dda(wMap, pos, playerPos))
		{
			return;
		}
		double moveSpeed = FrameInfo->frameTime * 0.1;
		vec2d moveDir = normalize(playerPos - pos);
		pos.x += moveDir.x * moveSpeed;
		pos.y += moveDir.y * moveSpeed;
		return;
	}

	if (FrameInfo->time - attTimer > 1000)
	{
		attTimer = FrameInfo->time;
		player->applyDamage(20);
#ifdef _DEBUG
		printf_s("Player health : %d\n", player->getHealth());
#endif
	}
}
