#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <time.h>
#include <sstream>
#include "gTexture.h"
#include "gPlayer.h"
#include "gFruit.h"
#include "Utilities.h"

using namespace std;

enum gameStates {
	MainMenu,
	Playing,
	EndScreen,
	Options
};

bool gInit(SDL_Window **window, SDL_Renderer **renderer) {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "Error initing sdl " << SDL_GetError() << endl;
		success = false;
		cin.get();
		return 0;
	}
	else {
		(*window) = SDL_CreateWindow("Background", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wWidth, wHeight, SDL_WINDOW_SHOWN);
		if (!(*window)) {
			cout << "Error creating window " << SDL_GetError() << endl;
			cin.get();
			success = false;
			return 0;
		}
		else {
			SDL_SetWindowFullscreen((*window), 0);
			(*renderer) = SDL_CreateRenderer((*window), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (!(*renderer)) {
				cout << "Creating renderer failed " << SDL_GetError() << endl;
				cin.get();
				success = false;
				return 0;
			}

			int img_flags = IMG_INIT_PNG;
			if (!IMG_Init(img_flags) & img_flags) {
				cout << "Image init failed " << IMG_GetError() << endl;
				cin.get();
				success = false;
				return 0;
			}

			if (Mix_OpenAudio(44200, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
				cout << "Mixer could not be create " << Mix_GetError() << endl;
				cin.get();
				success = false;
			}

			if (TTF_Init() == -1) {
				cout << "TTF could not be initialized " << TTF_GetError() << endl;
				cin.get();
				success = false;
			}
		}
	}
	return success;
}

void loadMedia(SDL_Renderer *renderer, gTexture **background, gPlayer *player, Mix_Music **backMusic, Mix_Music **menuMusic,
	gFruit **lollipopFruits, gFruit **mmFruits, TTF_Font **font, TTF_Font **fpsFont, gTexture **buttons,
	gTexture *tFrame, Mix_Chunk **menuPressButton, Mix_Chunk **menuHoverButton, gTexture *lives, gTexture *score) {
	(*background) = new gTexture[5];
	(*lollipopFruits) = new gFruit[9];
	(*mmFruits) = new gFruit[9];
	
	(*background)[0].loadTexture(renderer, "images/background/background.png");
	(*background)[1].loadTexture(renderer, "images/background/trees_and_bushes.png");
	(*background)[2].loadTexture(renderer, "images/background/ground.png");
	(*background)[3].loadTexture(renderer, "images/background/clouds.png");
	(*background)[4].loadTexture(renderer, "images/background/distant_clouds.png");

	for (int i = 0; i < 5; i++) {
		(*background)[i].setSize(wWidth, wHeight);
	}

	(*player).loadTexture(renderer, "images/player/professor.png");
	(*player).setPosition(playerStartingPosX, playerStartingPosY);
	(*player).setSize(playerWidth, playerHeight);

	(*lollipopFruits)[0].loadTexture(renderer, "images/fruits/lollipop_blue.png");
	(*lollipopFruits)[1].loadTexture(renderer, "images/fruits/lollipop_green.png");
	(*lollipopFruits)[2].loadTexture(renderer, "images/fruits/lollipop_orange.png");
	(*lollipopFruits)[3].loadTexture(renderer, "images/fruits/lollipop_pink.png");
	(*lollipopFruits)[4].loadTexture(renderer, "images/fruits/lollipop_purple.png");
	(*lollipopFruits)[5].loadTexture(renderer, "images/fruits/lollipop_rainbow.png");
	(*lollipopFruits)[6].loadTexture(renderer, "images/fruits/lollipop_red.png");
	(*lollipopFruits)[7].loadTexture(renderer, "images/fruits/lollipop_teal.png");
	(*lollipopFruits)[8].loadTexture(renderer, "images/fruits/lollipop_yellow.png");

	(*mmFruits)[0].loadTexture(renderer, "images/fruits/mm_blue.png");
	(*mmFruits)[1].loadTexture(renderer, "images/fruits/mm_brown.png");
	(*mmFruits)[2].loadTexture(renderer, "images/fruits/mm_green.png");
	(*mmFruits)[3].loadTexture(renderer, "images/fruits/mm_orange.png");
	(*mmFruits)[4].loadTexture(renderer, "images/fruits/mm_purple.png");
	(*mmFruits)[5].loadTexture(renderer, "images/fruits/mm_red.png");
	(*mmFruits)[6].loadTexture(renderer, "images/fruits/mm_teal.png");
	(*mmFruits)[7].loadTexture(renderer, "images/fruits/mm_yellow.png");

	(*font) = TTF_OpenFont("fonts/base05.ttf", fontSize);
	(*fpsFont) = TTF_OpenFont("fonts/OpenSansR.ttf", fontSize * 2/3);

	(*buttons) = new gTexture[9];
	for (int i = 0; i < 3; i++) {
		(*buttons)[i].setPosition(wWidth / 7, (6 + i * 3) * wHeight / 18);
		(*buttons)[i + 3].setPosition(wWidth / 7, (6 + i * 3) * wHeight / 18);
		(*buttons)[i + 6].setPosition(wWidth / 7, (6 + i * 3) * wHeight / 18);
	}

	(*buttons)[0].loadTextureFromText(renderer, *font, "PLAY");
	(*buttons)[1].loadTextureFromText(renderer, *font, "OPTIONS");
	(*buttons)[2].loadTextureFromText(renderer, *font, "QUIT");
	(*buttons)[3].loadTextureFromText(renderer, *font, "PLAY", {122, 0, 0, 0});
	(*buttons)[4].loadTextureFromText(renderer, *font, "OPTIONS", { 122, 0, 0, 0 });
	(*buttons)[5].loadTextureFromText(renderer, *font, "QUIT", { 122, 0, 0, 0 });
	(*buttons)[6].loadTextureFromText(renderer, *font, "PLAY", { 255,255,255,255 }, { 255,0,0,255 });
	(*buttons)[7].loadTextureFromText(renderer, *font, "OPTIONS", { 255,255,255,255 }, { 255,0,0,255 });
	(*buttons)[8].loadTextureFromText(renderer, *font, "QUIT", { 255,255,255,255 }, { 255,0,0,255 });

	tFrame->setPosition((23 * wWidth) / 24, 1 / 24 * wHeight);
	tFrame->setSize(wWidth / 18, wHeight / 13);

	lives->setPosition(wWidth / 12, wHeight / 12);
	lives->setSize(wWidth / 10, wHeight / 6);

	score->setPosition(lives->getX() + lives->getTextureWitdh() + 100, wHeight / 12);
	score->setSize(wWidth / 10, wHeight / 6);

	(*backMusic) = Mix_LoadMUS("sounds/backgroundPlayingMusic.ogg");
	if (!(*backMusic)) {
		cout << "Background music loading failed " << Mix_GetError() << endl;
	}
	(*menuMusic) = Mix_LoadMUS("sounds/menuPlayingMusic.ogg");
	if (!(*menuMusic)) {
		cout << "Menu music loading failed" << Mix_GetError() << endl;
	}
	(*menuPressButton) = Mix_LoadWAV("sounds/menuPressButton.wav");
	if (!(*menuPressButton)) {
		cout << "menu press sound loading failed " << Mix_GetError() << endl;
	}
	(*menuHoverButton) = Mix_LoadWAV("sounds/menuHoverButton.wav");
	if (!(*menuHoverButton)) {
		cout << "menu hover button loading failed " << Mix_GetError() << endl;
	}
}

void loadMenu(SDL_Renderer *renderer, gTexture *buttons) {
	for (int i = 0; i < 3; i++)
		buttons[i].Draw(renderer);
}

void loadGame(SDL_Renderer *renderer, gTexture *background, gPlayer *player, gFruit **candy, gFruit *lollipopFruits, gFruit *mmFruits,
	Uint32 *difficulty, Uint32 *timer1, bool (*spawn), int *lives, int *gScore, int *helper, Uint32 *startTime) {

	if ((*gScore) >= 500 * (*helper)) {
		if((*candy)->getMoveSpeed() <= maxMoveSpeed)
			(*candy)->increaseMoveSpeed();
		(*helper)++;
	}

	for (int i = 1; i < 5; i++)
		background[i].Draw(renderer);
	player->DrawCollider(renderer);
	player->Move(renderer);
	

	Uint32 timer = SDL_GetTicks() - (*timer1);
	Uint32 diffTimer = SDL_GetTicks() - (*startTime);

	if ((*difficulty) >= 2700 && diffTimer > (10000 * (Uint32)(*helper))) {
		(*difficulty) -= 100;
		(*startTime) += diffTimer;
	}

	if (timer > (*difficulty) || (*spawn) == true) {
		(*timer1) += timer;

		(*candy)->gFree();
		
		int x = rand() % 2;
		if (x == 0) {
			int temp = rand() % 9;
			(*candy) = new gFruit(lollipopFruits[temp].getTexture(), rand() % (wWidth - lollipopFruits[temp].getTextureWitdh()),
				-lollipopFruits[temp].getTextureHeight(), lollipopFruits[temp].getTextureWitdh(), lollipopFruits[temp].getTextureHeight());
		}
		else if (x == 1) {
			int temp = rand() % 8;
			(*candy) = new gFruit(mmFruits[temp].getTexture(), rand() % (wWidth - lollipopFruits[temp].getTextureWitdh()),
				-mmFruits[temp].getTextureHeight(), mmFruits[temp].getTextureWitdh(), mmFruits[temp].getTextureHeight());
		}
	}
	(*candy)->DrawCollider(renderer);
	(*candy)->Move(player->getPlayerCollider(), renderer, lives, gScore);
}
void loadEndScreen() {

}

void loadOptions() {

}

bool pointInsideTheBox(SDL_Point point, SDL_Rect rect) {
	bool inside = true;

	if (point.x < rect.x)
		inside = false;
	if (point.y < rect.y)
		inside = false;
	if (point.x > rect.x + rect.w)
		inside = false;
	if (point.y > rect.y + rect.w)
		inside = false;

	return inside;
}

int main(int argc, char *args[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	gTexture *background = NULL;
	gTexture *buttons = NULL;
	gTexture score, life;
	gPlayer player = gPlayer(playerStartingPosX, playerStartingPosY, playerWidth, playerHeight);
	SDL_Event ev;
	gFruit *lollipopFruits = NULL;
	gFruit *mmFruits = NULL;
	Mix_Music *backgroundMusic = NULL, *menuMusic = NULL;
	Mix_Chunk *menuPressButton = NULL, *menuHoverButton = NULL;
	TTF_Font *font = NULL, *fpsFont = NULL;
	gameStates gStates = MainMenu;
	bool run = true;
	int numberOfFruitsOnScreen = 5;
	SDL_Point mouse;
	gTexture tFrameRate;
	Uint32 framesPassed = 0, avgFPS = 0, timer = 0;
	stringstream text;
	gFruit *candy = NULL;
	int lives = initialLives, gScore = 0;
	//Seconds
	Uint32 difficulty = initialDifficulty;
	bool first = true, spawn = false;
	int helper = 1;
	Uint32 diffTimer;
	gTexture endScreen;

	srand((unsigned int)time(NULL));
	gInit(&window, &renderer);
	loadMedia(renderer, &background, &player, &backgroundMusic, &menuMusic,
		&lollipopFruits, &mmFruits, &font, &fpsFont, &buttons,
		&tFrameRate, &menuPressButton, &menuHoverButton, &life, &score);

	Mix_FadeInMusic(menuMusic, -1, 2000);
	Mix_VolumeMusic(20);
	while (run) {
		avgFPS = framesPassed / (SDL_GetTicks() / 1000.f);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		background[0].Draw(renderer, 0, 0);
		switch (gStates) {
			case Playing:
				loadGame(renderer, background, &player, &candy, lollipopFruits, mmFruits,
					&difficulty, &timer, &spawn, &lives, &gScore, &helper, &diffTimer);
				if (lives == 0)
					gStates = EndScreen;
				break;
			case Options:
				loadOptions();
				break;
			case EndScreen:
				loadEndScreen();
				background[0].Draw(renderer);
				endScreen.loadTextureFromText(renderer, fpsFont, "YOU DIED", { 0xff,0xff,0xff,0xff }, { 0xff, 0, 0, 0xff });
				endScreen.setPosition(wWidth / 3, wHeight / 3);
				endScreen.setSize(wWidth / 4, wHeight / 3);
				endScreen.Draw(renderer);
				score.Draw(renderer);
				SDL_RenderPresent(renderer);
				gStates = MainMenu;
				SDL_Delay(3000);
				break;
			case MainMenu:
				SDL_GetMouseState(&mouse.x, &mouse.y);
				loadMenu(renderer, buttons);
				if (pointInsideTheBox(mouse, buttons[0].getCollider())) {
					buttons[3].Draw(renderer);
				}
				else if (pointInsideTheBox(mouse, buttons[1].getCollider())) {
					buttons[4].Draw(renderer);
				}
				else if (pointInsideTheBox(mouse, buttons[2].getCollider())) {
					buttons[5].Draw(renderer);
				}
				break;
		}
		if (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) {
				run = false;
			}
			player.handleEvent(ev);

			if (gStates == MainMenu) {
				if (ev.type == SDL_MOUSEBUTTONDOWN) {
					if (pointInsideTheBox(mouse, buttons[0].getCollider())) {
						buttons[6].Draw(renderer);
					}
					if (pointInsideTheBox(mouse, buttons[1].getCollider())) {
						buttons[7].Draw(renderer);
					}
					if (pointInsideTheBox(mouse, buttons[2].getCollider())) {
						buttons[8].Draw(renderer);
					}
				}
				if (ev.type == SDL_MOUSEBUTTONUP) {
					SDL_GetMouseState(&mouse.x, &mouse.y);
					if (pointInsideTheBox(mouse, buttons[0].getCollider())) {
						gStates = Playing;
						player.pReset();
						lives = initialLives;
						first = true;
						gScore = 0;
						candy->resetMoveSpeed();
						timer = SDL_GetTicks();
						diffTimer = SDL_GetTicks();

						//Create candy
						int x = rand() % 2;
						if (x == 0) {
							int temp = rand() % 9;
							candy = new gFruit(lollipopFruits[temp].getTexture(), rand() % (wWidth - lollipopFruits[temp].getTextureWitdh()),
								-lollipopFruits[temp].getTextureHeight(), lollipopFruits[temp].getTextureWitdh(), lollipopFruits[temp].getTextureHeight());
						}
						else if (x == 1) {
							int temp = rand() % 8;
							candy = new gFruit(mmFruits[temp].getTexture(), rand() % (wWidth - lollipopFruits[temp].getTextureWitdh()),
								-mmFruits[temp].getTextureHeight(), mmFruits[temp].getTextureWitdh(), mmFruits[temp].getTextureHeight());
						}

						Mix_PlayChannel(-1, menuPressButton, 0);
						Mix_FadeOutMusic(500);
						Mix_FadeInMusic(backgroundMusic, -1, 500);
					}
					if (pointInsideTheBox(mouse, buttons[1].getCollider())) {
						Mix_PlayChannel(-1, menuPressButton, 0);
					}
					if (pointInsideTheBox(mouse, buttons[2].getCollider())) {
						Mix_PlayChannel(-1, menuPressButton, 0);
						run = false;
					}
				}
			}
		}

		text.str("");
		text << "FPS" << avgFPS;

		tFrameRate.loadTextureFromText(renderer, fpsFont, text.str().c_str(), { 0,0,0,0xff });
		tFrameRate.Draw(renderer);

		text.str("");
		text << "SCORE" << gScore;

		score.loadTextureFromText(renderer, fpsFont, text.str().c_str(), { 0,0,0,0xff });
		score.Draw(renderer);

		text.str("");
		text << "LIVES" << lives;

		life.loadTextureFromText(renderer, fpsFont, text.str().c_str(), { 0,0,0,0xff });
		life.Draw(renderer);

		SDL_RenderPresent(renderer);
		framesPassed++;
	}

	buttons->gFree();
	lollipopFruits->gFree();
	mmFruits->gFree();
	background->gFree();
	if(candy)
		candy->gFree();
	
	if(buttons)
		delete[]buttons;
	if(lollipopFruits)
		delete[]lollipopFruits;
	if(mmFruits)
		delete[]mmFruits;
	if(background)
		delete[]background;
	if(candy)
		delete candy;

	TTF_CloseFont(font);
	TTF_CloseFont(fpsFont);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_FreeMusic(backgroundMusic);
	Mix_FreeMusic(menuMusic);
	Mix_FreeChunk(menuPressButton);
	Mix_FreeChunk(menuHoverButton);

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	_CrtDumpMemoryLeaks();
	return 0;
}