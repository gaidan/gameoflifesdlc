#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void drawSquare(SDL_Renderer* renderer, int x, int y) {
	for(int i = 0; i < 8; i++) {
		for(int n = 0; n < 8; n++) {
			SDL_RenderDrawPoint(renderer, x+i, y+n);
		}
	}
}

void highlightPixel(SDL_Renderer* renderer, int x, int y) {
	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	drawSquare(renderer, x, y);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void drawPixel(SDL_Renderer* renderer, int x, int y, int alive) {
	if(alive==1) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}
	else if(alive==0){
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
	drawSquare(renderer, x, y);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

int main() {
	int w = 600;
	int h = 600;
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("Game of Life",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w, h, 0);
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	int map[75][75];
	for(int i = 0; i < 75; i++) {
		for(int n = 0; n < 75; n++) {
			map[i][n] = 0;
		}
	}
	int mode = 0;
	int running = 0;
	while(!running) {
		if(mode==0) {
			SDL_Event event;
			int mousex, mousey;
			SDL_GetMouseState(&mousex, &mousey);
			if(mousex%8 != 0) {
				mousex-=(mousex%8);
			}
			if(mousey%8 != 0) {
				mousey-=(mousey%8);
			}
			while (SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) {
					running = 1;
					break;
				}
				if(event.type == SDL_MOUSEBUTTONDOWN) {
					mousey /= 8;
					mousex /= 8;
					if(map[mousey][mousex] == 0) {
						map[mousey][mousex] = 1;
					}
					else {
						map[mousey][mousex] = 0;
					}
				}
				if(event.type == SDL_KEYDOWN) {
					if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
						mode = 1;
					}
				}
			}
			SDL_RenderClear(rend);
			for(int i = 0; i < 75; i++) {
				for(int n = 0; n < 75; n++) {
					drawPixel(rend, i*8, n*8, map[n][i]);
				}
			}
			highlightPixel(rend, mousex, mousey);
			SDL_RenderPresent(rend);
			SDL_Delay(1000/60);
		}
		if (mode==1) {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) {
					running = 1;
					break;
				}
				if(event.type == SDL_KEYDOWN) {
					if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                                                mode = 0;
                                        }
				}
			}
			SDL_RenderClear(rend);
			int tempmap[75][75];
			memcpy(tempmap, map, sizeof(map));
			int neighbours = 0;
			for (int i = 0; i < 75; i++) {
				for (int n = 0; n < 75; n++) {
					neighbours = 0;
					if(map[i-1][n-1] == 1) {
						neighbours += 1;
					}
					if(map[i-1][n] == 1) {
						neighbours += 1;
					}
					if(map[i-1][n+1] == 1) {
						neighbours += 1;
					}
					if(map[i][n-1] == 1) {
						neighbours += 1;
					}
					if(map[i][n+1] == 1) {
						neighbours += 1;
					}
					if(map[i+1][n-1] == 1) {
						neighbours += 1;
					}
					if(map[i+1][n] == 1) {
						neighbours += 1;
					}
					if(map[i+1][n+1] == 1) {
						neighbours += 1;
					}
					if(map[i][n] == 1) {
						if(neighbours >= 2 && neighbours <= 3) {
							tempmap[i][n] = 1;
						}
						else {
							tempmap[i][n] = 0;
						}
					}
					if(map[i][n] == 0) {
						if(neighbours == 3) {
							tempmap[i][n] = 1;
						}
					}
				}
			}
			memcpy(map, tempmap, sizeof(tempmap));
			for(int i = 0; i<75; i++) {
				for (int n = 0; n < 75; n++) {
					drawPixel(rend, i*8, n*8, map[n][i]);
				}
			}
			SDL_RenderPresent(rend);
			SDL_Delay(1000/60);
		}
	}
}
