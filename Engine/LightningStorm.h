#pragma once

#include"Lightning.h"

class LightningStorm
{
public:
	LightningStorm();
	LightningStorm(SDL_Renderer* ren, int* elapsedTime);
	~LightningStorm();


	void stormStartingAtEndingAtY(int x0, int y0, int y1);
	void generateBoltsStartingAtEndingAt(int x0, int y0, int y1);
	void render();

private:

	int numberOfBolts;
	int* elapsedRunTime;
	SDL_Renderer *renderer;

	bool renderStorm;

	static const int MIN_BOLTS = 1;
	static const int MAX_BOLTS = 15;
	Lightning bolt[MAX_BOLTS];

	double LightningSpreadFactor;

	double timeBetweenStrikes;
	double timeOfLastStrike;
	double timeSinceStrike;
	double lightningLifetime;

};