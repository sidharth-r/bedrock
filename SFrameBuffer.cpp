#include "SFrameBuffer.h"

bool fInitBuffer(SDL_Window* window, SFrameBuffer* frameBuffer)
{
	//frameBuffer->screenBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_W, SCREEN_H);
	SDL_PixelFormat *format = SDL_GetWindowSurface(window)->format;
	frameBuffer->screenBuffer = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, 32, format->Rmask, format->Gmask, format->Bmask, format->Amask);
	SDL_SetColorKey(frameBuffer->screenBuffer, SDL_TRUE, 0);
	//SDL_SetSurfaceRLE(frameBuffer->screenBuffer, SDL_TRUE);
	SDL_SetSurfaceBlendMode(frameBuffer->screenBuffer, SDL_BLENDMODE_NONE);
	return true;
}

/*
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

void fDrawBuffer(SDL_Window* window, SFrameBuffer* frameBuffer)
{
//SDL_UpdateTexture(frameBuffer->screenBuffer, NULL, frameBuffer->pixel, SCREEN_W * sizeof(Uint32));
//SDL_RenderClear(renderer);
//SDL_RenderCopy(renderer, frameBuffer->screenBuffer, NULL, NULL);
SDL_Surface* screenBuffer = frameBuffer->screenBuffer, *screen = SDL_GetWindowSurface(window);
SDL_FillRect(screenBuffer, NULL, 255);
SDL_LockSurface(screenBuffer);
Uint32* p;
int bpp = screenBuffer->format->BytesPerPixel, pitch = screenBuffer->pitch;
for (int y = 0; y < SCREEN_H; y++)
{
for (int x = 0; x < SCREEN_W; x++)
{
p = (Uint32*)((Uint8*)screenBuffer->pixels + y * pitch + x * bpp);
*p = frameBuffer->pixel[y][x];
}
}
SDL_UnlockSurface(screenBuffer);

SDL_BlitSurface(screenBuffer, 0, screen, 0);
}


*/

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
	//frameBuffer->pixel[y][x] = pixel;
	SDL_Surface* screenBuffer = frameBuffer->screenBuffer;
	Uint32* p;
	int bpp = screenBuffer->format->BytesPerPixel, pitch = screenBuffer->pitch;
	p = (Uint32*)((Uint8*)screenBuffer->pixels + y * pitch + x * bpp);
	*p = pixel;
	frameBuffer->pixelData[y][x].id = id;
}

void fDrawBuffer(SDL_Window* window, SFrameBuffer* frameBuffer)
{
	//SDL_UpdateTexture(frameBuffer->screenBuffer, NULL, frameBuffer->pixel, SCREEN_W * sizeof(Uint32));
	//SDL_RenderClear(renderer);
	//SDL_RenderCopy(renderer, frameBuffer->screenBuffer, NULL, NULL);
	SDL_Surface* screenBuffer = frameBuffer->screenBuffer, *screen = SDL_GetWindowSurface(window);
	/*SDL_LockSurface(screenBuffer);
	Uint32* p;
	int bpp = screenBuffer->format->BytesPerPixel, pitch = screenBuffer->pitch;
	for (int y = 0; y < SCREEN_H; y++)
	{
		for (int x = 0; x < SCREEN_W; x++)
		{
			p = (Uint32*)((Uint8*)screenBuffer->pixels + y * pitch + x * bpp);
			*p = frameBuffer->pixel[y][x];
		}
	}
	SDL_UnlockSurface(screenBuffer);*/

	//SDL_FillRect(screenBuffer, NULL, SDL_MapRGBA(screenBuffer->format,255,255,255,255));
	SDL_BlitSurface(screenBuffer, 0, screen, 0);
	SDL_FillRect(screenBuffer, NULL, 0);

	SDL_UpdateWindowSurface(window);
}
