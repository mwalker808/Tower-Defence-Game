#include "Tower.h"

Tower::Tower()
{
}

//Tower::Tower(SDL_Renderer* ren, Interaction *cont, Console * dConsole, int * tGameTime, std::vector<AnimatedSprite> tCannonSprites, AnimatedSprite tBaseSprite, AnimatedSprite tProjectileSprite, std::string tName, std::string tDescription, int c, int tDamage, int tRange, int projAOEDamageRange, int nTotalTargets, int xPos, int yPos, int towType)
//{
//	srand(NULL);
//	renderer = ren;
//	devConsole = dConsole;
//	controls = cont;
//	gameTime = tGameTime;
//	nCurrentTowerUpgrade = 0;
//	towerType = towType;
//	nSpinningBlades = 1;//TowerType = 1
//	initializeValidAndInvalidLocationSprites();//Has to be initialized after all of the pointers for renderer, gameTime, controls, and developerConsole
//	towerCannonSprites = tCannonSprites;
//	towerBaseSprite = tBaseSprite;
//	projectileSprite = tProjectileSprite;
//	setPosition(xPos, yPos);
//	towerName = tName;
//	towerDescription = tDescription;
//	setCost({ c });
//	damage.push_back(tDamage);
//	setRange({tRange});//Should be called after the valid, invalid, and towerRange sprites have been initialized by initializeValidAndInvalidLocationSprites();
//	projectileAOEDamageRange.push_back(projAOEDamageRange);
//	maxTargets = { nTotalTargets };
//
//	initializeInfoText();
//
//
//	SELECTION_STATE = 0, PLACED_STATE = 1, FIRING_STATE = 3;
//	towerState = SELECTION_STATE;
//	validLocation = false;
//	displayInfo = false;
//	idleAnimationTimer = 0;
//	timeOfLastIdleAnimation = 0;
//	idleCannonSpeed = 0;
//
//
//	//PROJECTILE INITIATION
//	
////	projectileSprite = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/bullet3.png", renderer, gameTime, 0, 0, 1, 1, 1, 9, 9, 400, 540);
//	projectileSpeed = 8;
//	projectileX = 0;
//	projectileY = 0;
//	timeOfLastShot = 0;
//	timeBetweenShots = 1000;
//
//	//TOWER TYPE 2: LightningTower
//	lightningStrike = Lightning(renderer, tGameTime);
//	jumpCounter = 0, maxJumps = 3;
//	timeOfLastJump = -100, timeBetweenJumps = 100;
//}

Tower::Tower(SDL_Renderer * ren, Interaction * cont, Console * dConsole, int * tGameTime, std::vector<AnimatedSprite> tCannonSprites, AnimatedSprite tBaseSprite, AnimatedSprite tProjectileSprite, std::string tName, std::string tDescription, std::vector<int> costOfTower, std::vector<int> tDamage, std::vector<int> tRange, std::vector<int> tAOEDamageRange, std::vector<int> nTotalTargets, int xPos, int yPos, int towType)
{
	srand(NULL);
	renderer = ren;
	devConsole = dConsole;
	controls = cont;
	gameTime = tGameTime;
	nCurrentTowerUpgrade = 0;
	towerType = towType;
	nSpinningBlades = 1;//TowerType = 1
	initializeValidAndInvalidLocationSprites();//Has to be initialized after all of the pointers for renderer, gameTime, controls, and developerConsole
	towerCannonSprites = tCannonSprites;
	towerBaseSprite = tBaseSprite;
	projectileSprite = tProjectileSprite;
	setPosition(xPos, yPos);
	towerName = tName;
	towerDescription = tDescription;
	setCost( costOfTower );
	damage = tDamage;
	setRange( tRange );//Should be called after the valid, invalid, and towerRange sprites have been initialized by initializeValidAndInvalidLocationSprites();
	projectileAOEDamageRange = tAOEDamageRange;
	maxTargets = nTotalTargets;

	initializeInfoText();


	SELECTION_STATE = 0, PLACED_STATE = 1, FIRING_STATE = 3;
	towerState = SELECTION_STATE;
	validLocation = false;
	displayInfo = false;
	idleAnimationTimer = 0;
	timeOfLastIdleAnimation = 0;
	idleCannonSpeed = 0;


	//PROJECTILE INITIATION

	//	projectileSprite = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/bullet3.png", renderer, gameTime, 0, 0, 1, 1, 1, 9, 9, 400, 540);
	projectileSpeed = 8;
	projectileX = 0;
	projectileY = 0;
	timeOfLastShot = 0;
	timeBetweenShots = 1000;

	//TOWER TYPE 2: LightningTower
	lightningStrike = Lightning(renderer, tGameTime);
	jumpCounter = 0, maxJumps = 3;
	timeOfLastJump = -100, timeBetweenJumps = 100;
}

