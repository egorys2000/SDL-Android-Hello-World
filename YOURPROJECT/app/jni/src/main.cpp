#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <string.h>

//Texture wrapper class
class Texture
{
public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadTextureFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Creates blank texture
	bool createBlank(int width, int height, SDL_TextureAccess access);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Pixel accessors
	Uint32* getPixels32();
	Uint32 getPixel32(Uint32 x, Uint32 y);
	Uint32 getPitch32();
	void copyRawPixels32(void* pixels);
	bool lockTexture();
	bool unlockTexture();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Surface pixels
	SDL_Surface* mSurfacePixels;

	//Raw pixels
	void* mRawPixels;
	int mRawPitch;

	//Image dimensions
	int mWidth;
	int mHeight;
};

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

Texture::Texture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	mSurfacePixels = NULL;
	mRawPixels = NULL;
	mRawPitch = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadTextureFromFile(std::string path)
{
    //Get rid of preexisting texture
    free();

    mTexture = IMG_LoadTexture(gRenderer, path.c_str());
    SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);

	//Return success
	return mTexture != NULL;
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(mTexture == NULL)
		{
			SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
	}

bool Texture::createBlank(int width, int height, SDL_TextureAccess access)
{
	//Get rid of preexisting texture
	free();

	//Create uninitialized texture
	mTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
	if(mTexture == NULL)
	{
		SDL_Log("Unable to create streamable blank texture! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		mWidth = width;
		mHeight = height;
	}

	return mTexture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if(mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}

	//Free surface if it exists
	if(mSurfacePixels != NULL)
	{
		SDL_FreeSurface(mSurfacePixels);
		mSurfacePixels = NULL;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	if(SDL_SetTextureBlendMode(mTexture, blending) == -1)
    {
        SDL_Log("SDL_SetTextureBlendMode failed! SDL Error: %s\n", SDL_GetError());
    }
}

void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	//Set clip rendering dimensions
	if(clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

Uint32* Texture::getPixels32()
{
	Uint32* pixels = NULL;

	if(mSurfacePixels != NULL)
	{
		pixels =  static_cast<Uint32*>(mSurfacePixels->pixels);
	}

	return pixels;
}

Uint32 Texture::getPixel32(Uint32 x, Uint32 y)
{
	//Convert the pixels to 32 bit
	Uint32* pixels = static_cast<Uint32*>(mSurfacePixels->pixels);

	//Get the pixel requested
	return pixels[(y * getPitch32()) + x];
}

Uint32 Texture::getPitch32()
{
	Uint32 pitch = 0;

	if(mSurfacePixels != NULL)
	{
		pitch = mSurfacePixels->pitch / 4;
	}

	return pitch;
}

bool Texture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if(mRawPixels != NULL)
	{
		SDL_Log("Texture is already locked!\n");
		success = false;
	}
	//Lock texture
	else
	{
		if(SDL_LockTexture(mTexture, NULL, &mRawPixels, &mRawPitch) != 0)
		{
			SDL_Log("Unable to lock texture! %s\n", SDL_GetError());
			success = false;
		}
	}

	return success;
}

bool Texture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if(mRawPixels == NULL)
	{
		SDL_Log("Texture is not locked!\n");
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture(mTexture);
		mRawPixels = NULL;
		mRawPitch = 0;
	}

	return success;
}

void Texture::copyRawPixels32(void* pixels)
{
	//Texture is locked
	if(mRawPixels != NULL)
	{
		//Copy to locked pixels
		memcpy(mRawPixels, pixels, mRawPitch * mHeight);
	}
}

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
	if(!gPictureTexture.loadTextureFromFile("thumb.png"))
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
        if(!gTextTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
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
					gTextTexture.loadFromRenderedText("Hello Portrait Mode!", SDL_Color{0, 0, 255, 255});
				}
				else
				{
					gTextTexture.loadFromRenderedText("Hello Landscape Mode!", SDL_Color{0, 255, 0, 255});
				}

                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                gPictureTexture.render((gScreenRect.w - gPictureTexture.getWidth()) / 2, gScreenRect.h - gPictureTexture.getHeight());

                gTextTexture.render((gScreenRect.w - gTextTexture.getWidth()) / 2, (gScreenRect.h - gTextTexture.getHeight()) / 2);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
