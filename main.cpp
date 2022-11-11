#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <math.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

class Texture
{
	public:
		Texture();

		~Texture();

		bool loadFromFile(std::string path);
		
		void free();
		
		void render(int x, int y, double angle = 0.0);

		int getWidth();
		int getHeight();

	private:
		SDL_Texture* playerTexture;

		int width;
		int height;
};

class Player
{
    public:
		static const int PLAYER_WIDTH = 50;
		static const int PLAYER_HEIGHT = 50;

		static const int PLAYER_SPEED = 4;

		Player();

		void handleEvent(SDL_Event& e);

		void move();

		void render();

    private:
		int positionX, positionY;

		int velocityX, velocityY;
};

Texture playerTexture;

Texture::Texture()
{
	playerTexture = nullptr;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(std::string path)
{
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == nullptr)
	{
		printf("Unable to load image!");
		return false;
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		
		width = loadedSurface->w;
		height = loadedSurface->h;

		SDL_FreeSurface(loadedSurface);
	}

	playerTexture = newTexture;
	return true;
}


void Texture::free()
{
	if (playerTexture != nullptr)
	{
		SDL_DestroyTexture(playerTexture);
		playerTexture = nullptr;
		width = 0;
		height = 0;
	}
}


void Texture::render(int x, int y, double angle)
{
	SDL_Rect renderQuad = {x, y, width, height};

	SDL_RenderCopyEx(renderer, playerTexture, nullptr, &renderQuad, angle, nullptr, SDL_FLIP_NONE);
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}


Player::Player()
{
    positionX = 0;
    positionY = 0;

    velocityX = 0;
    velocityY = 0;
}

void Player::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_UP: velocityY -= PLAYER_SPEED; break;
            case SDLK_DOWN: velocityY += PLAYER_SPEED; break;
            case SDLK_LEFT: velocityX -= PLAYER_SPEED; break;
            case SDLK_RIGHT: velocityX += PLAYER_SPEED; break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_UP: velocityY += PLAYER_SPEED; break;
            case SDLK_DOWN: velocityY -= PLAYER_SPEED; break;
            case SDLK_LEFT: velocityX += PLAYER_SPEED; break;
            case SDLK_RIGHT: velocityX -= PLAYER_SPEED; break;
        }
    }
}

void Player::move()
{
	if (velocityX != 0 && velocityY != 0)
	{
		int speedX = (int)round(velocityX / pow(2.0,0.5));
		int speedY = (int)round(velocityY / pow(2.0,0.5));
		positionX += speedX;
		positionY += speedY;
				
		if ((positionX < 0 ) || (positionX + PLAYER_WIDTH > SCREEN_WIDTH))
		{
			positionX -= speedX;
		}


		if ((positionY < 0) || (positionY + PLAYER_HEIGHT > SCREEN_HEIGHT))
		{
			positionY -= speedY;
		}
	}
	else
	{
		positionX += velocityX;
		positionY += velocityY;
		
		if ((positionX < 0 ) || (positionX + PLAYER_WIDTH > SCREEN_WIDTH))
		{
			positionX -= velocityX;
		}


		if ((positionY < 0) || (positionY + PLAYER_HEIGHT > SCREEN_HEIGHT))
		{
			positionY -= velocityY;
		}
	}
}

void Player::render()
{
	playerTexture.render(positionX, positionY);
}


void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	
	window = SDL_CreateWindow("Box", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	IMG_Init(IMG_INIT_PNG);
}

void close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;
	
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia()
{
	if  (!playerTexture.loadFromFile( "box.bmp"))
	{
		printf( "Failed to load player texture!\n" );
		return false;
	}

	return true;
}

int main(int argv, char* args[]) 
{
	init();
	bool quit = false;
	
	SDL_Event e;
	
	Player player;
	
	if (!loadMedia()) 
	{
		return -1;
	}

	while(!quit)
	{
		while( SDL_PollEvent( &e ) != 0 )
		{		
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}		
			player.handleEvent( e );			
		}

			player.move();

			SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( renderer );
			
			player.render();
			
			SDL_RenderPresent(renderer);
	}
	
	close();
	return 0;
}	