#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "CActor.h"
#include "SFrameInfo.h"
#include "vmath.h"


#define SCREEN_H 600
#define SCREEN_W 800
#define G_TITLE "bedrock"

bool init();
bool loadData(char* mapFile = NULL);
void quit();
void drawFrame();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
CTexture* gSpriteEn;

struct __structColor
{
	SDL_Color white, black, red, blue, green, cyan, yellow, magenta, light_grey, dark_grey;
}Color{ 
	{ 255, 255, 255, 255 },
	{ 0, 0, 0, 255 }, 
	{ 255, 0, 0, 255 }, 
	{ 0, 0, 255, 255 }, 
	{ 0, 255, 0, 255 }, 
	{ 0, 255, 255, 255 }, 
	{ 255, 255, 0, 255 }, 
	{ 255, 0, 255, 255 }, 
	{ 90, 90, 90, 255 }, 
	{ 50, 50, 50, 255 }
};

std::vector<std::vector<int>> wMap, wMap_fallback = {
	{ 1, 2, 3, 2, 1 },
	{ 1, 0, 0, 0, 4 },
	{ 8, 0, 0, 0, 3 },
	{ 7, 0, 0, 0, 4 },
	{ 1, 5, 6, 7, 1 }
};

/*
double posX = 4, posY = 3;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 1;

double time = 0, timeOld = 0;
*/

SFrameInfo gFrameInfo = { SCREEN_W, SCREEN_H, { 4, 3 }, { -1, 0 }, { 0, 1 }, 0, 0, 0 };

using namespace vmath;

