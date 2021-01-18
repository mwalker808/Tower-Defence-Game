#pragma once
#include <iostream>
#include<math.h>
#include<vector>
#include<time.h>
#include<SDL.h>
#include<SDL_image.h>
#include"Sprite.h"
#include "Utility.h"
#include "AnimatedSprite.h"
#include "Interaction.h"
#include "Text.h"
#include "Console.h"
#include"Line.h"
#include"HitBox.h"
#include"Button.h"
#include"Tower.h"
#include"Enemy.h"
#include"Map.h"
#include"Interface.h"
#include"Player.h"

#include"Lightning.h"
#include"LightningStorm.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void initialize();

	void handleInput();

	void update();
	void slowUpdate();

	void render();

	//MISC
	bool isRunning();
	void close();
	void changeToWave(int w);//WAVE

	void testUpdate();//contains update functions currently in the testing.

private:
	void initializeWindow();
	void initializeInput();
	void initializeConsole();
	void initializeSprites();
	void initializeMap();
	void initializeEnemies();
	void initializeText();
	void initializeTimeVariables();
	void initializeButtons();
	void initializeInterfaces();
	void initializePlayer();
	void initializeWaves();

	bool run = true;

	//WINDOW VARIABLES: initializeWindow()
	const int SCREEN_WIDTH = 1400;
	const int SCREEN_HEIGHT = 700;
	Utility mainWindow;

	//INPUT VARIABLES: initializeInput()
	SDL_Event e;
	Interaction controls;

	//CONSOLE VARIABLES: initializeConsole()
	Console developerConsole;

	//SPRITE VARIABLES: initializeSprites()
	Sprite test;
	SDL_Texture * girlEnemyWalkingSpriteSheetTexture,
		*linkEnemyWalkingSpriteSheetTexture,
		*backpackDudeEnemyWalkingSpriteSheetTexture,
		*princessEnemyWalkingSpriteSheetTexture,
		*golemEnemyWalkingSpriteSheetTexture,
		*ufoEnemyWalkingSpriteSheetTexture,
		*maleChildEnemyWalkingSpriteSheetTexture,
		*boysAndFoxesWalkingSpriteSheetTexture/*redBoyEnemy, blueBoyEnemy, greenBoyEnemy, greenFoxEnemy, blackFoxEnemy*/,
		*lizardKidWalkingSpriteSheetTexture,
		*cameleonKidWalkingSpriteSheetTexture,
		*wolfKidWalkingSpriteSheetTexture,
		*tigerKidWalkingSpriteSheetTexture,
		*foxKidWalkingSpriteSheetTexture;

	AnimatedSprite welcomeScreenInterfaceBackground, towerShopInterfaceBackground;
	AnimatedSprite testSprite;
	AnimatedSprite pichu, squirtle, bird, smurf, volt;
	AnimatedSprite enemyDownAnimation, enemyLeftAnimation, enemyRightAnimation, enemyUpAnimation;
	AnimatedSprite yellowTowerBase, yellowTowerCannon1, yellowTowerCannon2, yellowTowerCannon3, pinkTowerBase, pinkTowerCannon1, pinkTowerCannon2, pinkTowerCannon3, blackTowerBase, blackTowerCannon1, blackTowerCannon2, blackTowerCannon3;
	AnimatedSprite lightningTowerSprite;
	AnimatedSprite explosion;
	AnimatedSprite linkDownAnimation, linkUpAnimation, linkRightAnimation, linkLeftAnimation;
	AnimatedSprite princessDownAnimation, princessUpAnimation, princessRightAnimation, princessLeftAnimation;
	AnimatedSprite backpackDudeDownAnimation, backpackDudeUpAnimation, backpackDudeRightAnimation, backpackDudeLeftAnimation;
	AnimatedSprite golemDownAnimation, golemUpAnimation, golemRightAnimation, golemLeftAnimation;
	std::vector<AnimatedSprite> ufoAnimations;
	std::vector<AnimatedSprite> maleChildAnimations;
	std::vector<AnimatedSprite> redBoyAnimations;
	std::vector<AnimatedSprite> blueBoyAnimations;
	std::vector<AnimatedSprite> greenBoyAnimations;
	std::vector<AnimatedSprite> greenFoxAnimations;
	std::vector<AnimatedSprite> blackFoxAnimations;
	std::vector<AnimatedSprite> lizardKidAnimations;
	std::vector<AnimatedSprite> cameleonKidAnimations;
	std::vector<AnimatedSprite> wolfKidAnimations;
	std::vector<AnimatedSprite> tigerKidAnimations;
	std::vector<AnimatedSprite> foxKidAnimations;
	AnimatedSprite robot, bat;
	AnimatedSprite lightningTeslaCoilEffectSprite;

	//TEXT VARIABLES: initializeText()
	Text fpsText;

	//TIME VARIABLES: initializeTimeVariables();
	int gameStartTime, elapsedGameTime, timeOfLastClockTextChange;//
	Text clockText;//IN-GAME CLOCK
	int currentTime, deltaTime, lastTimeFPS;//GLOBAL TIME VARIABLES
	int nFPS;//Keeps track of the number of frames within a given time

	//BUTTONS: initializeButtons()
	Button buttonManager;

	//MAP: initializeMap()
	Map map;
	bool editMapMode = false;
	AnimatedSprite bugMap;

	//PLAYER: initializePlayer()
	Player mainPlayer;

	//PROJECTILE SPRITES
	std::vector<AnimatedSprite> projectileSprites;

	//TOWERS: 
	Tower yellowTower, blueTower, redTower, bladeThrowingTower, lightningTower;
	std::vector<Tower> uniqueTowers;//Contains all of the unique towers
	std::vector<Tower> towersOnField;//Contains all of the towers on the field
	void addTower(int n);

	//ENEMIES: initializeEnemies()
	Enemy girlEnemy, 
		linkEnemy, 
		princessEnemy,
		backpackDudeEnemy,
		batEnemy, 
		golemEnemy,
		ufoEnemy,
		maleChildEnemy,
		redBoyEnemy,
		blueBoyEnemy,
		greenBoyEnemy,
		greenFoxEnemy,
		blackFoxEnemy,
		lizardKidEnemy,
		cameleonKidEnemy,
		wolfKidEnemy,
		tigerKidEnemy,
		foxKidEnemy;
	std::vector<Enemy> enemies;
