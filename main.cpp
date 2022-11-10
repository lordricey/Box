#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	
	gWindow = SDL_CreateWindow("Box", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	IMG_Init(IMG_INIT_PNG);
}

void close() 
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	
	IMG_Quit();
	SDL_Quit();
}

int main(int argv, char* args[]) 
{
	init();
	bool quit = false;
	
	SDL_Event e;
	
	while(!quit)
	{
		while( SDL_PollEvent( &e ) != 0 )
		{		
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}			
		}		
	}
	close();
	return 0;
}	