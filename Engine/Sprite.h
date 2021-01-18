#pragma once
#include<iostream>
#include<string>
#include <SDL.h>
#include<SDL_image.h>
#include <math.h>       /* atan */
#include"Console.h"


/**
TODO
-Create a method that cuts textures(i.e. in half) without stretching or compressing the image
	*Create a method that will allow you to stretch and compress the cut image
*/

/** FEATURES
-changeWidth(int iW) and changeHeight(int iH): Stretching and compressing
-changeOpactity(Uint8 alpha): Changing the opacity
-changeColor(SDL_Color color): Changing the color
-rotate(double degrees): Rotates the sprite by degrees (Rotation is in a clockwise motion)
*/
class Sprite
{
public:
	Sprite();
	Sprite(SDL_Texture *textur, SDL_Renderer *ren, int width, int height);
	Sprite(SDL_Texture *textur, SDL_Renderer *ren, int width, int height, int xPos, int yPos);
	Sprite(std::string textureFileName, SDL_Renderer *ren, int width, int height, int xPos, int yPos);
	Sprite(std::string textureFileName, SDL_Renderer *ren, int width, int height, int xPos, int yPos, Console *devConsole);
	~Sprite();

	//Initializer
	void initializeRect();
	void initializeTexture(SDL_Texture * textur);
	void initializeTexture(std::string textureFileName);

	//Setters
	bool setX(int xPos);
	bool setY(int yPos);
	bool setW(int width);
	bool setH(int height);
	bool setTexture(SDL_Texture *textur);
	bool setTexture(std::string textureFileName);
	bool setRenderer(SDL_Renderer* ren);
	void setDeveloperConsole(Console *devConsole);

	//Getters
	int getX();
	int getY();
	int getH();
	int getW();
	int getOpacity();
	SDL_Texture *getTexture();

	//Render Function
	void render();
	void renderAt(int xPos, int yPos);

	//Color and Opacity Functions
	void changeOpactity(Uint8 alpha);
	void changeColor(SDL_Color color);
	void changeWidth(int iW);
	void changeHeight(int iH);
	void rotate(double degrees);

	//Console Functions
	void sendLocationToConsole();

private:
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	SDL_Rect rect;

	int x, y, w, h, mAlpha, mDegrees = 0;

	Console *developerConsole;

};