int main(int argc, char ** argv)
{
	//double magDir = sqrt(gFrameInfo.dirY * gFrameInfo.dirY + gFrameInfo.dirX * gFrameInfo.dirX);
	//double magPlane = sqrt(gFrameInfo.planeX * gFrameInfo.planeX + gFrameInfo.planeY * gFrameInfo.planeY);
	double magDir = magnitude(gFrameInfo.dir);
	double magPlane = magnitude(gFrameInfo.plane);
	gFrameInfo.clipAngle = atan(magPlane / magDir);

	if (!init())
	{
		printf_s("FATAL_ERROR: Exiting.\n");
		return 0;
	}

	if (argc > 1)
	{
		if (!loadData(argv[1]))
		{
			printf_s("ERROR: loadData failed.\n");
			return 0;
		}
	}
	else
	{
		if (!loadData())
		{
			printf_s("ERROR: loadData failed.\n");
			return 0;
		}
	}

	CActor en1(&gFrameInfo, gSpriteEn, 5, 5);

	bool fQuit = false;
	SDL_Event sdlEvt;

	while (!fQuit)
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		drawFrame();
		en1.draw();

		SDL_RenderPresent(gRenderer);

		gFrameInfo.timeOld = gFrameInfo.time;
		gFrameInfo.time = SDL_GetTicks();
		double frameTime = (gFrameInfo.time - gFrameInfo.timeOld) / 1000.0;
		double moveSpeed = frameTime * 1.5;
		double rotSpeed = frameTime * 3.0;

		while (SDL_PollEvent(&sdlEvt) != 0)
		{
			if (sdlEvt.type = SDL_KEYDOWN)
			{
				if (sdlEvt.key.keysym.sym == SDLK_ESCAPE)
					fQuit = true;
				if (sdlEvt.key.keysym.sym == SDLK_UP)
				{
					if (wMap[int(gFrameInfo.pos.x + gFrameInfo.dir.x * moveSpeed)][int(gFrameInfo.pos.y)] == false) 
						gFrameInfo.pos.x += gFrameInfo.dir.x * moveSpeed;
					if (wMap[int(gFrameInfo.pos.x)][int(gFrameInfo.pos.y + gFrameInfo.dir.y * moveSpeed)] == false) 
						gFrameInfo.pos.y += gFrameInfo.dir.y * moveSpeed;
				}
				if (sdlEvt.key.keysym.sym == SDLK_DOWN)
				{
					if (wMap[int(gFrameInfo.pos.x - gFrameInfo.dir.x * moveSpeed)][int(gFrameInfo.pos.y)] == false) 
						gFrameInfo.pos.x -= gFrameInfo.dir.x * moveSpeed;
					if (wMap[int(gFrameInfo.pos.x)][int(gFrameInfo.pos.y - gFrameInfo.dir.y * moveSpeed)] == false) 
						gFrameInfo.pos.y -= gFrameInfo.dir.y * moveSpeed;
				}
				if (sdlEvt.key.keysym.sym == SDLK_RIGHT)
				{
					double oldDirX = gFrameInfo.dir.x;
					gFrameInfo.dir.x = gFrameInfo.dir.x * cos(-rotSpeed) - gFrameInfo.dir.y * sin(-rotSpeed);
					gFrameInfo.dir.y = oldDirX * sin(-rotSpeed) + gFrameInfo.dir.y * cos(-rotSpeed);
					double oldPlaneX = gFrameInfo.plane.x;
					gFrameInfo.plane.x = gFrameInfo.plane.x * cos(-rotSpeed) - gFrameInfo.plane.y * sin(-rotSpeed);
					gFrameInfo.plane.y = oldPlaneX * sin(-rotSpeed) + gFrameInfo.plane.y * cos(-rotSpeed);
				}
				if (sdlEvt.key.keysym.sym == SDLK_LEFT)
				{
					double oldDirX = gFrameInfo.dir.x;
					gFrameInfo.dir.x = gFrameInfo.dir.x * cos(rotSpeed) - gFrameInfo.dir.y * sin(rotSpeed);
					gFrameInfo.dir.y = oldDirX * sin(rotSpeed) + gFrameInfo.dir.y * cos(rotSpeed);
					double oldPlaneX = gFrameInfo.plane.x;
					gFrameInfo.plane.x = gFrameInfo.plane.x * cos(rotSpeed) - gFrameInfo.plane.y * sin(rotSpeed);
					gFrameInfo.plane.y = oldPlaneX * sin(rotSpeed) + gFrameInfo.plane.y * cos(rotSpeed);
				}
#ifdef DEBUG
				if (sdlEvt.key.keysym.sym == SDLK_d)
				{
					printf_s("Player pos		: x: %f y: %f\n", gFrameInfo.pos.x, gFrameInfo.pos.y);
					printf_s("Player dir		: x: %f y: %f\n", gFrameInfo.dir.x, gFrameInfo.dir.y);
					printf_s("Plane				: x: %f y: %f\n", gFrameInfo.plane.x, gFrameInfo.plane.y);
					printf_s("Clip angle 		: %f\n", gFrameInfo.clipAngle);
					printf_s("-------------------------------------\n");
					printf_s("Actor:\n");
					printf_s("Pos				: x: %f y: %f\n", en1.pos.x, en1.pos.y);
					en1.debugVars();
					printf_s("-------------------------------------\n");
					printf_s("Texture:\n");
					printf_s("renderQuad pos	: x: %f y: %f\n", en1.texture->renderQuad.x, en1.texture->renderQuad.y);
					printf_s("renderQuad size	: w: %f h: %f\n", en1.texture->renderQuad.w, en1.texture->renderQuad.h);
				}
#endif
			}
		}

		SDL_Delay(17);
	}
	
	quit();
	return 0;
}

