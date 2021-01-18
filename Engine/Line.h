#pragma once
#include"Sprite.h"
#include<math.h>

class Line
{
public:
	Line();
	Line(SDL_Renderer* ren, int x0, int y0, int x1, int y1);
	~Line();

	//Getters
	Sprite* getLineSprite();
	int getInitialX();
	int getInitialY();
	int getAngle();


	//Setters
	void setLineSprite(Sprite sprite);
	void setInitialPoint(int x, int y);
	void setFinalPoint(int x, int y);
	void computeAndSetLength();
	void computeAndSetAngle();
	void setPosition();

	//Other
	void rotate(int angle);
	void changePosistion(int x0, int y0, int x1, int y1);
	void render();


private:

	SDL_Renderer* renderer;
	Sprite lineSprite;
	int initialX;
	int initialY;
	int finalX;
	int finalY;
	double lineLength;
	double angle;

	bool temp = true;
};

