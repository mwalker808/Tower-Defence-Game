#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(SDL_Renderer * ren, Interaction * cont, Console * dConsole, int * tGameTime, Map *map, std::string enemyName, double eSpeed, int bHealth, std::vector<AnimatedSprite> walkingAnimations, Player * p)
{
	renderer = ren;
	controls = cont;
	devConsole = dConsole;
	gameTime = tGameTime;
	setMap(map);
	player = p;

	name = enemyName;
	speed = eSpeed;
	baseHealth = bHealth;
	setCurrentHealth(bHealth);

	animations = walkingAnimations;
	currentAnimation = 0;

	initializeHitBox();
	setPosition(0, 0);

	extraSpeed = 0;
	mDX = speed;
	mDY = 0;

	currentTileI = 0;
	currentTileJ = 0;

	previousTileI = -1;
	previousTileJ = -1;

	motionCounter = 40;

	spawned = false;

	setBounty(150);

	healthBarRect.push_back({ 100,100,50,6 });
	healthBarRect.push_back({ 100,100,50,6 });

//	healthBar = AnimatedSprite("./images/TowerDefenceSprites/EnemySprites/healthBar.png", renderer, gameTime, 0, 0,10, 1, 10, 400, 6, 200, 100);


}


Enemy::~Enemy()
{
}

HitBox Enemy::getHitBox()
{
	return hitBox;
}

HitBox * Enemy::getHitBoxPtr()
{
	return &hitBox;
}

std::string Enemy::getName()
{
	return name;
}

int Enemy::getCurrentHealth()
{
	return currentHealth;
}

int Enemy::getBaseHealth()
{
	return baseHealth;
}

int Enemy::getBounty()
{
	return bounty;
}

int Enemy::getCurrentTileI()
{
	return currentTileI;
}

int Enemy::getCurrentTileJ()
{
	return currentTileJ;
}

int Enemy::getX()
{
	return x;
}

int Enemy::getY()
{
	return y;
}

int Enemy::getCenterX()
{
	return 	animations[currentAnimation].getCenterX();
}

int Enemy::getCenterY()
{
	return 	animations[currentAnimation].getCenterY();
}

void Enemy::setPosition(int xPos, int yPos)
{
	x = xPos;
	y = yPos;

	animations[currentAnimation].setX(x);
	animations[currentAnimation].setY(y);

	hitBox.setX(x);
	hitBox.setY(y);
}

void Enemy::moveBy(int dX, int dY)
{
	if (isAlive())
	{
		setPosition(x + dX, y + dY);
	}
}

void Enemy::setMap(Map * m)
{
	levelMap = m;
}

void Enemy::setCurrentHealth(int cH)
{
	currentHealth = cH;
		//changes length of green health bar
}

void Enemy::setBounty(int b)
{
	bounty = b;
}

void Enemy::setColor(SDL_Color newColor)
{
	animations[currentAnimation].changeColor(newColor);
}

bool Enemy::spawn()
{
	if ((*levelMap).getSpawnJ() != -1 && (*levelMap).getSpawnI() != -1)
	{
		spawned = true;
		setPosition((*levelMap).getSpawnJ() * (*levelMap).getTileWidth(), (*levelMap).getSpawnI() * (*levelMap).getTileHeight());
		return true;
	}
	else
	{
		(*devConsole).insertMessage("UNABLE TO SPAWN ENEMY: " + std::string("NO SPAWN TILE IN MAP"));
		return false;
	}
}

bool Enemy::isSpawned()
{
	return spawned;
}

bool Enemy::isAlive()
{
	return getCurrentHealth() > 0;
}

void Enemy::suicide()
{
	setCurrentHealth(0);
}

void Enemy::hurt(int d)
{
	if (getCurrentHealth() > 0)
	{
		if (d >= getCurrentHealth())
		{
			setCurrentHealth(0);
			(*player).setCash((*player).getCash() + getBounty());
		}
		else
		{
			setCurrentHealth(getCurrentHealth() - d);
		}
	}
	if (healthBarRect.size() >= 2)
	{
		healthBarRect[1].w = healthBarRect[0].w * ((double)getCurrentHealth() / (double)baseHealth);
	}
	//else
	//{
	//	(*devConsole).insertMessage(std::to_string(healthBarRect.size()));
	//}
}

bool Enemy::mouseHoveringOverEnemy()
{
	int tempDX = ((*controls).getMouseX() - animations[currentAnimation].getCenterX());
	int tempDY = ((*controls).getMouseY() - animations[currentAnimation].getCenterY());
	if (animations[currentAnimation].getW() < 60)
	{
		return sqrt(tempDX * tempDX + tempDY * tempDY) < 30; //NOTE: The 30 is to increase the click box of the enemy and make it easier to click
	}
	else
	{
		return sqrt(tempDX * tempDX + tempDY * tempDY) < animations[currentAnimation].getW() / 2; 
	}
}

