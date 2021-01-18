#include "Utility.h"

Utility::Utility()
{
	screenHeight = 0;
	screenWidth = 0;

	screenShotCounter = 0;
}

Utility::Utility(int w, int h)
{
	screenHeight = h;
	screenWidth = w;

	screenShotCounter = 0;
}


Utility::~Utility()
{
}

void Utility::setHeight(int h)
{
	screenHeight = h;
}

void Utility::setWidth(int w)
{
	screenWidth = w;
}

void Utility::setWindowSize(int h, int w)
{
	SDL_SetWindowSize(window, h, w);
}

int Utility::getHeight()
{
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	return screenHeight;
}

int Utility::getWidth()
{
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	return screenWidth;
}

SDL_Renderer * Utility::getRenderer()
{
	return renderer;
}

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void Utility::logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/*
* Saves a screenshot of the windows "output" into a file called "Screenshot_???"
*/
bool Utility::captureScreenshot()
{
	std::string fileName = std::string("Screenshot_") + std::to_string(screenShotCounter) + std::string(".bmp");
	screenShotCounter++;

	SDL_Surface *sshot = SDL_CreateRGBSurface(0, getWidth(), getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, fileName.c_str());
	SDL_FreeSurface(sshot);
	return true;
}

int Utility::createWindow()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	window = SDL_CreateWindow("WINDOW NAME", 100, 50, screenWidth,
		screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		SDL_Quit();
		return 1;
	}
}

//Delletes all allocated memory of the window
void Utility::close()
{
	//Deallocate surface
	//	SDL_FreeSurface(gHelloWorld);
	//	gHelloWorld = NULL;

	//Destroy Renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}