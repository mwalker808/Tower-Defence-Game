#include "HitBox.h"

HitBox::HitBox()
{
	renderer = nullptr;
	rect = { 0,0,0,0 };
}

HitBox::HitBox(SDL_Renderer * ren, int x0, int y0, int w0, int h0)
{
	renderer = ren;
	rect = {x0,y0,w0,h0};
}

HitBox::HitBox(SDL_Renderer * ren, SDL_Rect rectangle)
{
	renderer = ren;
	rect = rectangle;
}

HitBox::~HitBox()
{

}

void HitBox::setX(int x0)
{
	rect.x = x0;
	centerX = rect.x + rect.w;
}

void HitBox::setY(int y0)
{
	rect.y = y0;
	centerY = rect.y + rect.h;
}

void HitBox::setH(int h0)
{
	rect.h = h0;
}

void HitBox::setW(int w0)
{
	rect.w = w0;
}

int * HitBox::getCenterXptr()
{
	return &centerX;
}

int * HitBox::getCenterYptr()
{
	return &centerY;
}

int HitBox::getX()
{
	return rect.x;
}

int HitBox::getY()
{
	return rect.y;
}

int HitBox::getW()
{
	return rect.w;
}

int HitBox::getH()
{
	return rect.h;
}

int HitBox::getCenterX()
{
	return getX() + getW() / 2;
}

int HitBox::getCenterY()
{
	return getY() + getH() / 2;
}

bool HitBox::collisionCheck(HitBox hb)
{
	if (getX() < hb.getX() + hb.getW() &&
		getX() + getW() > hb.getX() &&
		getY() < hb.getY() + hb.getH() &&
		getH() + getY() > hb.getY())
	{
		isCollided = true;
	}
	else
	{
		isCollided = false;
	}
	return isCollided;
}

//Checks to see if a hit box will sollide with another hitbox after moving <dX, dY> away from its current position
bool HitBox::futureCollisionCheck(HitBox hb, int dX, int dY)
{
	if (getX() + dX < hb.getX() + hb.getW() &&
		getX() + dX + getW() > hb.getX() &&
		getY() + dY < hb.getY() + hb.getH() &&
		getH() + getY() + dY > hb.getY())
	{

		if (dX > 0)
		{
			rightObsticle = true;
			leftObsticle = false;
		}
		else if(dX < 0)
		{
			leftObsticle = true;
			rightObsticle = false;
		}

		if (dY > 0)
		{
			downObsticle = true;
			upObsticle = false;
		}
		else if (dY < 0)
		{
			upObsticle = true;
			downObsticle = false;
		}


		return true;
	}
	else
	{
		return false;
	}
}

void HitBox::moveTo(int x0, int y0)
{
	setX(x0);
	setY(y0);
}

//void HitBox::moveBy(int dX, int dY)
//{
//	setX(getX() + dX);
//	setY(getY() + dY);
//}

void HitBox::render()
{
	//Box is drawn as red if 
	if (isCollided)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
