#pragma once
#include <iostream>
#include<string>
#include<SDL.h>
#include<SDL_image.h>


class Utility
{
public:
	Utility();
	Utility(int w, int h);
	~Utility();

	//Setters
	void setHeight(int h);
	void setWidth(int w);
	void setWindowSize(int h, int w);

	//Getters
	int getHeight();
	int getWidth();
	SDL_Renderer* getRenderer();

	void logSDLError(std::ostream &os, const std::string &msg);
	int createWindow();
	void close();
	bool captureScreenshot();


private:
	int screenWidth, screenHeight, screenShotCounter;
	SDL_Window *window;
	SDL_Renderer *renderer;


};