#include "rmath.h"

bool rmath::dda(vector<vector<int>> &wMap, vec2d origin, vec2d dest)
{
	double dir = magnitude(dest - origin);

	double rayPosX = origin.x;
	double rayPosY = origin.y;
	double rayDirX = (dest - origin).x;
	double rayDirY = (dest - origin).y;

	int mapX = int(origin.x);
	int mapY = int(origin.y);

	double sideDistX, sideDistY;

	double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
	double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

	int stepX, stepY;

	int side;

	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (rayPosX - mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
	}
	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (rayPosY - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
	}

	while (true)
	{
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			mapX += stepX;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			mapY += stepY;
			side = 1;
		}
		if (mapX == int(dest.x) && mapY == int(dest.y))
			return true;
		if (wMap[mapX][mapY] > 0)
			return false;
	}
}