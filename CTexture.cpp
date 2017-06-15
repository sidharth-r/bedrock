#include "CTexture.h"
#include <SDL_image.h>
#include <stdio.h>

CTexture::CTexture()
{
}

CTexture::CTexture(SDL_Renderer* r, SDL_PixelFormat* f, int w, int h)
{
	gRenderer = r;
	format = f;
	mWidth = w;
	mHeight = h;
	mTexture = NULL;
	renderQuad = { 0, 0, 0, 0 };
}

CTexture::~CTexture()
{
	free();
}

bool CTexture::loadFromFile(char* path)
{
	mSurface = IMG_Load(path);
	//mSurface = SDL_LoadBMP(path);
	if (mSurface == NULL)
	{
		printf_s("ERROR: IMG_Load failed.\nPath:%s\n%s", path, IMG_GetError());
		return false;
	}

	mTexture = SDL_CreateTextureFromSurface(gRenderer, mSurface);
	//mTexture = SDL_CreateTexture(gRenderer, format->format, 0, mSurface->w, mSurface->h);
	
	if (mTexture == NULL)
	{
		printf_s("ERROR: SDL_CreateTextureFromSurface failed.\n%s\n", IMG_GetError());
		return false;
	}
	
	return true;
}

void CTexture::free()
{
	SDL_DestroyTexture(mTexture);
	SDL_FreeSurface(mSurface);
}

void CTexture::render(int x, int y, SDL_Rect* clip)
{
	offsetXY(x, y);
	
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

void CTexture::resizeQuad(double scale)
{
	renderQuad.w = mWidth * scale;
	renderQuad.h = mHeight * scale;
}

void CTexture::offsetXY(int x, int y)
{
	renderQuad.x = x - renderQuad.w / 2;
	renderQuad.y = y - renderQuad.h / 2;
}

SDL_Color CTexture::sampleTexture(int x, int y)
{
	SDL_Color col = { 0, 0, 0, 255 };

	if (SDL_LockSurface(mSurface) == -1)
	{
		printf_s("ERROR: Could not lock surface.");
		return col;
	}
	int bpp = mSurface->format->BytesPerPixel;
	if (bpp != 4)
		printf_s("WARNING: Surface is not 32bpp.\n");

	Uint32 p = *(Uint32*)mSurface->pixels + y * mSurface->pitch + x * bpp;	
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

	if (a > 0 && a < 255)
	{
		printf_s("invalpha");
	}

	if ((r > 0 || g > 0 || b > 0) && a == 255)
	{
		printf_s("colhit");
	}

	col = { r, g, b, a };

	return col;
}