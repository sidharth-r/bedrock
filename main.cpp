#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <thread>
#include <vector>

#include "CActor.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "SFrameBuffer.h"
#include "SFrameInfo.h"
#include "vmath.h"

#define SCREEN_H 600
#define SCREEN_W 800
#define G_TITLE "bedrock"

#define TEX_WIDTH 256
#define TEX_HEIGHT 256

bool init();
bool loadData(char* mapFile = NULL);
void quit();
void drawFrame();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
CTexture* gSpriteEn;
CTexture* gSpriteEn2;

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

SFrameInfo gFrameInfo = { SCREEN_W, SCREEN_H, { 4, 3 }, { -1, 0 }, { 0, 1 }, 0, 0, 0, 0 };

CPlayer gPlayer(&gFrameInfo);

//CEnemy en1(2, &gFrameInfo, gSpriteEn, 5, 5);
//CEnemy en2(3, &gFrameInfo, gSpriteEn2, 4, 1);
CEnemy en1, en2;

bool f = false;
/*
void draw(SDL_Surface* mSurface, SDL_Window* w3, SDL_Renderer* r3)
{
	if (f == false)
	{
		for (int x = 0; x < 256; x++)
		{
			int y = 230;
			if (SDL_LockSurface(mSurface) == -1)
			{
				printf_s("ERROR: Could not lock surface.");
				return;
			}
			int bpp = mSurface->format->BytesPerPixel;
			if (bpp != 4)
				printf_s("WARNING: Surface is not 32bpp.\n");

			Uint8* px = (Uint8*)mSurface->pixels + y * mSurface->pitch + x * bpp;
			Uint32 p = *(Uint32*)px;
			SDL_UnlockSurface(mSurface);
			
			printf_s("|%d|", p);
		}
		f = true;
	}


	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			SDL_Color col = { 0, 0, 0, 255 };

			if (SDL_LockSurface(mSurface) == -1)
			{
				printf_s("ERROR: Could not lock surface.");
				return;
			}
			int bpp = mSurface->format->BytesPerPixel;
			if (bpp != 4)
				printf_s("WARNING: Surface is not 32bpp.\n");

			Uint8* px = (Uint8*)mSurface->pixels + y * mSurface->pitch + x * bpp;
			Uint32 p = *(Uint32*)px;
			SDL_UnlockSurface(mSurface);
			
			Uint8 r, g, b, a;
			SDL_PixelFormat* format = mSurface->format;
			Uint32 temp = p & format->Rmask;
			temp = temp >> format->Rshift;
			temp = temp << format->Rloss;
			r = (Uint8)temp;
			temp = p & format->Gmask;
			temp = temp >> format->Gshift;
			temp = temp << format->Gloss;
			g = (Uint8)temp;
			temp = p & format->Bmask;
			temp = temp >> format->Bshift;
			temp = temp << format->Bloss;
			b = (Uint8)temp;
			temp = p & format->Amask;
			temp = temp >> format->Ashift;
			temp = temp << format->Aloss;
			a = (Uint8)temp;

			SDL_GetRGBA(p, format, &r, &g, &b, &a);

			if (a > 0 && a < 255)
			{
				printf_s("invalpha");
			}

			if ((r > 0 || g > 0 || b > 0) && a == 255)
			{
				printf_s("colhit");
			}

			col = { r, g, b, a };

			SDL_SetRenderDrawColor(r3, r, g, b, a);
			SDL_RenderDrawPoint(r3, x, y);

		}
	}
	SDL_RenderPresent(r3);
	
}
*/

using namespace vmath;

int main(int argc, char ** argv)
{
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

	fInitBuffer(gWindow, &gFrameInfo.frameBuffer);

	en1 = CEnemy(2,&gFrameInfo, gSpriteEn, 6, 5);
	//en1 = e1;
	en2 = CEnemy(3,&gFrameInfo, gSpriteEn2, 5, 2);
	//en2 = e2;

	bool fQuit = false;
	SDL_Event sdlEvt;

	//SDL_Window* w2 = SDL_CreateWindow("2", 0, 0, 800, 600, 0);
	//SDL_Surface* s = SDL_LoadBMP("ena.bmp");

	SDL_Window* w3;
	SDL_Renderer* r3;

	//w3 = SDL_CreateWindow("3", 200, 800, 257, 257, 0);
	//SDL_CreateWindowAndRenderer(257, 257, NULL, &w3, &r3);

	while (!fQuit)
	{
		/*
		if (SDL_BlitSurface(en1.texture->mSurface, NULL, SDL_GetWindowSurface(w2), NULL) != 0)
		{
			printf_s("%s\n", SDL_GetError());
		}
		SDL_UpdateWindowSurface(w2);

		draw(en1.texture->mSurface,w3,r3);*/

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		drawFrame();
		//en1.draw();
		//en2.draw();

		if (gPlayer.getHealth() <= 0)
		{
			SDL_SetRenderDrawColor(gRenderer, Color.red.r / 2, Color.red.g, Color.red.b, Color.red.a);
			SDL_RenderClear(gRenderer);
		}

		fDrawBuffer(gWindow, &gFrameInfo.frameBuffer);
		//fDrawBuffer(w2, &gFrameInfo.frameBuffer);
		
		//SDL_RenderPresent(gRenderer);
				
		//en1.process(&gPlayer,wMap);
		//en2.process(&gPlayer,wMap);

		gFrameInfo.timeOld = gFrameInfo.time;
		gFrameInfo.time = SDL_GetTicks();
		gFrameInfo.frameTime = (gFrameInfo.time - gFrameInfo.timeOld) / 1000.0;
		double moveSpeed = gFrameInfo.frameTime * 1.5;
		double rotSpeed = gFrameInfo.frameTime * 3.0;

		while (SDL_PollEvent(&sdlEvt) != 0)
		{
			if (sdlEvt.type == SDL_MOUSEBUTTONDOWN)
			{
				if (sdlEvt.button.button == SDL_BUTTON_LEFT)
				{
					gPlayer.attack();
				}
			}
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
//dbg
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
//dbg
			}
		}

		SDL_Delay(17);
	}
	
	quit();
	return 0;
}

