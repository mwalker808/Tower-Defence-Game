#pragma once
#include<SDL.h>


/**
TODO
-
*/

/** FEATURES
-
*/
class HitBox
{
public:
	HitBox();
	HitBox(SDL_Renderer *ren, int x0, int y0, int w0, int h0);
	HitBox(SDL_Renderer *ren, SDL_Rect rectanle);
	~HitBox();

	//Setters
	void setX(int x0);
	void setY(int y0);
	void setH(int h0);
	void setW(int w0);

	//Getters
	int getX();
	int getY();
	int getW();
	int getH();
	int getCenterX();
	int getCenterY();
	int* getCenterXptr();
	int* getCenterYptr();

	//Collision detection
	bool collisionCheck(HitBox hb);
	bool futureCollisionCheck(HitBox hb, int dX, int dY);

	//Motion Functions
	void moveTo(int x0, int y0);
//	void moveBy(int dX, int dY);


	//Render Functions
	void render();

private:
	SDL_Renderer *renderer;
	SDL_Rect rect;

	bool isCollided = false;
	bool leftObsticle = false, rightObsticle = false, upObsticle = false, downObsticle = false;
	int centerX, centerY;
};

