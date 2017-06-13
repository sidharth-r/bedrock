#include "CTexture.h"
#include <SDL_image.h>
#include <stdio.h>

CTexture::CTexture()
{
}

CTexture::CTexture(SDL_Renderer* r, int w, int h)
{
	gRenderer = r;
	//mWidth = w;
	//mHeight = h;
	mTexture = NULL;
	renderQuad = { 0, 0, w, h };
}

CTexture::~CTexture()
{
	free();
}

bool CTexture::loadFromFile(char* path)
{
	SDL_Surface* temp = IMG_Load(path);
	if (temp == NULL)
	{
		printf_s("ERROR: IMG_Load failed.\nPath:%s\n%s", path, IMG_GetError());
		return false;
	}

	mTexture = SDL_CreateTextureFromSurface(gRenderer, temp);
	if (mTexture == NULL)
	{
		printf_s("ERROR: SDL_CreateTextureFromSurface failed.\n%s\n", IMG_GetError());
		return false;
	}
	SDL_FreeSurface(temp);

	return true;
}

void CTexture::free()
{
	SDL_DestroyTexture(mTexture);
}

void CTexture::render(int x, int y, SDL_Rect* clip)
{
	renderQuad.x = x;
	renderQuad.y = y;
	
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

void CTexture::resizeQuad(int w, int h)
{
	renderQuad.w = w;
	renderQuad.h = h;
}