Tower::~Tower()
{
}

void Tower::setRange(std::vector<int> rnge)
{
	range = rnge;
	updateTowerValidAndInlavidSpritesSize();
}

void Tower::setPosition(int xPos, int yPos)
{
	centerX = xPos;
	centerY = yPos;

	x = centerX - towerBaseSprite.getW() / 2;
	y = centerY - towerBaseSprite.getH() / 2;
	towerBaseSprite.setX(x);
	towerBaseSprite.setY(y);

	switch (towerType)
	{
	case TOWER_TYPE_LIGHTNING:
		effectSprite.setPosition(x + (40 - effectSprite.getW()) / 2 - 9, y - effectSprite.getH() / 2 + 8);
		break;
	}

	for (int i = 0; i < towerCannonSprites.size(); i++)
	{
		x = centerX - towerCannonSprites[i].getW() / 2.0;
		y = centerY - towerCannonSprites[i].getH() / 2;
		towerCannonSprites[i].setX(x);
		towerCannonSprites[i].setY(y);
	}

	x = centerX - (double)invalidLocationSprite.getW() / 2.0;
	y = centerY - (double)invalidLocationSprite.getH() / 2.0;
	invalidLocationSprite.setX(x);
	invalidLocationSprite.setY(y);

	validLocationSprite.setX(x);
	validLocationSprite.setY(y);

	towerRangeSprite.setX(x);
	towerRangeSprite.setY(y);


}

void Tower::setMap(Map * m)
{
	levelMap = m;
}

void Tower::setCost(std::vector<int> c)
{
	cost = c;
}

void Tower::setTimeBetweenShots(int tBS)
{
	timeBetweenShots = tBS;
}

void Tower::setEffectSprite(AnimatedSprite eSprite)
{
	effectSprite = eSprite;
}

int Tower::getX()
{
	return x;
}

int Tower::getY()
{
	return y;
}

int Tower::getCenterX()
{
	return centerX;
}

int Tower::getCenterY()
{
	return centerY;
}

int Tower::getCost()
{
	if (nCurrentTowerUpgrade < cost.size() && nCurrentTowerUpgrade >= 0)
	{
		return cost[nCurrentTowerUpgrade];
	}
	else
	{
		return -1;
	}
}

std::string Tower::getName()
{
	return towerName;
}

int Tower::getDamage()
{
	if (nCurrentTowerUpgrade < damage.size() && nCurrentTowerUpgrade >= 0)
	{
		return damage[nCurrentTowerUpgrade];
	}
	else
	{
		return -1;
	}
}

int Tower::getNumberOfTargets()
{
	return 0;
}

int Tower::getTowerRange()
{
	if (nCurrentTowerUpgrade < range.size() && nCurrentTowerUpgrade >= 0)
	{
		return range[nCurrentTowerUpgrade];
	}
	else
	{
		return -1;
	}
}

int Tower::getNCurrentTowerUpgrade()
{
	return nCurrentTowerUpgrade;
}

int Tower::getUpgradeCost()
{
	if (nCurrentTowerUpgrade + 1 < cost.size())
	{
		return cost[nCurrentTowerUpgrade + 1];
	}
	else
	{
		return -1;
	}
}

int Tower::getRefundAmmount()
{
	int tempSum = 0;

	for (int i = 0; i < cost.size() && i <= nCurrentTowerUpgrade; i++)
	{
		tempSum += cost[i];
	}

	return tempSum;
}

void Tower::rotateCannon(double degrees)
{
	towerCannonSprites[nCurrentTowerUpgrade].rotate(degrees);
}

void Tower::rotateCannonBy(double degrees)
{
	towerCannonSprites[nCurrentTowerUpgrade].rotateBy(degrees);
}

