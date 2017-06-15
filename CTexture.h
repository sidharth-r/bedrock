#pragma once

#include <SDL.h>

class CTexture
{
public:
	SDL_Renderer* gRenderer;
	SDL_PixelFormat* format;
	SDL_Texture* mTexture;
	SDL_Surface* mSurface;
	SDL_Rect renderQuad;
	int mWidth, mHeight;

public:
	CTexture();
	CTexture(SDL_Renderer*, SDL_PixelFormat*, int, int);
	~CTexture();
	bool loadFromFile(char*);
	void free();
	void render(int, int, SDL_Rect* clip = NULL);
	void resizeQuad(double scale);
	void offsetXY(int, int);
	SDL_Color sampleTexture(int, int);
};