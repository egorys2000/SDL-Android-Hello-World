#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "texture.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Screen dimensions
SDL_Rect gScreenRect = {0, 0, 320, 240};

//Scene textures
Texture gTextTexture;
Texture gPictureTexture;

//Globally used font
TTF_Font* gFont = NULL;

const int FONT_SIZE = 50;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			SDL_Log("Warning: Linear texture filtering not enabled!");
		}

        //Get device display mode
        SDL_DisplayMode displayMode;
        if(SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
        {
            gScreenRect.w = displayMode.w;
            gScreenRect.h = displayMode.h;
       }

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if(gWindow == NULL)
		{
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL)
			{
				SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
				
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					SDL_Log("SDL_image could not initialize! %s\n", IMG_GetError());
					success = false;
				}

                 //Initialize SDL_ttf
                if(TTF_Init() == -1)
                {
                    SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    //Load splash textures
	if(!gPictureTexture.loadTextureFromFile(gRenderer, "thumb.png"))
	{
		SDL_Log("Failed to load picture! Error: %s\n", IMG_GetError());
		success = false;
	}
    gPictureTexture.setBlendMode(SDL_BLENDMODE_BLEND);

    //Open the font
    gFont = TTF_OpenFont("SalemErgotism.ttf", FONT_SIZE);
    if(gFont == NULL)
    {
        SDL_Log("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = {0, 0, 0};
        if(!gTextTexture.loadFromRenderedText(gRenderer, gFont, "The quick brown fox jumps over the lazy dog", textColor))
        {
            SDL_Log("Failed to render text texture!\n");
            success = false;
        }
    }

	return success;
}

void close()
{
	//Free loaded textures
    gTextTexture.free();
    gPictureTexture.free();

    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
    TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if(!init())
	{
		SDL_Log("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if(!loadMedia())
		{
			SDL_Log("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while(!quit)
			{
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}
					//Window event
					else if(e.type == SDL_WINDOWEVENT)
					{
						//Window resize/orientation change
						if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						{
							//Get screen dimensions
							gScreenRect.w = e.window.data1;
							gScreenRect.h = e.window.data2;
							
							//Update screen
							SDL_RenderPresent(gRenderer);
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render splash
				if(gScreenRect.h >= gScreenRect.w)
				{
					gTextTexture.loadFromRenderedText(gRenderer, gFont, "Hello Portrait Mode!", SDL_Color{0, 0, 255, 255});
				}
				else
				{
					gTextTexture.loadFromRenderedText(gRenderer, gFont, "Hello Landscape Mode!", SDL_Color{0, 255, 0, 255});
				}

                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                gPictureTexture.render(gRenderer, (gScreenRect.w - gPictureTexture.getWidth()) / 2, gScreenRect.h - gPictureTexture.getHeight());

                gTextTexture.render(gRenderer, (gScreenRect.w - gTextTexture.getWidth()) / 2, (gScreenRect.h - gTextTexture.getHeight()) / 2);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