void Tower::toggleInfoDisplay()
{
//	if (displayInfo)
//	{
//	}
//	else
//	{
////		devConsole->insertMessage("Tower Info Requested ");
//		//(*devConsole).insertMessage("Tower Name: " + towerName);
//		//(*devConsole).insertMessage("Tower Upgrade (0 lowest): " + std::to_string(currentTowerUpgrade));
//		//(*devConsole).insertMessage("Range: " + std::to_string(range));
//		//(*devConsole).insertMessage("Damage: " + std::to_string(damage));
//		////	(*devConsole).insertMessage(": " + std::to_string());
//		//(*devConsole).insertMessage("Description: " + towerDescription);
//	}
	displayInfo = !displayInfo;
}

void Tower::idleAnimation()
{
	if (*gameTime > timeOfLastIdleAnimation + idleAnimationTimer)
	{
		idleAnimationTimer = (rand() % 5000);
		timeOfLastIdleAnimation = (*gameTime);
		idleCannonSpeed = double(idleAnimationTimer) / 1000.0 - 2.5;
	}
	else
	{
//		(*devConsole).insertMessage("Cannon Speed: " + std::to_string(idleCannonSpeed));
		rotateCannonBy(idleCannonSpeed);
	}
}

bool Tower::isSelected()
{
	return towerState == SELECTION_STATE;
}

bool Tower::isDisplayingInfo()
{
	return displayInfo;
}

bool Tower::isUpgradeable()
{

	return (nCurrentTowerUpgrade + 1 < cost.size());
}

