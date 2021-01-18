#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite()
{
	sourceIsSpriteSheet = false;
}

AnimatedSprite::AnimatedSprite(int * gTime)
{
	gameTime = gTime;
}

AnimatedSprite::AnimatedSprite(std::vector<std::string> textureFiles, SDL_Renderer * ren, int * gTime, int width, int height, int xPos, int yPos)
{
	sourceIsSpriteSheet = false;
	textureFileNames = textureFiles;
	size = textureFileNames.size();
	currentTexture = 0;
	setX(xPos);
	setY(yPos);
	setRenderer(ren);
	initializeTextures();
	setW(width);
	setH(height);

	initializeRect(); //Only use initializeRect() in the constructor
	gameTime = gTime;
}

AnimatedSprite::AnimatedSprite(std::string spriteSheetFileName, SDL_Renderer * ren, int * gTime, int xStart, int yStart, int n, int nRows, int nCols, int width, int height, int xPos, int yPos)
{	
	sourceIsSpriteSheet = true;
	textureFileNames.push_back(spriteSheetFileName);
	size = n;
	currentTexture = 0;
	setX(xPos);
	setY(yPos);
	setRenderer(ren);

	initializeTextures();
	setW(width / nCols);
	setH(height / nRows);


	initializeRect(xStart, yStart, n, nRows, nCols); //Only use initializeRect() in the constructor
	gameTime = gTime;
	timeBetweenAnimations = 100, timeOfLastAnimation = 0;
}

AnimatedSprite::AnimatedSprite(SDL_Texture * spriteSheetTexture, SDL_Renderer * ren, int * gTime, int xStart, int yStart, int n, int nRows, int nCols, int width, int height, int xPos, int yPos)
{
	sourceIsSpriteSheet = true;
//	textureFileNames.push_back(spriteSheetFileName);
	size = n;
	currentTexture = 0;
	setX(xPos);
	setY(yPos);
	setRenderer(ren);

	textures.push_back(spriteSheetTexture);
	setW(width / nCols);
	setH(height / nRows);

	initializeRect(xStart, yStart, n, nRows, nCols); //Only use initializeRect() in the constructor
	gameTime = gTime;
	timeBetweenAnimations = 100, timeOfLastAnimation = 0;
}

AnimatedSprite::~AnimatedSprite()
{
}

//Only use initializeRect() in the constructor
//Used to initialize the rect object to the origional dimensions of the sprite
void AnimatedSprite::initializeRect()
{
	rect.push_back({ 0,0,getW(),getH() });
}

void AnimatedSprite::initializeRect(int xStart, int yStart, int n, int nRows, int nCols)
{
	int tempW = getW();
	int tempH = getH();
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols && i * nCols + j < n; j++)
		{
			rect.push_back({xStart + tempW * j, yStart + tempH * i, tempW, tempH});
		}
	}
}

void AnimatedSprite::initializeTextures()
{
	SDL_Texture *temp;
	for (int i = 0; i < textureFileNames.size(); i++)
	{

		textures.push_back(temp);
		setTexture(textureFileNames[i], i);
	}
}

int AnimatedSprite::getX()
{
	return x;
}

int AnimatedSprite::getY()
{
	return y;
}

int AnimatedSprite::getH()
{
	return h;
}

int AnimatedSprite::getW()
{
	return w;
}

int AnimatedSprite::getCenterX()
{
	return getX() + getW() / 2;
}

int AnimatedSprite::getCenterY()
{
	return getY() + getH() / 2;
}

int AnimatedSprite::getCurrentTexture()
{
	return currentTexture;
}

SDL_Texture * AnimatedSprite::getTexture(int n)
{
	return textures[n];
}

SDL_Renderer * AnimatedSprite::getRenderer()
{
	return renderer;
}

int AnimatedSprite::getGameTime()
{
	return *gameTime;
}

