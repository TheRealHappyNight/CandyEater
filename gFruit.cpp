#include "gFruit.h"
#include <iostream>
#include <math.h>

float gFruit::moveSpeed = 5;

gFruit::gFruit(SDL_Texture *t, int x, int y, int w, int h) : gTexture(t,x,y,w,h)
{
	int tWidth = getTextureWitdh() / 2;
	int tHeight = getTextureHeight() / 2;
	
	this->circleCollider.x = x + tWidth;
	this->circleCollider.y = y + tHeight;
	this->circleCollider.r = sqrt(tWidth * tWidth + tHeight * tHeight) - 10;
}

/*
gFruit::gFruit(const gFruit &obj) {
	(gTexture)(*this) = obj;
	circleCollider = obj.circleCollider;
	moveSpeed = obj.moveSpeed;
}

gFruit& gFruit::operator=(const gFruit &obj) {
	(gTexture)(*this) = obj;
	circleCollider = obj.circleCollider;
	moveSpeed = obj.moveSpeed;

	return (*this);
}*/

bool gFruit::checkColliderWithPlayer(SDL_Rect pCollider) {
	int cX, cY;

	if (circleCollider.x < pCollider.x)
		cX = pCollider.x;
	else if (circleCollider.x > pCollider.x + pCollider.w)
		cX = pCollider.x + pCollider.w;
	else
		cX = circleCollider.x;

	if (circleCollider.y < pCollider.y)
		cY = pCollider.y;
	else if (circleCollider.y > pCollider.y + pCollider.h)
		cY = pCollider.y + pCollider.h;
	else
		cY = circleCollider.y;

	int deltaX = cX - circleCollider.x;
	int deltaY = cY - circleCollider.y;

	if (deltaX * deltaX + deltaY * deltaY < circleCollider.r * circleCollider.r)
		return true;

	return false;
}

void gFruit::setPosition(int x, int y) {
	int tWidth = getTextureWitdh() / 2;
	int tHeight = getTextureHeight() / 2;

	this->circleCollider.x = x + tWidth;
	this->circleCollider.y = y + tHeight;
}

void gFruit::Draw(SDL_Renderer* renderer, SDL_Rect *clip, float angle, SDL_Point *center, SDL_RendererFlip flip) {
	SDL_Rect tSize = { circleCollider.x - (gTexture::getTextureWitdh() / 2), circleCollider.y - (gTexture::getTextureHeight() / 2)
		, gTexture::getTextureWitdh(), gTexture::getTextureHeight() };

	SDL_RenderCopyEx(renderer, gTexture::getTexture(), clip, &tSize, angle, center, flip);
}

void gFruit::DrawCollider(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
	SDL_Point *a = new SDL_Point[5];
	a[0] = { circleCollider.x, circleCollider.y - (int)circleCollider.r};
	a[1] = { circleCollider.x - (int)circleCollider.r, circleCollider.y};
	a[2] = { circleCollider.x, circleCollider.y + (int)circleCollider.r};
	a[3] = { circleCollider.x + (int)circleCollider.r, circleCollider.y};
	a[4] = a[0];
	if (SDL_RenderDrawLines(renderer, a, 5) < 0)
		std::cout << "Not Okay\n";
	delete a;
}

void gFruit::Move(SDL_Rect player, SDL_Renderer* renderer ,int *lifes, int *gScore, SDL_Rect *clip, float angle,
	SDL_Point *center, SDL_RendererFlip flip) {
	if (circleCollider.y - circleCollider.r - 1 > wHeight) {
		(*lifes)--;
		gFree();
		return;
	}
	if (checkColliderWithPlayer(player)) {
		(*gScore) += 100;
		gFree();
		return;
	}
	Draw(renderer, clip, angle, center, flip);
	circleCollider.y += (int)moveSpeed;
}

void gFruit::resetMoveSpeed() {
	moveSpeed = minMoveSpeed;
}

void gFruit::gFree() {
	circleCollider.x = -gTexture::getTextureWitdh();
	circleCollider.y = 0;
	circleCollider.r = 0;
}

gFruit::~gFruit(){
}