//	std::vector<std::vector<Enemy>> optomizedEnemyVector;
	void addEnemy();
	std::vector<Enemy*> tempEnemies;
	std::vector<Enemy *> getEnemies(int tileI, int tileJ);
	int timeBetweenSpawns, timeOfLastSpawn;

	//WAVES: initializeWaves()
	int timeLastWaveWasCleared, timeBetweenWaves, timeUntilNextWave;
	int timeOfLastWaveAnnimation, durrationOfWaveAnimation;
	bool playWaveAnimation, boolWaveCompleted;
	std::vector<Text> waveText;
	int currentWave;
	std::vector<std::vector<Enemy>> waves;
	
	//INTERFACE
	Interface welcomeScreenInterface;
	Interface towerShopInterface;

	int gameState; //0: Welcome Screen, 1: start game, 
	bool startRound;

	//Spark Effect
	AnimatedSprite spark;

	///////////////////////////////////////
	//			RENDER FUNCTIONS
	///////////////////////////////////////
	void renderMap();
	void renderEnemies();
	void renderTowersOnField();
	void renderWaves();//Controls the rendering of the wave flyby animation when a wave changes

	///////////////////////////////////////
	//			UPDATE FUNCTIONS
	///////////////////////////////////////
	void updateGameTime();
	void updateClock();
	void updateWindow();//Updates items based on the withdow being resized
	void updateConsole();
	void updateFPS();
	void updateEnemies();
	void updateTowers();
	void updateTowerShopInterface();
	void updatePlayer();
	void updateWaves();
	void updateDeveloperTools();
};

