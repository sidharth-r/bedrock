#pragma once
class CPlayer
{
	int health;
public:
	CPlayer();
	~CPlayer();
	int getHealth();
	void applyDamage(int);
};

