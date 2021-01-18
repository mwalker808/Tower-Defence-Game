#include "Sprite.h"

Sprite::Sprite()
{
	setX(0);
	setY(0);
	setW(0);
	setH(0);
//	setTexture(nullptr);
//	setRenderer(nullptr);
//	setDeveloperConsole(nullptr);
	initializeRect(); //Only use initialize Rect in the constructor
}

Sprite::Sprite(SDL_Texture * textur, SDL_Renderer * ren, int width, int height)
{
	setX(0);
	setY(0);
	setTexture(textur);
	setRenderer(ren);
	setW(width);
	setH(height);

	initializeRect(); //Only use initialize Rect in the constructor
}

Sprite::Sprite(SDL_Texture * textur, SDL_Renderer * ren, int width, int height, int xPos, int yPos)
{
	setX(xPos);
	setY(yPos);
	setRenderer(ren);
	setTexture(textur);
	setW(width);
	setH(height);

	initializeRect(); //Only use initializeRect() in the constructor
}

Sprite::Sprite(std::string textureFileName, SDL_Renderer * ren, int width, int height, int xPos, int yPos)
{
	setX(xPos);
	setY(yPos);
	setRenderer(ren);
	initializeTexture(textureFileName);
	setW(width);
	setH(height);
	initializeRect(); //Only use initializeRect() in the constructor
}

Sprite::Sprite(std::string textureFileName, SDL_Renderer * ren, int width, int height, int xPos, int yPos, Console *devConsole)
{
	setX(xPos);
	setY(yPos);
	setRenderer(ren);
	initializeTexture(textureFileName);
	setW(width);
	setH(height);
	initializeRect(); //Only use initializeRect() in the constructor
	setDeveloperConsole(devConsole);
}

Sprite::~Sprite()
{
}

//Only use initializeRect() in the constructor
//Used to initialize the rect object to the origional dimensions of the sprite
void Sprite::initializeRect()
{
	rect.x = 0;
	rect.y = 0;
	rect.w = getW();
	rect.h = getH();
}

void Sprite::initializeTexture(std::string textureFileName)
{
	texture = IMG_LoadTexture(renderer, textureFileName.c_str());
}

//returns true if x is not the same as xPos and sets x equal to xPos
//Returns false if xPos is the same as x
bool Sprite::setX(int xPos)
{
	if (x != xPos)
	{
		x = xPos;
		return true;
	}
	else
	{
		return false;
	}
}

//returns true if y is not the same as yPos and sets y equal to yPos
//Returns false if yPos is the same as y
bool Sprite::setY(int yPos)
{
	if (y != yPos)
	{
		y = yPos;
		return true;
	}
	else
	{
		return false;
	}
}

bool Sprite::setW(int width)
{
	if (w != width)
	{
		w = width;
		return true;
	}
	else
	{
		return false;
	}
}

bool Sprite::setH(int height)
{
	if (h != height)
	{
		h = height;
		return true;
	}
	else
	{
		return false;
	}
}

bool Sprite::setTexture(SDL_Texture * textur)
{
	if (texture != textur)
	{
		SDL_DestroyTexture(texture);
		texture = textur;
		return true;
	}
	else
	{
		return false;
	}
}

bool Sprite::setTexture(std::string textureFileName)
{	
	SDL_DestroyTexture(texture);
	texture = IMG_LoadTexture(renderer, textureFileName.c_str());
	return true;
}

//if ren is different from renderer then returns true and sets renderer equal to ren
bool Sprite::setRenderer(SDL_Renderer * ren)
{
	if (renderer != ren)
	{
		renderer = ren;
		return true;
	}
	else
	{
		return false;
	}
}

void Sprite::setDeveloperConsole(Console * devConsole)
{
	developerConsole = devConsole;
}


int Sprite::getX()
{
	return x;
}

int Sprite::getY()
{
	return y;
}

int Sprite::getH()
{
	return h;
}

int Sprite::getW()
{
	return w;
}

int Sprite::getOpacity()
{
	return mAlpha;
}

SDL_Texture * Sprite::getTexture()
{
	return texture;
}

void Sprite::render()
{
	SDL_Rect dst;
	dst.x = getX();
	dst.y = getY();
	dst.w = w;
	dst.h = h;

	SDL_RenderCopyEx(renderer,
		texture,
		&rect,
		&dst,
		mDegrees,
		NULL,
		SDL_FLIP_NONE);
}

void Sprite::renderAt(int xPos, int yPos)
{
	setX(xPos);
	setY(yPos);
	render();
}

//0 is invisible, 255 is opaque
//255 is default
void Sprite::changeOpactity(Uint8 alpha)
{
	mAlpha = alpha;
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, mAlpha);
}

//255 is the maximum input
void Sprite::changeColor(SDL_Color color)
{
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

void Sprite::changeWidth(int iW)
{
	w = iW;
	//	setTexture();
	rect.w = w;
}

void Sprite::changeHeight(int iH)
{
	h = iH;
	//	setTexture();
	rect.h = h;
}


void Sprite::sendLocationToConsole()
{
	(*developerConsole).insertMessage("This sprite is at (" + std::to_string(getX()) + std::string(", ") + std::to_string(getY()) + std::string(")"));
}

void Sprite::rotate(double degrees)
{
	mDegrees = degrees;
}