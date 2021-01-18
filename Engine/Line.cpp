#include "Line.h"


Line::Line()
{
	lineSprite = Sprite("./images/line.png", renderer, 13, 1, initialX, initialY);

}

Line::Line(SDL_Renderer* ren, int x0, int y0, int x1, int y1)
{
	renderer = ren;
	setInitialPoint(x0, y0);
	setFinalPoint(x1, y1);
	computeAndSetLength();
	lineSprite = Sprite("./images/line.png", renderer, 13, 1, initialX, initialY);
	computeAndSetAngle();//set angle has to come after the sprite was created
	lineSprite.changeHeight(lineLength);
	lineSprite.changeColor({ 185,222,255 });
}

Line::~Line()
{
}

Sprite * Line::getLineSprite()
{
	return &lineSprite;
}

int Line::getInitialX()
{
	return initialX;
}

int Line::getInitialY()
{
	return initialY;
}

int Line::getAngle()
{
	return angle;
}

void Line::setLineSprite(Sprite sprite)
{
	lineSprite = sprite;
}

void Line::setInitialPoint(int x, int y)
{
	initialX = x;
	initialY = y;
}

void Line::setFinalPoint(int x, int y)
{
	finalX = x;
	finalY = y;
}

void Line::computeAndSetLength()
{
	lineLength = sqrt((initialX - finalX) *(initialX - finalX) + (initialY - finalY) * (initialY - finalY));
}

void Line::computeAndSetAngle()
{
	int xDiff = initialX - finalX;
	int yDiff = initialY - finalY;
	angle = atan2(yDiff, xDiff);
	angle = (double)(angle) * 180.0 / 3.14159; //Converts the angle to degrees
	angle -= 90;
	lineSprite.rotate(angle);
}

void Line::setPosition()
{
	//double cX = (double)lineSprite.getX() - (double)(lineSprite.getW()) / 2.0;
	//double cY = (double)lineSprite.getY() - (double)lineLength / 2.0;
	double newX/* = lineSprite.getX()*/;
	double newY/* = lineSprite.getY()*/;
	newY = (double)lineSprite.getY() - (double)lineSprite.getH() / 2.0 - cos(angle * 3.14159 / 180.0) * (double)lineSprite.getH() / 2.0;
	newX = (double)lineSprite.getX() + (double(lineSprite.getH()) / 2.0) * sin(angle * 3.14159 / 180.0);
	lineSprite.setX(newX);
	lineSprite.setY(newY);
}

void Line::rotate(int angle)
{
	lineSprite.rotate(angle);
}

void Line::changePosistion(int x0, int y0, int x1, int y1)
{
	setInitialPoint(x0, y0);
	setFinalPoint(x1, y1);
	computeAndSetLength();
	computeAndSetAngle();
	lineSprite.setX(x0);
	lineSprite.setY(y0);
	lineSprite.changeHeight(lineLength);
//	lineSprite.changeColor({ 255,242,255 });
	temp = true;
}

void Line::render()
{
	if (temp)
	{
		setPosition();
		temp = false;
	}

	lineSprite.render();
}