bool Enemy::enemyClicked()
{
	return (controls->mouseClicked() && mouseHoveringOverEnemy());
}

void Enemy::initializeHitBox()
{
	if (animations.size() > 0)
	{
		hitBox = HitBox(renderer, { animations[0].getX(),animations[0].getY(),animations[0].getW(),animations[0].getH() });
	}
	else
	{
		(*devConsole).insertMessage("No Animations for enemy");
		hitBox = HitBox(renderer, {0,0,1,1});
	}
}

bool Enemy::updatePosition()
{
	bool tempReturnValue = false;
	//CHECK FOR IF THE SPRITE GOES OUT OF THE TILE PRIOR TO CHECKING WHERE TO MOVE TO NEXT
	if (motionCounter + speed > 40)
	{
		extraSpeed = motionCounter + speed - 40;
		if (mDX > 0)
		{
			mDX = mDX - extraSpeed;
		}
		else if(mDX < 0)
		{
			mDX = mDX + extraSpeed;
		}
		else if(mDY > 0)
		{
			mDY = mDY - extraSpeed;
		}
		else if (mDY < 0)
		{
			mDY = mDY + extraSpeed;
		}
	}

	if (motionCounter >= 40)
	{
		tempReturnValue = true;
		previousTileI = currentTileI;
		previousTileJ = currentTileJ;
		currentTileJ = (x + 20) / 40;
		currentTileI = (y + 20) / 40;

		if ((*levelMap).getMapIntValue(currentTileI, currentTileJ) == 4)
		{
			mDX = 0;
			mDY = 0;
			suicide();
			if ((*player).getHealth() > 0)
			{
				(*player).setHealth((*player).getHealth() - 1);
			}
		}
		//DOWN
		else if (((*levelMap).getMapIntValue(currentTileI + 1, currentTileJ) == 0 || (*levelMap).getMapIntValue(currentTileI + 1, currentTileJ) == 4) && previousTileI != currentTileI + 1)//down
		{
			mDX = 0;
			mDY = speed;
			currentAnimation = 3;
		}
		//UP
		else if (((*levelMap).getMapIntValue(currentTileI - 1, currentTileJ) == 0 || (*levelMap).getMapIntValue(currentTileI - 1, currentTileJ) == 4) && previousTileI != currentTileI - 1)//up
		{
			mDX = 0;
			mDY = -speed;
			currentAnimation = 2;
		}
		//RIGHT
		else if (((*levelMap).getMapIntValue(currentTileI, currentTileJ + 1) == 0 || (*levelMap).getMapIntValue(currentTileI, currentTileJ + 1) == 4) && previousTileJ != currentTileJ + 1)//right
		{
			mDX = speed;
			mDY = 0;
			currentAnimation = 1;
		}
		//LEFT
		else if (((*levelMap).getMapIntValue(currentTileI, currentTileJ - 1) == 0 || (*levelMap).getMapIntValue(currentTileI, currentTileJ - 1) == 4) && previousTileJ != currentTileJ - 1)//left
		{
			mDX = -speed;
			mDY = 0;
			currentAnimation = 0;
		}
		else
		{
			mDX = 0;
			mDY = 0;
		}
		if (mDX != 0 || mDY != 0)
		{
			motionCounter -= 40;
		}
//		(*devConsole).insertMessage("Current: " + std::to_string(currentTileJ) + "," + std::to_string(currentTileI));
//		(*devConsole).insertMessage("Previou: " + std::to_string(previousTileJ) + "," + std::to_string(previousTileI));
	}
	motionCounter += abs(mDX) + abs(mDY);

	moveBy(mDX, mDY);
		//MOVES HEALTH BARS
	healthBarRect[0].x = x - 20;
	healthBarRect[0].y = y - 20;
	healthBarRect[1].x = x - 20;
	healthBarRect[1].y = y - 20;

	return tempReturnValue;
}

void Enemy::renderHealthBar()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &healthBarRect[0]);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &healthBarRect[1]);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void Enemy::render()
{
	if (isAlive() && isSpawned())
	{
		animations[currentAnimation].render();
		renderHealthBar();
//		hitBox.render();
//		healthBar.setCurrentTexture(9 - (int)((double)((double)getCurrentHealth() / (double)baseHealth) * 9.0));
//		healthBar.renderAt(y - 4, x - 2);
	}

}

void Enemy::renderInterfaceCopyAt(int xPos, int yPos)
{
	if (animations.size() > 3)
	{
		animations[3].renderCopyAt(animations[currentAnimation].getX() + xPos, animations[currentAnimation].getY() + yPos);
	}
	else
	{
		animations[currentAnimation].renderCopyAt(animations[currentAnimation].getX() + xPos, animations[currentAnimation].getY() + yPos);
	}
}
