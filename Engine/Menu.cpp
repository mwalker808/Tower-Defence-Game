#include "Menu.h"



Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::setX(int xPos)
{
	x = xPos;
	menuRect.x = x;
}

void Menu::setY(int yPos)
{
	y = yPos;
	menuRect.y = y;
}

void Menu::setW(int width)
{
	w = width;
	menuRect.w = w;
}

void Menu::setH(int height)
{
	h = height;
	menuRect.h = height;
}

void Menu::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &menuRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
