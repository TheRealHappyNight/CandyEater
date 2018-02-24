#include "gPlayer.h"
#include "Utilities.h"
#include <iostream>

gPlayer::gPlayer(int sX, int sY, int width, int height){
	pTexture.setPosition(sX, sY);
	pTexture.setSize(width, height);
	pActualState = pIdle;
	vel = 0.0f;
	frame = 0;
	
	pStates[pIdle][0] = { 4 * 64, 2 * 64, 64, 64 };

	for (int i = 0; i < 9; i++) {
		pStates[pWalkingLeft][i].x = i * 64;
		pStates[pWalkingLeft][i].y = 64;
		pStates[pWalkingLeft][i].w = 64;
		pStates[pWalkingLeft][i].h = 64;
		pStates[pWalkingRight][i].x = i * 64;
		pStates[pWalkingRight][i].y = 3 * 64;
		pStates[pWalkingRight][i].w = 64;
		pStates[pWalkingRight][i].h = 64;
	}

	pCollider.x = pTexture.getX() + playerColliderXdiff;
	pCollider.y = pTexture.getY() + playerColliderYdiff;
	pCollider.w = pTexture.getTextureWitdh() - playerColliderWdiff;
	pCollider.h = pTexture.getTextureHeight() - playerColliderHdiff;
}

void gPlayer::handleEvent(SDL_Event &ev) {
	if (ev.type == SDL_KEYDOWN && ev.key.repeat == 0) {
		switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
			case SDLK_a:
				vel -= moveSpeed;
				pActualState = pWalkingLeft;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				vel += moveSpeed;
				pActualState = pWalkingRight;
				break;	
		}
	}
	else if(ev.type == SDL_KEYUP && ev.key.repeat == 0) {
		switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
			case SDLK_a:
				vel += moveSpeed;
				pActualState = pIdle;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				vel -= moveSpeed;
				pActualState = pIdle;
				break;
		}
	}
}

void gPlayer::Move(SDL_Renderer *renderer) {
	pTexture.setPosition(pTexture.getX() + vel, pTexture.getY());
	pCollider.x = pTexture.getX() + playerColliderXdiff;
	
	if (pTexture.getX() + playerColliderXdiff < 0) {
		pTexture.setPosition(-playerColliderXdiff, pTexture.getY());
		pCollider.x = pTexture.getX() + playerColliderXdiff;
	}

	if (pTexture.getX() + playerColliderXdiff + pTexture.getTextureWitdh() - playerColliderWdiff > wWidth) {
		pTexture.setPosition(wWidth - pTexture.getTextureWitdh() + playerColliderXdiff, pTexture.getY());
		pCollider.x = pTexture.getX() + playerColliderXdiff;
	}

	Draw(renderer, pStates[pActualState][frame / 9]);

	if (pActualState == pWalkingLeft || pActualState == pWalkingRight)
		++frame;

	if (pActualState == pIdle || vel == 0.0f)
		frame = 0;
	
	if (frame / moveSpeed - 1 >= 9)
		frame = 0;
}

void gPlayer::Draw(SDL_Renderer *renderer, SDL_Rect state) {
	pTexture.Draw(renderer, &state);
}

void gPlayer::DrawCollider(SDL_Renderer *renderer) {
	SDL_Point *a = new SDL_Point[5];
	a[0] = { pCollider.x, pCollider.y };
	a[1] = { pCollider.x + pCollider.w, pCollider.y };
	a[2] = { pCollider.x + pCollider.w, pCollider.y + pCollider.h };
	a[3] = { pCollider.x, pCollider.y + pCollider.h };
	a[4] = a[0];
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
	if (SDL_RenderDrawLines(renderer, a, 5) < 0)
		std::cout << "Not Okay\n";
	delete a;
}

void gPlayer::pReset() {
	pTexture.setPosition(playerStartingPosX, playerStartingPosY);
	pCollider.x = pTexture.getX() + playerColliderXdiff;
	frame = 0;
	vel = 0.0f;
	pActualState = pIdle;
}

void gPlayer::setSize(int w, int h) {
	pTexture.setSize(w,h);
	pCollider.w = pTexture.getTextureWitdh() - playerColliderWdiff;
	pCollider.h = pTexture.getTextureHeight() - playerColliderHdiff;
}

void gPlayer::setPosition(int x, int y) {
	pTexture.setPosition(x, y);
	pCollider.x = pTexture.getX() + playerColliderXdiff;
	pCollider.y = pTexture.getY() + playerColliderYdiff;
}

gPlayer::~gPlayer(){
}
