#include <SDL_ttf.h>
#include <SDL_image.h>

#include "texture.h"

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

bool Texture::loadTextureFromFile(SDL_Renderer* renderer, std::string path)
{
    //Get rid of preexisting texture
    Texture::free();

    mTexture = IMG_LoadTexture(renderer, path.c_str());
    SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);

	//Return success
	return mTexture != NULL;
}

bool Texture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font , std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if(textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
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

bool Texture::createBlank(SDL_Renderer* renderer, int width, int height, SDL_TextureAccess access)
{
	//Get rid of preexisting texture
	free();

	//Create uninitialized texture
	mTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
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

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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
	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
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