bool Tower::upgrade()
{
	if (nCurrentTowerUpgrade + 1 < cost.size())
	{
		nCurrentTowerUpgrade++;
		updateTowerValidAndInlavidSpritesSize();

		if (towerType == TOWER_TYPE_SPINNINGBLADE)
		{
			towerCannonSprites[0].setW(range[nCurrentTowerUpgrade]);
			towerCannonSprites[0].setPosition(getCenterX() - (double)towerCannonSprites[0].getW() / 2.0, getCenterY() - (double)towerCannonSprites[0].getH() / 2.0);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Tower::reset()
{
	//NOTE: if you clear projectiles in air for TOWER_TYPE_SPINNINGBLADE then all of the blades will be removed
	if (towerType == TOWER_TYPE_BULLET)
	{
		projectilesInAir.clear();
	}

	targetEnemies.clear();
}

bool Tower::mouseHoveringOverTower()
{
	int tempDX = ((*controls).getMouseX() - centerX);
	int tempDY = ((*controls).getMouseY() - centerY);
	return sqrt(tempDX * tempDX + tempDY * tempDY) < towerBaseSprite.getW() / 2;
}

bool Tower::towerClicked()
{
	return  mouseHoveringOverTower() && (*controls).mouseClicked();
}

void Tower::handleInput()
{
	if ((*controls).mouseClicked() && validLocation && towerState == SELECTION_STATE)
	{
		towerState = PLACED_STATE;
		(*levelMap).setMapIntValue(getCenterY() / 40, getCenterX() / 40, 2);
	}
	//else if (towerState != SELECTION_STATE && towerClicked())
	//{
	//	toggleInfoDisplay();
	//}
}

void Tower::findTarget(std::vector<Enemy*> e)
{
	switch (towerType)
	{
	case TOWER_TYPE_SPINNINGBLADE:
	{
		int temp = 0;
		if ((*gameTime) > timeOfLastShot + timeBetweenShots)
		{
			double tempX0 = 0;
			double tempY0 = 0;
			double tempX1 = 0;
			double tempY1 = 0;
			double tempDist = 0;
			for (int i = 0; i < e.size(); i++)
			{
				if (e[i]->isAlive() && e[i]->isSpawned())
				{
					for (int k = 0; k < projectilesInAir.size(); k++)
					{
//						devConsole->insertMessage(std::to_string(projectilesInAir.size()));
						tempX0 = e[i]->getHitBox().getCenterX();
						tempY0 = e[i]->getHitBox().getCenterY();
						tempX1 = projectilesInAir[k].getCenterX();
						tempY1 = projectilesInAir[k].getCenterY();
						tempDist = sqrt((tempX0 - tempX1) * (tempX0 - tempX1) + (tempY0 - tempY1) *(tempY0 - tempY1));
						//					(*devConsole).insertMessage(std::to_string((*e)[i].getHitBox().getCenterX()));
						if (tempDist < projectilesInAir[k].getW())
						{
							e[i]->hurt(damage[nCurrentTowerUpgrade]);
							timeOfLastShot = *gameTime;
						}
						temp++;
					}
				}
			}
			//			devConsole->insertMessage(std::to_string(temp));
		}
	}
	break;
	default:
		(*devConsole).insertMessage("Invalid tower type in Tower::findTarget(std::vector<Enemy>* e)");
	}

}

void Tower::findTarget(std::vector<Enemy>* e)
{
	switch (towerType)
	{
	case TOWER_TYPE_BULLET:
	{
		int tempShotCounter = 0;
		if ((*gameTime) > timeOfLastShot + timeBetweenShots)
		{
			for (int i = 0; i < (*e).size(); i++)
			{
				if ((*e)[i].isAlive() && (*e)[i].isSpawned())
				{
					double tempDist = sqrt(((*e)[i].getHitBox().getCenterX() - centerX) * ((*e)[i].getHitBox().getCenterX() - centerX) + ((*e)[i].getHitBox().getCenterY() - centerY) * ((*e)[i].getHitBox().getCenterY() - centerY));
					if (tempDist < range[nCurrentTowerUpgrade] / 2)
					{
						//Rotates the cannon to the target
						double tempDX = (*e)[i].getHitBox().getCenterX() - centerX;
						double tempDY = (*e)[i].getHitBox().getCenterY() - centerY;
						double tempAngle = (atan2(tempDY, tempDX) + 3.14159);
						tempAngle = tempAngle * 180 / 3.14159 + 180;//Convert to degrees
	//					(*devConsole).insertMessage(std::to_string(tempAngle));
						rotateCannon(tempAngle);
						//Adds a projectile towards the enemy found
						projectileSprite.setX(centerX);
						projectileSprite.setY(centerY);
						projectilesInAir.push_back(projectileSprite);
						targetEnemies.push_back(&(*e)[i]);
						timeOfLastShot = *gameTime;
						tempShotCounter++;
						if (tempShotCounter >= maxTargets[nCurrentTowerUpgrade])
						{
							i = (*e).size();//breaks out of for loop
						}
					}
				}
			}
		}
	}
		break;
	case TOWER_TYPE_SPINNINGBLADE:
	{
		if ((*gameTime) > timeOfLastShot + timeBetweenShots)
		{
			double tempX0 = 0;
			double tempY0 = 0;
			double tempX1 = 0;
			double tempY1 = 0;
			double tempDist = 0;
			for (int i = 0; i < (*e).size(); i++)
			{
				if ((*e)[i].isAlive() && (*e)[i].isSpawned())
				{
					for (int k = 0; k < projectilesInAir.size(); k++)
					{
//						devConsole->insertMessage(std::to_string(projectilesInAir.size()));
						tempX0 = (*e)[i].getHitBox().getCenterX();
						tempY0 = (*e)[i].getHitBox().getCenterY();
						tempX1 = projectilesInAir[k].getCenterX();
						tempY1 = projectilesInAir[k].getCenterY();
						tempDist = sqrt((tempX0 - tempX1) * (tempX0 - tempX1) + (tempY0 - tempY1) *(tempY0 - tempY1));
						//					(*devConsole).insertMessage(std::to_string((*e)[i].getHitBox().getCenterX()));
						if (tempDist < projectilesInAir[k].getW())
						{
							(*e)[i].hurt(damage[nCurrentTowerUpgrade]);
							timeOfLastShot = *gameTime;
						}
					}
				}
			}
		}
	}
		break;
	case TOWER_TYPE_LIGHTNING:
	{
		if ((*gameTime) > timeOfLastShot + timeBetweenShots)
		{
			for (int i = 0; i < (*e).size(); i++)
			{
				if ((*e)[i].isAlive() && (*e)[i].isSpawned() && targetEnemies.size() == 0)
				{
					double tempDist = sqrt(((*e)[i].getHitBox().getCenterX() - centerX) * ((*e)[i].getHitBox().getCenterX() - centerX) + ((*e)[i].getHitBox().getCenterY() - centerY) * ((*e)[i].getHitBox().getCenterY() - centerY));
					if (tempDist < range[nCurrentTowerUpgrade] / 2)
					{
						for (int k = 0; i + k < (*e).size() && k < maxTargets[nCurrentTowerUpgrade] && (*e)[i + k].isAlive() && (*e)[i + k].isSpawned(); k++)
						{
							targetEnemies.push_back(&(*e)[i + k]);
						}
						timeOfLastShot = *gameTime;
						i = (*e).size();//breaks out of for loop
					}
				}
			}
		}
	}
		break;
	default:
		(*devConsole).insertMessage("Invalid tower type in Tower::findTarget(std::vector<Enemy>* e)");
	}
}

void Tower::updateProjectilesInAir()
{
	switch (towerType)
	{
	case TOWER_TYPE_BULLET:
	{		
			//runs a check to see if the projectiles enemy is within 20 pixles of the enemy
			//projectilesInAir and targetEnemies share the same index
			//Updates the projectiles path and orientation to move towards the target enemy
			//when an enemy is "hit" then the enemy is "hurt" and then the targetEnemies and projectilesInAir vectors have their current index element removed
		for (int i = 0; i < projectilesInAir.size(); i++)
		{
			double tempDX = (*targetEnemies[i]).getHitBox().getCenterX() - projectilesInAir[i].getX();
			double tempDY = (*targetEnemies[i]).getHitBox().getCenterY() - projectilesInAir[i].getY();
			double tempAngle = (atan2(tempDY, tempDX) + 3.14159);

			if (abs(tempDX) + abs(tempDY) < 20 || !(*targetEnemies[i]).isAlive())//checks if the projectile is close enough to the center of the sprite for a clearly visible hit
			{
				(*targetEnemies[i]).hurt(damage[nCurrentTowerUpgrade]);

				projectilesInAir.erase(projectilesInAir.begin() + i);
				targetEnemies.erase(targetEnemies.begin() + i);
			}
			else
			{
				projectilesInAir[i].rotate(tempAngle * 180 / 3.14159 + 180);//Need to convert the nagle from radians to degrees
				projectilesInAir[i].setX(projectilesInAir[i].getX() - projectileSpeed * cos(tempAngle));
				projectilesInAir[i].setY(projectilesInAir[i].getY() - projectileSpeed * sin(tempAngle));
			}
		}
	}
		break;
	case TOWER_TYPE_SPINNINGBLADE:
	{	
		//ADDING SPINNIGN BLADE PROJECTILES IF THEY WERE NTO ALREADY ADDED
		if (projectilesInAir.size() != maxTargets[nCurrentTowerUpgrade])
		{
			projectilesInAir.clear();
			radiansRotationCounter.clear();

//TODO: CHANGE HOW THE ARMS ARE CREATED. CREATE AN ARMSPRITE VECTOR AND ADD THEM IN SIMILAR TO PROJECTILES IN AIR AND RADIANSROTATIONCOUNTER, SEE ABOVE
			//RESETS THE CANNON SPRITES ANGLES BACK TO THEIR ORIGIONAL POSITIONS
			for (int i = 0; i < towerCannonSprites.size(); i++)
			{
				towerCannonSprites[i].rotate(0);
			}
			//ADDS MORE CANNONS IF THERE ARENT THE SAME NUMBER OF BLADES AS CANNONS
			if (towerCannonSprites.size() != maxTargets[nCurrentTowerUpgrade])
			{
				for (int i = 1; i < maxTargets.size() && towerCannonSprites.size() != maxTargets[nCurrentTowerUpgrade]; i++)
				{
					towerCannonSprites.push_back(towerCannonSprites[0]);
				}
			}

			for (int i = 0; i < maxTargets[nCurrentTowerUpgrade] && i < towerCannonSprites.size(); i++)
			{
				projectilesInAir.push_back(projectileSprite);
				radiansRotationCounter.push_back(i * 2 * 3.14159 / maxTargets[nCurrentTowerUpgrade]);
				projectilesInAir[i].setPosition(getCenterX() - (double)projectileSprite.getW() / 2.0 - ((double)(range[nCurrentTowerUpgrade])) * cos(radiansRotationCounter[i]) / 2, getCenterY() - (double)projectileSprite.getH() / 2.0 + ((double)(range[nCurrentTowerUpgrade])) * sin(radiansRotationCounter[i]) / 2.0);
				//INITIALIZES THE CANNONS DIMENTIONS SO THAT IT LOOKS AS THOUT IT IS HOLDING THE BLADE
				towerCannonSprites[i].setW(range[nCurrentTowerUpgrade]);//
				towerCannonSprites[i].setH(projectileSprite.getH() / 4);//
				towerCannonSprites[i].rotateBy(i * 360 / maxTargets[nCurrentTowerUpgrade] + 180);
				towerCannonSprites[i].setPosition(getCenterX() - (double)towerCannonSprites[i].getW() / 2.0, getCenterY() - (double)towerCannonSprites[i].getH() / 2.0);//
			}
		}
		//UPDATES THE SPINNING BLADES LOCATION TO SPIN AROUND ITS SOURCE TOWER
		for (int i = 0; i <  projectilesInAir.size() && i < towerCannonSprites.size(); i++)
		{
			//UPDATES THE ROTATION OF THE BLADE
			projectilesInAir[i].rotateBy(6);
						//UPDATES THE POSITION OF THE BLADE HOLDER
			towerCannonSprites[i].rotateBy(-.02 * 180.0 / 3.14159);
			radiansRotationCounter[i] += .02;

			projectilesInAir[i].setPosition(getCenterX() - (double)projectileSprite.getW() / 2.0 - ((double)(range[nCurrentTowerUpgrade])) * cos(radiansRotationCounter[i]) / 2, getCenterY() - (double)projectileSprite.getH() / 2.0 + ((double)(range[nCurrentTowerUpgrade])) * sin(radiansRotationCounter[i]) / 2.0);
		}
	}
		break;
	case TOWER_TYPE_LIGHTNING:
	{
		if (targetEnemies.size() > 0 && jumpCounter < targetEnemies.size())
		{
			if ((*gameTime) > timeOfLastJump + timeBetweenJumps)
			{
				if (jumpCounter == 0)
				{
					sourceX = centerX;
					sourceY = centerY;
					targetX = targetEnemies[jumpCounter]->getHitBox().getCenterX();
					targetY = targetEnemies[jumpCounter]->getHitBox().getCenterY();
					lightningStrike.generateMultipleLightingStartingAtEndingAt(sourceX, sourceY, targetX, targetY, 1);
				}
				else if (jumpCounter < targetEnemies.size())
				{
					sourceX = targetEnemies[jumpCounter - 1]->getHitBox().getCenterX();
					sourceY = targetEnemies[jumpCounter - 1]->getHitBox().getCenterY();
					targetX = targetEnemies[jumpCounter]->getHitBox().getCenterX();
					targetY = targetEnemies[jumpCounter]->getHitBox().getCenterY();
					lightningStrike.generateMultipleLightingStartingAtEndingAt(sourceX, sourceY, targetX, targetY, 1);
				}
				//Damages the targeted enemy and then increments jumpConter to the next enemy, also resets the timeOfLastJump
				targetEnemies[jumpCounter]->hurt(damage[nCurrentTowerUpgrade]);
				jumpCounter++;
				timeOfLastJump = *gameTime;
			}
		}
		else
		{
			jumpCounter = 0;
			targetEnemies.clear();
		}
	}
		break;
	default:
		(*devConsole).insertMessage("Invalid tower type in Tower::updateProjectilesInAir");
	}
}

void Tower::updateTower()
{
	handleInput();
	if (towerState == SELECTION_STATE)
	{
		setPosition(0 + (*controls).getMouseX() / 40 * 40 + 20, 0 + (*controls).getMouseY() / 40 * 40 + 20); 
		if ((*levelMap).getMapIntValue((*controls).getMouseY() / 40,  (*controls).getMouseX() / 40 ) == 1)
		{
			validLocation = true;
		}
		else
		{
			validLocation = false;
		}
	}
	else if (towerState == PLACED_STATE)
	{
//		idleAnimation();

		updateProjectilesInAir();

	}
	else if (towerState == FIRING_STATE)
	{

	}
}


void Tower::initializeValidAndInvalidLocationSprites()
{
		invalidLocationSprite = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/InvalidTowerLocationInTowerRange.png", renderer, gameTime, 0, 0, 1, 1, 1, 200, 200, 0, 0);
		validLocationSprite = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/InTowerRange.png", renderer, gameTime, 0, 0, 1, 1, 1, 200, 200, 0, 0);
		towerRangeSprite = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/TowerRange.png", renderer, gameTime, 0, 0, 1, 1, 1, 1200, 1200, 0, 0);
}

void Tower::initializeInfoText()
{
	SDL_Color tempColor = {255,255,255};
	int tempFontSize = 16;


	infoText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", tempFontSize, tempColor, "Tower Name: " + towerName));
	infoText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", tempFontSize, tempColor, "Tower Upgrade (0 lowest): " + std::to_string(nCurrentTowerUpgrade)));
	infoText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", tempFontSize, tempColor, "Range: " + std::to_string(range[nCurrentTowerUpgrade])));
	infoText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", tempFontSize, tempColor, "Damage: " + std::to_string(damage[nCurrentTowerUpgrade])));
	infoText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", tempFontSize, tempColor, "Description: " + towerDescription[nCurrentTowerUpgrade]));

	for (int i = 0; i < infoText.size(); i++)
	{
		infoText[i].setPosition(1010, 400 + i * (infoText[i].getHeight() + 3));
	}
}

void Tower::updateTowerValidAndInlavidSpritesSize()
{
	invalidLocationSprite.setW(range[nCurrentTowerUpgrade]);
	invalidLocationSprite.setH(range[nCurrentTowerUpgrade]);

	validLocationSprite.setW(range[nCurrentTowerUpgrade]);
	validLocationSprite.setH(range[nCurrentTowerUpgrade]);

	towerRangeSprite.setW(range[nCurrentTowerUpgrade]);
	towerRangeSprite.setH(range[nCurrentTowerUpgrade]);

	setPosition(centerX, centerY);
}

void Tower::renderInvalidValidRangeSprites()
{
	if (!validLocation)
	{
		invalidLocationSprite.render();
		towerRangeSprite.render();
	}
	else
	{
		validLocationSprite.render();
		towerRangeSprite.render();
	}
}

void Tower::renderSelectionState()
{
	renderInvalidValidRangeSprites();
}

void Tower::renderPlacedState()
{
}

void Tower::renderTowerInfo()
{
	towerRangeSprite.render();

	//for (int i = 0; i < infoText.size(); i++)
	//{
	//	infoText[i].render();
	//}
}

void Tower::renderProjectiles()
{
	for (int i = 0; i < projectilesInAir.size(); i++)
	{
		projectilesInAir[i].render();
	}
}

void Tower::render()
{
	if (towerState == SELECTION_STATE)
	{
		renderSelectionState();
	}

	if (isDisplayingInfo())
	{
		renderTowerInfo();
	}

	towerBaseSprite.render();


	if (towerType == TOWER_TYPE_BULLET)
	{
		towerCannonSprites[nCurrentTowerUpgrade].render();
	}
	else if (towerType == TOWER_TYPE_SPINNINGBLADE)
	{
		for (int i = 0; i < towerCannonSprites.size() && i < projectilesInAir.size(); i++)
		{
			towerCannonSprites[i].render();
		}
	}
	else if (towerType == TOWER_TYPE_LIGHTNING)
	{
		effectSprite.render();
		lightningStrike.renderLightning();
	}
}

void Tower::renderInterfaceCopyAt(int xPos, int yPos)
{
	towerBaseSprite.renderCopyAt(towerBaseSprite.getX() + xPos, towerBaseSprite.getY() + yPos);

	if (towerType == TOWER_TYPE_BULLET)
	{
		towerCannonSprites[nCurrentTowerUpgrade].renderCopyAt(towerCannonSprites[nCurrentTowerUpgrade].getX() + xPos, towerCannonSprites[nCurrentTowerUpgrade].getY() + yPos);
	}
	else if (towerType == TOWER_TYPE_LIGHTNING)
	{
		effectSprite.renderCopyAt(effectSprite.getX() + xPos, effectSprite.getY() + yPos);
	}
	else if(towerType == TOWER_TYPE_SPINNINGBLADE)
	{
		for (int i = 0; i < towerCannonSprites.size() && i < projectilesInAir.size(); i++)
		{
			towerCannonSprites[i].renderCopyAt(towerCannonSprites[i].getX() + xPos, towerCannonSprites[i].getY() + yPos);
			projectilesInAir[i].renderCopyAt(projectilesInAir[i].getX() + xPos, projectilesInAir[i].getY() + yPos);
		}
	}
}
