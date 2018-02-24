#pragma once
#include "gTexture.h"
#include "Utilities.h"

struct Circle {
	int x, y;
	double r;
};

class gFruit :
	public gTexture
{
public:
	gFruit(SDL_Texture *t = NULL, int x = 0, int y = 0, int w = 0, int h = 0);
	//gFruit(const gFruit &obj);
	//gFruit& operator=(const gFruit &obj);
	void setPosition(int x, int y);
	Circle getCollider() {
		return circleCollider;
	}
	bool checkColliderWithPlayer(SDL_Rect pCollider);
	void Draw(SDL_Renderer* renderer, SDL_Rect *clip = NULL, float angle = 0.0f,
		SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawCollider(SDL_Renderer* renderer);
	void Move(SDL_Rect player, SDL_Renderer* renderer, int *lifes, int *gScore, SDL_Rect *clip = NULL, float angle = 0.0f,
		SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void increaseMoveSpeed() {
		moveSpeed += 0.1f;
	}
	float getMoveSpeed() {
		return moveSpeed;
	}
	void resetMoveSpeed();
	void gFree();
	~gFruit();

private:
	Circle circleCollider;
	static float moveSpeed;
		
};

