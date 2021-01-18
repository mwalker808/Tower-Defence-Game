#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include<string>

class Text
{
public:
	Text();
	Text(SDL_Renderer *ren, std::string fileNameOfFont, int sizeOfFont, SDL_Color color, std::string textToDisplay);
	~Text();

	//Initializers
	void initializeTexture();
	void initializeSurface();

	//Setters
	void setFont(std::string fileNameOfFont, int sizeOfFont);
	void setSurface();
	void setTexture();
	void setRect();
	void setX(int x);
	void setY(int y);
	void setPosition(int x, int y);
	void setOutputText(std::string newText);
	void setTextColor(SDL_Color color);
	void setOpacity(int o);
	void setOutputTextAndColor(std::string newText, SDL_Color color);

	//Getters
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	std::string getOutputText();

	void initialize();
	void render();
	void renderAt(int x, int y);


private:
	int xpos, ypos;
	TTF_Font *mFont = NULL;
	std::string fontFileName;
	std::string outputText = "Default Text";
	SDL_Color textColor;  // this is the color in rgb format, default is black
	int opacity;
	double fontSize;
	SDL_Rect Message_rect;
	SDL_Surface* surfaceMessage;
	SDL_Texture* texture;
	SDL_Renderer *renderer;



};

