#include "LightningStorm.h"



LightningStorm::LightningStorm()
{
}

LightningStorm::LightningStorm(SDL_Renderer* ren, int* elapsedTime)
{
	renderer = ren;
	elapsedRunTime = elapsedTime;
	timeOfLastStrike = 0.0;
	timeBetweenStrikes = 400;
	lightningLifetime = 100;

	renderStorm = false;

	LightningSpreadFactor = 1.5;

	for (int i = 0; i < MAX_BOLTS; i++)
	{
		bolt[i] = Lightning(renderer, elapsedRunTime);
	}
}


LightningStorm::~LightningStorm()
{
}

void LightningStorm::stormStartingAtEndingAtY(int x0, int y0, int y1)
{
	timeSinceStrike = *elapsedRunTime - timeOfLastStrike;
	if (timeSinceStrike > timeBetweenStrikes)
	{
		generateBoltsStartingAtEndingAt(x0, y0, y1);
		timeOfLastStrike = *elapsedRunTime;
	}

	if (timeSinceStrike < lightningLifetime)
	{
		renderStorm = true;
	}
	else
	{
		renderStorm = false;
	}
}

void LightningStorm::generateBoltsStartingAtEndingAt(int x0, int y0, int y1)
{
	int yDiff = y1 - y0;
	int endX = x0 + (LightningSpreadFactor*yDiff - (rand() % int(2 * LightningSpreadFactor*yDiff + 1)));
	bolt[0].generateSingleLightingStartingAtEndingAt(x0, y0, endX, y1);
	numberOfBolts = rand() % (MAX_BOLTS);
	if (numberOfBolts < MIN_BOLTS)
	{
		numberOfBolts = MIN_BOLTS;
	}
	for (int i = 1; i < numberOfBolts + 1; i++)
	{
		int randomBolt = rand() % i;
		int randomJoint = rand() % (4 * (bolt[randomBolt].getSIZE() - 1) / 5);
		yDiff = y1 - bolt[randomBolt].getLines(randomJoint).getInitialY();
		endX = x0 + (LightningSpreadFactor*yDiff - (rand() % int(2 * LightningSpreadFactor*yDiff + 1)));
		bolt[i].generateSingleLightingStartingAtEndingAt(bolt[randomBolt].getLines(randomJoint).getInitialX(), bolt[randomBolt].getLines(randomJoint).getInitialY(), endX, y1);
	}
}

void LightningStorm::render()
{
	if (renderStorm)
	{
		for (int i = 0; i < numberOfBolts; i++)
		{
			bolt[i].renderLightning();
		}
	}
}
