#include "Text.h"

Text::Text()
{
}

Text::Text(SDL_Renderer *ren, std::string fileNameOfFont, int sizeOfFont, SDL_Color color, std::string textToDisplay)
{
	initialize();
	xpos = 0;
	ypos = 0;

	renderer = ren;

	fontFileName = fileNameOfFont;
	fontSize = sizeOfFont;
	outputText = textToDisplay;
	textColor = color;
	opacity = 255;

	setFont(fontFileName.c_str(), fontSize);
	initializeSurface();
	initializeTexture();
	setRect();
}

Text::~Text()
{

}

void Text::initializeTexture()
{
	texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
}

void Text::initializeSurface()
{
	surfaceMessage = TTF_RenderText_Solid(mFont, outputText.c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
}

void Text::setFont(std::string fileNameOfFont, int sizeOfFont)
{
	TTF_Font *temp = NULL;

	temp = TTF_OpenFont(fileNameOfFont.c_str(), sizeOfFont);
	if (temp == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		mFont = temp;
	}
}

void Text::setSurface()
{
	SDL_FreeSurface(surfaceMessage);
	surfaceMessage = TTF_RenderText_Solid(mFont, outputText.c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
}

void Text::setTexture()
{
	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
}

void Text::setRect()
{
	Message_rect.x = xpos;  //controls the rect's x coordinate 
	Message_rect.y = ypos; // controls the rect's y coordinte
	//Checks if the outputText has a non zero size
	if (getOutputText().length() != 0)
	{
		Message_rect.w = surfaceMessage->w; // controls the width of the rect
		Message_rect.h = surfaceMessage->h; // controls the height of the rect
	}
	else
	{
		Message_rect.w = 0; // controls the width of the rect
		Message_rect.h = 0; // controls the height of the rect
	}
}

void Text::setX(int x)
{
	xpos = x;
	Message_rect.x = xpos;
}

void Text::setY(int y)
{
	ypos = y;
	Message_rect.y = ypos;
}

void Text::setPosition(int x, int y)
{
	setX(x);
	setY(y);
}

void Text::setOutputText(std::string newText)
{
	outputText = newText;
	setSurface();
	setTexture();
	setOpacity(opacity);
	setRect();
}

void Text::setTextColor(SDL_Color color)
{
	textColor = color;
	setSurface();
	setTexture();
	setOpacity(opacity);
	setRect();
}

void Text::setOpacity(int o)
{
	opacity = o;
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, opacity);
}

void Text::setOutputTextAndColor(std::string newText, SDL_Color color)
{
	setOutputText(newText);
	setTextColor(color);
}

int Text::getX()
{
	return xpos;
}

int Text::getY()
{
	return ypos;
}

int Text::getWidth()
{
	return Message_rect.w;
}

int Text::getHeight()
{
	return Message_rect.h;
}

std::string Text::getOutputText()
{
	return outputText;
}

void Text::initialize()
{
	if (TTF_Init() == -1)
	{
		std::cout << "TTF Not initialized" << std::endl;
	}
}

void Text::render()
{
	SDL_RenderCopy(renderer, texture, NULL, &Message_rect);
}

void Text::renderAt(int x, int y)
{
	setX(x);
	setY(y);
	render();
}