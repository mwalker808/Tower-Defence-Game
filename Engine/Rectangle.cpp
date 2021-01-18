#include "Rectangle.h"



Rectangle::Rectangle()
{
}

Rectangle::Rectangle(SDL_Renderer * ren, int x0, int y0, int w0, int h0, double angle)
{
	renderer = ren;
	x = x0;
	y = y0;
	w = w0;
	h = h0;

	initializeEdges();
}


Rectangle::~Rectangle()
{
}

void Rectangle::initializeEdges()
{
	edges[0] = Line(renderer, x, y, x, y + h);
	edges[1] = Line(renderer, x, y, x + w, y);
	edges[2] = Line(renderer, x + w, y + h, x + w, y);
	edges[3] = Line(renderer, x + w, y + h, x , y + h);
}

void Rectangle::rotate(int degrees)
{
	//edges[0].rotate(edges[0].getAngle() + degrees);
	//edges[1].rotate(edges[1].getAngle() + degrees);
	//edges[2].rotate(edges[2].getAngle() + degrees);
	//edges[3].rotate(edges[3].getAngle() + degrees);
}

void Rectangle::render()
{
	for (int i = 0; i < 4; i++)
	{
		edges[i].render();
	}
}
