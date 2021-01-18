#include "Lightning.h"

Lightning::Lightning()
{
}

Lightning::Lightning(SDL_Renderer* ren, int* elapsedTime)
{
	srand(time(NULL));

	renderer = ren;
	x[0] = 300;
	y[0] = 200;

	jointPoint = Sprite("./images/point.png", renderer, 1, 1, 1, 1);

	jointGlow = Sprite("./images/joint1.png", renderer, 29, 29, 1, 1);
	jointGlow.changeColor({ 220,200,255 });
//	jointGlow.changeOpactity(200);

	sparkAnimation = AnimatedSprite("./images/TowerDefenceSprites/EnemySprites/spark_Effect.png", renderer, elapsedTime, 0, 0, 16, 4, 4, 728, 824, 200, 100);
	sparkAnimation.setW(60);
	sparkAnimation.setH(60);
	sparkAnimation.setTimeBetweenAnimations(1);
	sparkAnimation.changeColor({255,255,255});


	elapsedRunTime = elapsedTime;

//	colors = RainbowFlash(renderer, elapsedRunTime);

	setLines();
	createPoints();

	generateSingleLightning();

	timeSinceLastLightning = 0.0;
	timeOfLastLightning = 0.0;
	timeBetweenLightning = 100;
	
	lifeTimeOfLightning = 50;
	
	timeSinceLastDim = 0.0;
	timeOfLastDim = 0.0;
	timeBetweenDim = 100;

	minY = 20;


}


Lightning::~Lightning()
{
}

void Lightning::setLines()
{
	for (int i = 0; i < SIZE - 1; i++)
	{
		lines.push_back(Line(renderer, x[i], y[i], x[i + 1], y[i + 1]));
	}
}

Line Lightning::getLines(int i)
{
	return lines[i];
}

const int Lightning::getSIZE()
{
	return SIZE;
}

void Lightning::createPoints()
{
	//	std::normal_distribution<double> xDistribution(0, 10);
	//	std::normal_distribution<double> yDistribution(20, 10);
	for (int i = 1; i < SIZE; i++)
	{
		//		x[i] = x[i - 1] + xDistribution(generator);
		//		y[i] = y[i - 1] + abs(yDistribution(generator));
		int xChange = -10 + (rand() % 20);

		x[i] = x[i - 1] + xChange;
		int yChange = rand() % 40;
		if (yChange < minY)
		{
			yChange = minY;
		}
		y[i] = y[i - 1] + yChange;
	}
}

void Lightning::createPointsStartingAt(int x0, int y0)
{
	x[0] = x0;
	y[0] = y0;
	createPoints();
}

void Lightning::createPointsStartingAtEndingAt(int x0, int y0, int x1, int y1)
{
	x[0] = x0;
	y[0] = y0;
	x[SIZE - 1] = x1;
	y[SIZE - 1] = y1;

	for (int i = 1; i < SIZE - 1; i++)
	{
		int xDiff = x1 - x[i - 1];
		int yDiff = y1 - y[i - 1];
		int xSpan = xDiff / (SIZE - 1 - i);
		int ySpan = yDiff / (SIZE - 1 - i);

		int xChange = xSpan - 5 + (rand() % 10);
		int yChange = ySpan - 5 + (rand() % 10);
		x[i] = x[i - 1] + xChange;
		y[i] = y[i - 1] + yChange;

		/*		if (distance(x[i], y[i], x[SIZE - 1], y[SIZE - 1]) > distance(x[i - 1], y[i - 1], x[SIZE - 1], y[SIZE - 1]))
		{
		int deltaX = (x[SIZE - 1] - x[i - 1]) / (SIZE - 1 - i);
		int deltaY = (y[SIZE - 1] - y[i - 1]) / (SIZE - 1 - i);
		x[i] = x[i - 1] + deltaX;
		y[i] = y[i - 1] + deltaY;
		}
		*/
		if (sqrt((x[i] - x1)*(x[i] - x1) + (y[i] - y1)*(y[i] - y1)) <= sqrt(xSpan*xSpan + ySpan*ySpan) * 2.00)//checks to see if the generated point is next to the end point, then sets all remaining points to the end point
		{
			for (int k = 0; i + k < SIZE - 1; k++)
			{
				x[i + k] = x1;
				y[i + k] = y1;
			}
			i = SIZE - 1;
		}
	}

}

void Lightning::generateSingleLightning()
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].changePosistion(x[i], y[i], x[i + 1], y[i + 1]);
	}
}

void Lightning::generateSingleLightningAt(int x0, int y0)
{
	timeSinceLastLightning = *elapsedRunTime - timeOfLastLightning;
	if (timeSinceLastLightning > timeBetweenLightning)
	{
		createPointsStartingAt(x0, y0);
		for (int i = 0; i < lines.size(); i++)
		{
			lines[i].changePosistion(x[i], y[i], x[i + 1], y[i + 1]);
		}
		timeOfLastLightning = *elapsedRunTime;
	}
}

