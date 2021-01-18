#pragma once
#include"Line.h"
#include<math.h>
#include<vector>
#include<random>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include"AnimatedSprite.h"

class Lightning
{
public:
	Lightning();
	Lightning(SDL_Renderer* ren, int* elapsedTime);
	~Lightning();

	//Setters
	void setLines();

	//Getters
	Line getLines(int i);
	const int getSIZE();

	void createPoints();
	void createPointsStartingAt(int x0, int y0);
	void createPointsStartingAtEndingAt(int x0, int y0, int x1, int y1);
	void generateSingleLightning();
	void generateSingleLightningAt(int x0, int y0);
	void generateSingleLightingStartingAtEndingAt(int x0, int y0, int x1, int y1);
	void generateMultipleLightingStartingAtEndingAt(int x0, int y0, int x1, int y1, int n);
	void renderLightning();

//	RainbowFlash colors;


	void dimLightning();
	void unDimLightning();
	void singularStorm();
	void singularStormAt(int x, int y);


private:
	SDL_Renderer* renderer;

	Sprite jointGlow;
	Sprite jointPoint;
	AnimatedSprite sparkAnimation;

	static const int SIZE = 15;
	std::vector<Line> lines;
	std::vector<std::vector<Line>> bolts;
	int x[SIZE] = { 0 };
	int y[SIZE] = { 0 };

	int* elapsedRunTime;

	double timeBetweenLightning;
	double timeOfLastLightning;
	double timeSinceLastLightning;

	int lifeTimeOfLightning;

	double timeBetweenDim;
	double timeOfLastDim;
	double timeSinceLastDim;

	int minY;

};