//returns true if x is not the same as xPos and sets x equal to xPos
//Returns false if xPos is the same as x
bool AnimatedSprite::setX(int xPos)
{
	if (x != xPos)
	{
		x = xPos;
		dst.x = x;
		setCenterX();
		return true;
	}
	else
	{
		return false;
	}
}

//returns true if y is not the same as yPos and sets y equal to yPos
//Returns false if yPos is the same as y
bool AnimatedSprite::setY(int yPos)
{
	if (y != yPos)
	{
		y = yPos;
		dst.y = y;
		setCenterY();
		return true;
	}
	else
	{
		return false;
	}
}

void AnimatedSprite::setCenterX()
{
	centerX = getX() + getW() / 2;
}

void AnimatedSprite::setCenterY()
{
	centerY = getY() + getH() / 2;
}

void AnimatedSprite::setPosition(int xPos, int yPos)
{
	setX(xPos);
	setY(yPos);
}

bool AnimatedSprite::setW(int width)
{
	if (w != width)
	{
		w = width;
		dst.w = w;
		return true;
	}
	else
	{
		return false;
	}
}

bool AnimatedSprite::setH(int height)
{
	if (h != height)
	{
		h = height;
		dst.h = h;
		return true;
	}
	else
	{
		return false;
	}
}

void AnimatedSprite::setCurrentTexture(int n)
{
	currentTexture = n;
}

bool AnimatedSprite::setTexture(std::string textureFileName, int n)
{
	textures[n] = IMG_LoadTexture(renderer, textureFileName.c_str());
	return true;
}

bool AnimatedSprite::setRenderer(SDL_Renderer * ren)
{
	renderer = ren;
	return true;
}

void AnimatedSprite::setTimeBetweenAnimations(int tBA)
{
	timeBetweenAnimations = tBA;
}

void AnimatedSprite::animate()
{
	try {
		if ((*gameTime) > timeOfLastAnimation + timeBetweenAnimations)
		{
			if (!sourceIsSpriteSheet)
			{
				setCurrentTexture((currentTexture + 1) % textures.size());
			}
			else
			{
				setCurrentTexture((currentTexture + 1) % size);
			}
			timeOfLastAnimation = *gameTime;
		}
	}
	catch (const std::exception& ex) {
		// ...
		
	}
}

void AnimatedSprite::render()
{
	animate();
	if (!sourceIsSpriteSheet)
	{
		SDL_RenderCopyEx(renderer,
			textures[getCurrentTexture()],
			&rect[0],
			&dst,
			mDegrees,
			NULL,
			SDL_FLIP_NONE);
	}
	else
	{
		SDL_RenderCopyEx(renderer,
			textures[0],
			&rect[getCurrentTexture()],
			&dst,
			mDegrees,
			NULL,
			SDL_FLIP_NONE);
	}
}

//0 is invisible, 255 is opaque
//255 is default
void AnimatedSprite::changeOpactity(Uint8 alpha)
{
	if (mAlpha != alpha)
	{
		mAlpha = alpha;

		for (int i = 0; i < textures.size(); i++)
		{
			SDL_SetTextureBlendMode(textures[i], SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(textures[i], mAlpha);
		}
	}
}

//255 is max color
//0 is none of that color
void AnimatedSprite::changeColor(SDL_Color color)
{
	for (int i = 0; i < textures.size(); i++)
	{
		SDL_SetTextureColorMod(textures[i], color.r, color.g, color.b);
	}
}

void AnimatedSprite::changeWidth(int iW)
{
	w = iW;
}

void AnimatedSprite::changeHeight(int iH)
{
	h = iH;
}

void AnimatedSprite::rotate(double degrees)
{
	mDegrees = degrees;
}

void AnimatedSprite::rotateBy(double degrees)
{
	rotate(mDegrees + degrees);
}

void AnimatedSprite::renderAt(int xPos, int yPos)
{
	setX(xPos);
	setY(yPos);
	render();
}

void AnimatedSprite::renderCopyAt(int xPos, int yPos)
{
	int tempX = getX();
	int tempY = getY();
	renderAt(xPos, yPos);
	setPosition(tempX, tempY);
}