void drawFrame()
{
	for (int x = 0; x < SCREEN_W; x++)
	{
		double cameraX = 2 * x / double(SCREEN_W) - 1;
		double rayPosX = gFrameInfo.pos.x;
		double rayPosY = gFrameInfo.pos.y;
		double rayDirX = gFrameInfo.dir.x + gFrameInfo.plane.x * cameraX;
		double rayDirY = gFrameInfo.dir.y + gFrameInfo.plane.y * cameraX;

		int mapX = int(rayPosX);
		int mapY = int(rayPosY);

		double sideDistX, sideDistY;

		double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

		double perpWallDist;
		int stepX, stepY;

		int hit = 0, side;

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

		while (!hit)
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
			if (wMap[mapX][mapY] > 0)
			{
				hit = 1;
			}
		}

		if (side == 0)
			perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

		int lineHeight = (int)(SCREEN_H/ perpWallDist);

		int lStart = -lineHeight / 2 + SCREEN_H/ 2;
		if (lStart <= 0)
			lStart = 1;
		int lEnd = lineHeight / 2 + SCREEN_H/ 2;
		if (lEnd >= SCREEN_H)
			lEnd = SCREEN_H;
		
		SDL_Color color, colorCeil = Color.light_grey, colorFloor = Color.dark_grey;
		switch (wMap[mapX][mapY])
		{
		case 1:
			color = Color.white;
			break;
		case 2:
			color = Color.red;
			break;
		case 3:
			color = Color.green;
			break;
		case 4:
			color = Color.blue;
			break;
		case 5:
			color = Color.black;
			break;
		case 6:
			color = Color.yellow;
			break;
		case 7:
			color = Color.cyan;
			break;
		case 8:
			color = Color.magenta;
			break;
		default:
			color = Color.white;
		}
		if (side == 1)
		{
			color.r /= 2;
			color.g /= 2;
			color.b /= 2;
		}

		SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);		
		SDL_RenderDrawLine(gRenderer, x, lStart, x, lEnd);

		SDL_SetRenderDrawColor(gRenderer, colorCeil.r, colorCeil.g, colorCeil.b, colorCeil.a);
		SDL_RenderDrawLine(gRenderer, x, lStart, x, 0);

		SDL_SetRenderDrawColor(gRenderer, colorFloor.r, colorFloor.g, colorFloor.b, colorFloor.a);
		SDL_RenderDrawLine(gRenderer, x, lEnd, x, SCREEN_H);
	}

	//double cameraX = 2 * rSpriteEn.x / double(SCREEN_W) - 1;
	//double rayDirX = gFrameInfo.dirX + gFrameInfo.planeX * cameraX;
	//double rayDirY = gFrameInfo.dirY + gFrameInfo.planeY * cameraX;
	//double perpSpriteDist = (rSpriteEn.x - gFrameInfo.posX) / rayDirX;

	//gSpriteEn->render(200, 200);
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf_s("SDL_ERROR: SDL_Init failed.\n&s\n", SDL_GetError());
		return false;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &gWindow, &gRenderer) != 0)
		return false;

	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf_s("SDL_ERROR: Could not initialize SDL_image.\n%s\n", IMG_GetError());
		return false;
	}

	gSpriteEn = new CTexture(gRenderer, 512, 512);

	return true;
}

bool loadData(char* mapFile)
{
	if (!gSpriteEn->loadFromFile("en.png"))
		return false;

	FILE* map = NULL;

	if (mapFile != NULL)
	{
		fopen_s(&map, mapFile, "r");
		if (map == NULL)
		{
			printf_s("ERROR: Could not open map %s\n", mapFile);
			return false;
		}

		char buf[300];
		fgets(buf, 500, map);
		int rows, cols, i = 0;
		char m[3], n[3];
		while (buf[i] != ',')
		{
			m[i] = buf[i];
			i++;
		}
		rows = atoi(m); 
		i++;
		int j = 0;
		while (iswdigit(buf[i]))
		{
			n[j] = buf[i];
			i++;
			j++;
		}
		cols = atoi(n);
		wMap.resize(rows);
		for (i = 0; i < rows; i++)
		{
			wMap[i].resize(cols);

			if (fgets(buf, 300, map) == NULL)
			{
				printf_s("ERROR: Map load failed.\n");
				return false;
			}

			for (int c = 0, j = 0; buf[c] != '\0'; c++)
			{
				if (!iswdigit(buf[c]))
					continue;
				wMap[i][j] = buf[c] - '0';
				j++;
			}
		}
		
		/*
		for (i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				printf_s("%d ", wMap[i][j]);
			}
			printf_s("\n");
		}*/

		fclose(map);
	}
	else
	{
		wMap = wMap_fallback;
	}
	return true;
}

void quit()
{
	//SDL_FreeSurface(gSurface);
	//gSurface = NULL;

	gSpriteEn->free();
	delete gSpriteEn;
	gSpriteEn = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}