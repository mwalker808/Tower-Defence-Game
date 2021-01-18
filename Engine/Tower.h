#pragma once
#include<iostream>
#include"AnimatedSprite.h"
#include"Console.h"
#include<math.h>
#include"Enemy.h"
#include"Map.h"
#include"Text.h"
#include"Lightning.h"

enum TowerTypes { TOWER_TYPE_BULLET = 0, TOWER_TYPE_SPINNINGBLADE = 1, TOWER_TYPE_LIGHTNING = 2};

//TODO: Do not initialize text and sprites within this class. Change how to initialize valid, invalid, range, and infoText. Create the sprites in the gameengine and then pass them through the contructors for the animated sprites. 
//For the text, have the game engine take the tower info and then set the infotext in the game engine whenever a tower is clicked.

class Tower
{
public:
	Tower();
	//Tower(SDL_Renderer* ren, Interaction *cont, Console *dConsole, int *tGameTime, std::vector<AnimatedSprite> tCannonSprites, AnimatedSprite tBaseSprite, AnimatedSprite tProjectileSprite, std::string tName, std::string tDescription, int costOfTower, int tDamage, int tRange, int tAOEDamageRange, int nTotalTargets, int xPos, int yPos, int towType);
	Tower(SDL_Renderer* ren, Interaction *cont, Console *dConsole, int *tGameTime, std::vector<AnimatedSprite> tCannonSprites, AnimatedSprite tBaseSprite, AnimatedSprite tProjectileSprite, std::string tName, std::string tDescription, std::vector<int> costOfTower, std::vector<int> tDamage, std::vector<int> tRange, std::vector<int> tAOEDamageRange, std::vector<int> nTotalTargets, int xPos, int yPos, int towType);
	~Tower();

	//INITIALIZERS
	void initializeValidAndInvalidLocationSprites();
	void initializeInfoText();

	//Setters
	void setRange(std::vector<int> rnge);//Changes the towers attack range and will call the function updateTowerValidAndInlavidSpritesSize()
	void setPosition(int xPos, int yPos);//Sets the center position of all sprites to (xPos, yPos)
	void setMap(Map * m);
	void setCost(std::vector<int> c);
	void setTimeBetweenShots(int tBS);
	void setEffectSprite(AnimatedSprite eSprite);

	//Getters
	int getX();
	int getY();
	int getCenterX();
	int getCenterY();
	int getCost();
	std::string getName();
	int getDamage();
	int getNumberOfTargets();
	int getTowerRange();
	int getNCurrentTowerUpgrade();
	int getUpgradeCost();
	int getRefundAmmount();

	//Misc
	void rotateCannon(double degrees);
	void rotateCannonBy(double degrees);
	void toggleInfoDisplay();
	void idleAnimation();
	bool isSelected();
	bool isDisplayingInfo();
	bool isUpgradeable();
	bool upgrade();
	void reset();//clears the target projectiles and the target enemies

	//INPUT HANDLING
	bool mouseHoveringOverTower();//Returns true of the mouse is over the base of the tower.
	bool towerClicked();//Returns true if the tower is clicked while not in a Selection State
	void handleInput();

	//UPDATE FUNCTIONS
	void findTarget(std::vector<Enemy*> vectorOfEnemyPtrs);//projectiles
	void findTarget(std::vector<Enemy>* enemyVectorPtr);//projectiles
	void fire();
	void updateProjectilesInAir();


	void updateTower();


	//RENDER FUNCTIONS
	void renderInvalidValidRangeSprites();//Renders either a red or green circle based on whether or not the tower is in a valid spot.

	void renderSelectionState();
	void renderPlacedState();
	void renderTowerInfo();
	void renderFiringState();
	void renderProjectiles();
	void render();
	void renderInterfaceCopyAt(int xPos, int yPos);

private: 
	SDL_Renderer* renderer;
	Console *devConsole;
	Interaction *controls;
	int * gameTime;
	int towerType;
	AnimatedSprite towerBaseSprite;
	std::vector<AnimatedSprite> towerCannonSprites;
	std::vector<Text> infoText;

	void updateTowerValidAndInlavidSpritesSize();//Will update the sizes of these sprites, is only called when setRange() function is called
	AnimatedSprite validLocationSprite, invalidLocationSprite, towerRangeSprite;

	std::string towerName, towerDescription;
	std::vector<int> cost, damage, range, projectileAOEDamageRange;
	int nCurrentTowerUpgrade;
	int x, y, centerX, centerY;
	
	int towerState;
	bool validLocation, displayInfo;
	int SELECTION_STATE, PLACED_STATE, FIRING_STATE;

	int idleAnimationTimer, timeOfLastIdleAnimation;
	double idleCannonSpeed;

	//PROJECTILE VARIABLES
	AnimatedSprite projectileSprite;//TowerType = 0, 1
	std::vector<AnimatedSprite> projectilesInAir;//TowerType = 0, 1
//	std::vector<HitBox *> targetHitBoxes;
	std::vector<Enemy *> targetEnemies;//TowerType = 0, 1
	double projectileSpeed;//TowerType = 0, 1
	int projectileX, projectileY;//TowerType = 0, 1
	int timeOfLastShot, timeBetweenShots;//TowerType = 0, 1
	std::vector<HitBox> projectileHitBox;//TowerType = 1
	std::vector<double> radiansRotationCounter; //Tower Type = 1
	int nSpinningBlades;//TowerType =  1
	std::vector<int> maxTargets;//TowerType = 0
	Lightning lightningStrike;//TowerType = 2
	int jumpCounter, maxJumps; //TowerType = 2
	int timeOfLastJump, timeBetweenJumps;
	int sourceX, sourceY, targetX, targetY;//Tower Type = 2
	AnimatedSprite effectSprite; //TowerType = 2

	//Map Variables
	Map *levelMap;
	
	//TEST VARIABLES
	int textX1, testY1, textX2, testY2;

};