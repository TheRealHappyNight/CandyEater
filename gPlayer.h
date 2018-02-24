#pragma once
#include "gTexture.h"

enum playerStates {
	pIdle = 0,
	pWalkingLeft = 1,
	pWalkingRight = 2,
	pTotalStates = 3
};

class gPlayer
{
public:
	gPlayer(int sX = 0, int sY = 0, int width = 0, int height = 0);
	void handleEvent(SDL_Event &ev);
	void Move(SDL_Renderer *renderer);
	void Draw(SDL_Renderer *renderer, SDL_Rect state);
	void pReset();
	void DrawCollider(SDL_Renderer *renderer);

	void loadTexture(SDL_Renderer *renderer, std::string path) {
		pTexture.loadTexture(renderer, path);
	}
	SDL_Rect getPlayerState(int i) {
		return pStates[pActualState][i];
	}
	SDL_Rect getPlayerCollider() {
		return pCollider;
	}
	void setSize(int w, int h);
	void setPosition(int x, int y);
	~gPlayer();

private:
	const int moveSpeed = 5;
	float vel;
	gTexture pTexture;
	int frame;
	SDL_Rect pStates[3][9];
	playerStates pActualState;
	SDL_Rect pCollider;
};