void Lightning::generateSingleLightingStartingAtEndingAt(int x0, int y0, int x1, int y1)
{
	timeSinceLastLightning = *elapsedRunTime - timeOfLastLightning;
	if (timeSinceLastLightning > timeBetweenLightning)
	{
		createPointsStartingAtEndingAt(x0, y0, x1, y1);
		for (int i = 0; i < lines.size(); i++)
		{
			lines[i].changePosistion(x[i], y[i], x[i + 1], y[i + 1]);
		}
		timeOfLastLightning = *elapsedRunTime;
	}
}

void Lightning::generateMultipleLightingStartingAtEndingAt(int x0, int y0, int x1, int y1, int n)
{
	timeSinceLastLightning = *elapsedRunTime - timeOfLastLightning;
	if (timeSinceLastLightning > timeBetweenLightning)
	{
		bolts.clear();
		for (int j = 0; j < n; j++)
		{
			createPointsStartingAtEndingAt(x0, y0, x1, y1);
			for (int i = 0; i < lines.size(); i++)
			{
				lines[i].changePosistion(x[i], y[i], x[i + 1], y[i + 1]);
			}
			bolts.push_back(lines);
		}
		timeOfLastLightning = *elapsedRunTime;
	}
}

void Lightning::renderLightning()
{
	//	joint.renderAt(x[0] - joint.getImageWidth() / 4 -2, y[0] - joint.getImageWidth() / 2 - 1);

	//if (bolts.size() == 0)
	//{
	//	for (int i = 0; i < lines.size() - 1; i++)
	//	{
	//		if ((x[i] != x[SIZE - 1] && y[i] != y[SIZE - 1]) && (x[i] != x[0] && y[i] != y[0]))
	//		{
	//			jointGlow.renderAt(x[i] - jointGlow.getW() / 4 - 2, y[i] - jointGlow.getW() / 2 - 1);
	//		}
	//		//		colors.makeSpriteFlashRainbow(&joint);
	//		//		colors.makeSpriteFlashRainbow(lines[i].getLineSprite());
	//		lines[i].render();
	//	}
	//}
	//else
	{
		if ((*elapsedRunTime) < timeOfLastLightning + lifeTimeOfLightning)
		{
			int tempW = jointGlow.getW() / 4 - 2;
			int tempH = jointGlow.getW() / 2 - 1;
			for (int i = 0; i < bolts.size(); i++)
			{
				for (int j = 0; j < bolts[i].size(); j++)
				{
					bolts[i][j].render();
					jointGlow.renderAt(bolts[i][j].getInitialX() - tempW, bolts[i][j].getInitialY() - tempH);

					if (i == bolts.size() - 1 && j == bolts[i].size() - 1)
					{
						sparkAnimation.renderAt(bolts[i][j].getInitialX() - sparkAnimation.getW() / 2, bolts[i][j].getInitialY() - sparkAnimation.getW() / 2);
					}
				}
			}
		}
	}
}

void Lightning::dimLightning()
{
	timeSinceLastDim = *elapsedRunTime - timeOfLastDim;
	if (timeSinceLastDim > timeBetweenDim)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			Sprite temp = *lines[i].getLineSprite();
			if (temp.getOpacity() != 0)
			{
				temp.changeOpactity(1);
				jointGlow.changeOpactity(1);
				jointPoint.changeOpactity(1);
			}
			lines[i].setLineSprite(temp);

			timeOfLastDim = *elapsedRunTime;
		}
	}
}

void Lightning::unDimLightning()
{
	for (int i = 0; i < lines.size(); i++)
	{
		if ((*lines[i].getLineSprite()).getOpacity() != 0)
		{
			(*lines[i].getLineSprite()).changeOpactity(200);
			jointGlow.changeOpactity(200);
			jointPoint.changeOpactity(200);
		}
		lines[i].setLineSprite((*lines[i].getLineSprite()));
	}
}


void Lightning::singularStorm()
{
	timeSinceLastLightning = *elapsedRunTime - timeOfLastLightning;
	if (timeSinceLastLightning > timeBetweenLightning)
	{
		createPoints();
		generateSingleLightning();

		timeOfLastLightning = *elapsedRunTime;
	}
	renderLightning();
}

void Lightning::singularStormAt(int x, int y)
{
	timeSinceLastLightning = *elapsedRunTime - timeOfLastLightning;
	if (timeSinceLastLightning > timeBetweenLightning)
	{
		generateSingleLightningAt(x, y);

		timeOfLastLightning = *elapsedRunTime;
	}
	renderLightning();
}