void drawFrame()
{
	int zBuf[SCREEN_W];

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

		zBuf[x] = perpWallDist;

		int lineHeight = (int)(SCREEN_H / perpWallDist);

		int lStart = -lineHeight / 2 + SCREEN_H / 2;
		if (lStart <= 0)
			lStart = 1;
		int lEnd = lineHeight / 2 + SCREEN_H / 2;
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

	int numSprites = 2;
	int sprOrder[2];
	double sprZ[2];
	CEnemy* sprite[2] = { &en1, &en2 };

	for (int i = 0; i < numSprites; i++)
	{
		sprOrder[i] = i;
		sprZ[i] = magnitude(gFrameInfo.pos - sprite[i]->pos);
	}

	//sort
	SDL_LockSurface(gFrameInfo.frameBuffer.screenBuffer);
	for (int i = 0; i < numSprites; i++)
	{
		vec2d dir = gFrameInfo.dir, plane = gFrameInfo.plane, pos = gFrameInfo.pos;

		vec2d spr = sprite[sprOrder[i]]->pos - pos;

		double invDet = 1 / (plane.x * dir.y - plane.y * dir.x);

		vec2d transform = { invDet * (dir.y * spr.x - dir.x * spr.y), invDet * (plane.x * spr.y - plane.y * spr.x) };

		int sprScreenX = int((SCREEN_W / 2) * (1 + transform.x / transform.y));
		
		int sprH = abs(int(SCREEN_H / transform.y));
		int startY = SCREEN_H / 2 - sprH / 2;
		if (startY < 0)
			startY = 0;
		int endY = SCREEN_H / 2 + sprH / 2;
		if (endY >= SCREEN_H)
			endY = SCREEN_H - 1;

		int sprW = abs(int(SCREEN_H / transform.y));
		int startX = sprScreenX - sprW / 2;
		if (startX < 0)
			startX = 0;
		int endX = sprScreenX + sprW / 2;
		if (endX >= SCREEN_W)
			endX = SCREEN_W - 1;

		if (i == 0 && transform.y > 0)
		{
			//printf_s("sfsf");
		}

		for (int str = startX; str < endX; str++)
		{
			//int tX = int(256 * (str - (sprScreenX - sprW / 2) * TEX_WIDTH / sprW)) / 256;
			int tX = int((str - (sprScreenX - sprW / 2)) * TEX_WIDTH / double(sprW));

			if (transform.y <= zBuf[str] && transform.y > 0)
			{
				//printf_s("%d", str);
			}

			if (transform.y > 0 && str > 0 && str < SCREEN_W && transform.y < zBuf[str])
			{
				for (int y = startY; y < endY; y++)
				{
					int t = y * 256 - SCREEN_H * 128 + sprH * 128;
					int tY = ((t * TEX_HEIGHT) / sprH) / 256;
					Uint32 p = sprite[sprOrder[i]]->texture->sampleTexture(tX, tY);
					Uint8 a = sprite[sprOrder[i]]->texture->sampleAlpha(tX, tY);
					if (a > 0)
					{
						fDraw_point(&gFrameInfo.frameBuffer, str, y, p, i + 2);
					}
				}
			}
		}
	}
	SDL_UnlockSurface(gFrameInfo.frameBuffer.screenBuffer);
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf_s("SDL_ERROR: SDL_Init failed.\n&s\n", SDL_GetError());
		return false;
	}

	/*if (SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &gWindow, &gRenderer) != 0)
	return false;*/
	gWindow = SDL_CreateWindow(G_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);
	if (!gWindow)
	{
		printf_s("SDL_ERROR: Could not create window.\n%s\n", SDL_GetError());
		return false;
	}

	gRenderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(gWindow));
	if (!gRenderer)
	{
		printf_s("SDL_ERROR: Could not create renderer.\n%s\n", SDL_GetError());
		return false;
	}

	gFrameInfo.renderer = gRenderer;
	gFrameInfo.window = gWindow;

	SDL_SetWindowGrab(gWindow, SDL_TRUE);

	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf_s("SDL_ERROR: Could not initialize SDL_image.\n%s\n", IMG_GetError());
		return false;
	}

	return true;
}

bool loadData(char* mapFile)
{
	gSpriteEn = new CTexture(gRenderer, 256, 256);
	if (!gSpriteEn->loadFromFile("en4.png"))
		return false;

	gSpriteEn2 = new CTexture(gRenderer, 256, 256);
	if (!gSpriteEn2->loadFromFile("en.png"))
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
	delete gSpriteEn;
	gSpriteEn = NULL;

	delete gSpriteEn2;
	gSpriteEn2 = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}