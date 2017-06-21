#include "SFrameBuffer.h"

bool fInitBuffer(SDL_Renderer* renderer, SFrameBuffer* frameBuffer)
{
	frameBuffer->bufTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_W, SCREEN_H);
	return true;
}

void fDraw_line_v(SFrameBuffer* frameBuffer, int x, int start, int end, Uint32 pixel, int id)
{
	int t;
	if (start > end)
	{
		t = start;
		start = end;
		end = t;
	}

	for (int y = start; y < end; y++)
	{
		//frameBuffer->pixel[x][y].color = color;
		frameBuffer->pixel[y][x] = pixel;
		frameBuffer->pixelData[y][x].id = id;
	}
}

void fDraw_point(SFrameBuffer* frameBuffer, int x, int y, Uint32 pixel, int id)
{
	//frameBuffer->pixel[x][y].color = color;
	frameBuffer->pixel[y][x] = pixel;
	frameBuffer->pixelData[y][x].id = id;
}

void fDrawBuffer(SDL_Renderer* renderer, SFrameBuffer* frameBuffer)
{
	SDL_UpdateTexture(frameBuffer->bufTexture, NULL, frameBuffer->pixel, SCREEN_W * sizeof(Uint32));
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, frameBuffer->bufTexture, NULL, NULL);
}
