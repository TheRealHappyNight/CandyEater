#include "gTexture.h"
#include <iostream>

gTexture::gTexture(SDL_Texture *t, int X, int Y, int w, int h):tWidth(w), tHeight(h), x(X), y(Y){
	texture = t;
}

gTexture::gTexture(const int x, const int y) {
	this->x = x;
	this->y = y;
}

gTexture::gTexture(const gTexture &obj): tWidth(obj.tWidth), tHeight(obj.tHeight), x(obj.x), y(obj.y) {
	texture = obj.texture;
}
gTexture& gTexture::operator=(const gTexture &obj) {
	texture = obj.texture;
	tWidth = obj.tWidth;
	tHeight = obj.tHeight;
	x = obj.x;
	y = obj.y;

	return (*this);
}

void gTexture::Draw(SDL_Renderer* renderer, SDL_Rect *clip, float angle, SDL_Point *center, SDL_RendererFlip flip) {
	SDL_Rect tSize = { x, y, tWidth, tHeight };

	SDL_RenderCopyEx(renderer, texture, clip, &tSize, angle, center, flip);
}

bool gTexture::loadTexture(SDL_Renderer* renderer, std::string path) {
	SDL_Surface *surface = NULL;
	gFree();

	surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cout << "Image loading failed " << IMG_GetError() << std::endl;
		std::cin.get();
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	tWidth = surface->w;
	tHeight = surface->h;
	return texture != NULL;
}

void gTexture::setSize(int w, int h) {
	tWidth = w;
	tHeight = h;
}

bool gTexture::loadTextureFromText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color bgColor, SDL_Color textColor) {
	gFree();

	//SDL_Surface *tempSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	SDL_Surface *tempSurface = TTF_RenderText_Shaded(font, textureText.c_str(), textColor, bgColor);
	if (!tempSurface) {
		std::cout << "Text not created " << TTF_GetError() << std::endl;
	}
	else {
		SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0xff, 0xff, 0xff));
		texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		if (!texture) {
			std::cout << "Surface not created " << SDL_GetError() << std::endl;
		}
		else {
			tWidth = tempSurface->w;
			tHeight = tempSurface->h;
		}
		SDL_FreeSurface(tempSurface);
	}
	return texture != NULL;
}

void gTexture::gFree() {
	if(texture != NULL)
		SDL_DestroyTexture(texture);
	texture = NULL;

	tWidth = 0;
	tHeight = 0;
}

SDL_Rect gTexture::getCollider() {
	return { x, y, tWidth, tHeight };
}

gTexture::~gTexture() {
}
