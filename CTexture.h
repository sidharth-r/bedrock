#pragma once

#include <SDL.h>

class CTexture
{
public:
	SDL_Renderer* gRenderer;
	SDL_Texture* mTexture;
	SDL_Rect renderQuad;
	//int mWidth, mHeight;

public:
	CTexture();
	CTexture(SDL_Renderer*,int,int);
	~CTexture();
	bool loadFromFile(char*);
	void free();
	void render(int, int, SDL_Rect* clip = NULL);
	void resizeQuad(int, int);
};