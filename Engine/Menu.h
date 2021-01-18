#pragma once
#include"Button.h"

class Menu
{
public:
	Menu();
	Menu(SDL_Renderer* ren, Sprite bg, int xPos, int yPos, int width, int height);
	~Menu();

	void setX(int xPos);
	void setY(int yPos);
	void setW(int width);
	void setH(int height);

	void addChoice(AnimatedSprite aSprite, std::string choiceName);
	void addText();

	void handleEvents();
	void update();
	void render();

private:
	SDL_Renderer* renderer;
	Button buttons;
	std::vector<AnimatedSprite> animatedSprites;

	Sprite backGround;
	SDL_Rect menuRect;
	int nButtons;
	int x, y, w, h;

};

