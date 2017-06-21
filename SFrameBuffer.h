#pragma once

#include <SDL.h>

#define SCREEN_H 600
#define SCREEN_W 800

struct SPixel
{
	SDL_Color color;
	unsigned int id;

	SPixel()
	{
		color = { 0, 0, 0, 255 };
		id = 0;
	}
};

struct SFrameBuffer
{
	SPixel pixelData[SCREEN_H][SCREEN_W];
	Uint32 pixel[SCREEN_H][SCREEN_W];
	SDL_Texture* bufTexture;
};

bool fInitBuffer(SDL_Renderer*, SFrameBuffer*);

void fDraw_line_v(SFrameBuffer* frameBuffer, int x, int start, int end, Uint32 pixel, int id);
void fDraw_point(SFrameBuffer* frameBuffer, int x, int y, Uint32 pixel, int id);

void fDrawBuffer(SDL_Renderer*, SFrameBuffer*);
