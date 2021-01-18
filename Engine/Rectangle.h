#pragma once
#include"Line.h"


/**
TODO

*/

/** FEATURES
-
*/
class Rectangle
{
public:
	Rectangle();
	Rectangle(SDL_Renderer *ren, int x0, int y0, int w, int h, double angle);
	~Rectangle();

	//Initializers
	void initializeEdges();

	//
	void rotate(int degrees);

	//Render Functions
	void render();

private:
	SDL_Renderer *renderer;
	Line edges[4];
	int x, y, w, h, mDegrees;

};

