#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class gTexture
{
public:
	gTexture(SDL_Texture *t = NULL, int x = 0, int y = 0, int w = 0, int h = 0);
	gTexture(const int x, const int y);
	gTexture(const gTexture &obj);
	gTexture& operator=(const gTexture &obj);
	~gTexture();
	virtual void Draw(SDL_Renderer* renderer, SDL_Rect *clip = NULL, float angle = 0.0f,
		SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	bool loadTexture(SDL_Renderer *renderer, std::string path);
	void setSize(int w, int h);
	bool loadTextureFromText(SDL_Renderer *renderer, TTF_Font *font = NULL, 
		std::string textureText = "Default", SDL_Color bgColor = { 0xff,0xff,0xff,0xff }, SDL_Color textColor = { 0,0,0,0xff });
	SDL_Rect getCollider();

	void setPosition(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int getTextureWitdh() {
		return tWidth;
	}
	int getTextureHeight() {
		return tHeight;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	SDL_Texture* getTexture() {
		return texture;
	}
	void gFree();

private:
	SDL_Texture * texture;
	int tWidth, tHeight;
	int x, y;

};

