#pragma once
#include"AnimatedSprite.h"
#include"Console.h"
#include"HitBox.h"
#include"Map.h"
#include"Player.h"

class Enemy
{
public:
	Enemy();
	Enemy(SDL_Renderer* ren, Interaction *cont, Console * dConsole, int * tGameTime, Map *map, std::string enemyName,double eSpeed, int bHealth, std::vector<AnimatedSprite> walkingAnimations, Player * p);
	~Enemy();

	//Getters
	HitBox getHitBox();
	HitBox* getHitBoxPtr();
	std::string getName();
	int getCurrentHealth();
	int getBaseHealth();
	int getBounty();
	int getCurrentTileI();
	int getCurrentTileJ();
	int getX();
	int getY();
	int getCenterX();
	int getCenterY();

	//Setters
	void setPosition(int xPos, int yPos);
	void moveBy(int dX, int dY);
	void setMap(Map * m);
	void setCurrentHealth(int cH);
	void setBounty(int b);
	void setColor(SDL_Color newColor);

	//Misc
	bool spawn();
	bool isSpawned();
	bool isAlive();
	void suicide();
	void hurt(int d);
	bool mouseHoveringOverEnemy();
	bool enemyClicked();

	//initializers
	void initializeHitBox();


	//Update
	bool updatePosition();
	void update();

	//Render
	void renderHealthBar();
	void render();
	void renderInterfaceCopyAt(int xPos, int yPos);

private:
	SDL_Renderer* renderer;
	Console *devConsole;
	Interaction *controls;
	Map *levelMap;
	int *gameTime;
	Player *player;
	std::vector<AnimatedSprite> animations; //0 = left, 1 = right, 2 = up, 3 = down
//	AnimatedSprite healthBar;
	int currentAnimation;

	int x, y;
	int mDX, mDY;
	int previousTileI, previousTileJ, currentTileI, currentTileJ;
	int motionCounter;

	std::string name;
	double speed, extraSpeed;
	int baseHealth, currentHealth;
	int bounty;

	//HEALTH BAR
	std::vector<SDL_Rect> healthBarRect;

	bool spawned;

	HitBox hitBox;
};