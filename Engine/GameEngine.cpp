#include "GameEngine.h"

GameEngine::GameEngine()
{
	elapsedGameTime = 0;
	gameStartTime = 0;
	timeOfLastClockTextChange = 0;
	gameState = 1; //0: welcome Screen, 1: Play game
	startRound = false;
}

GameEngine::~GameEngine()
{
}

void GameEngine::initialize()
{
	initializeWindow();
	initializeInput();
	initializeTimeVariables();
	initializeConsole();
	initializeSprites();
	initializeMap();
	initializePlayer();
	initializeEnemies();
	initializeText();
	initializeButtons();
	initializeInterfaces();
	initializeWaves();

	//TEST

}

void GameEngine::handleInput()
{
	controls.update();//User input (clicks, key events)
}

void GameEngine::update()
{
	if (controls.keyPressed(SDL_SCANCODE_6))//Place this in the interface class to disable the interface along with the buttons
	{
		if (buttonManager.isButtonsDisabled())
		{
			buttonManager.enableButtons();
		}
		else
		{
			buttonManager.disableAllButtons();
		}
	}

	//NESSESARY UPDATE FUNCTIONS: updateImportant()
	buttonManager.update();//CONTAINS DEVELOPMENT TOOLS RELATED BUTTONS
	updateWindow();
	updateGameTime();
	updateConsole();
	updateFPS();
	updateDeveloperTools();

	testUpdate();

	switch (gameState)
	{
	case 0:
		//WELCOME SCREEN: updateWelcomeScreen()
		welcomeScreenInterface.update();
		if (welcomeScreenInterface.isButtonOn("Play"))
		{
			timeLastWaveWasCleared = elapsedGameTime;
			gameState = 1;//changes gameState to the playing the TD game
		}
		else if (welcomeScreenInterface.isButtonOn("Quit"))
		{
			run = false;
		}
		break;
	case 1:
		//MAIN GAME: updateMainGame()
		updateClock();
		updateTowerShopInterface();
		updateEnemies();
		updateWaves();
		updateTowers();
		break;
	default:
		developerConsole.insertMessage("Invalid Game State in GameEngine::update()");
	}
}

void GameEngine::render()
{
	SDL_RenderClear(mainWindow.getRenderer());//DO NOT DELETE

	switch (gameState)
	{
	case 0:
		//WELCOME SCREEN: renderWelcomeScreen()
		welcomeScreenInterface.render();
		break;
	case 1:
		//MAIN GAME: renderMainGame()
		towerShopInterface.render();
		renderMap();
		renderEnemies();
		renderTowersOnField();
		renderWaves();
		clockText.renderAt(500,0);
		break;
	default:
		developerConsole.insertMessage("Invalid Game State in GameEngine::render()");
	}

	buttonManager.render();


	developerConsole.renderConsole();
	fpsText.renderAt(mainWindow.getWidth() - fpsText.getWidth() - 10, 10);//The fps is rendered at the top right corner of the screen
	SDL_RenderPresent(mainWindow.getRenderer());
}

bool GameEngine::isRunning()
{
	return run;
}

void GameEngine::close()
{
	mainWindow.close();
}

void GameEngine::changeToWave(int w)
{
	timeLastWaveWasCleared = elapsedGameTime;
	timeUntilNextWave = timeBetweenWaves;
	//CHANGES enemies TO THE NEXT WAVES ENEMIES
	if (w >= waves.size() || w < 0)
	{
		developerConsole.insertMessage("Invalid wave number " + std::to_string(w + 1) + ". Wave reset to 1.");
		w = 0;
	}
	else
	{
		developerConsole.insertMessage("Wave changed to " + std::to_string(w + 1) + ".");
	}

	currentWave = w % waves.size();

	for (int i = 0; i < towersOnField.size(); i++)
	{
		towersOnField[i].reset();
	}
	enemies.clear();
	enemies = waves[currentWave];

	//ANNIMATION INITIALIZATION
	playWaveAnimation = true;
	timeOfLastWaveAnnimation = currentTime;
	for (int i = 0; i < waveText.size(); i++)
	{
		switch (i)
		{
		case 0://"Wave: ???" TEXT
			waveText[i].setOutputText("Wave:" + std::to_string(currentWave + 1));
			waveText[i].setOpacity(255);
			waveText[i].setPosition(map.getMapWidth() - waveText[i].getWidth(), (map.getMapHeight() - waveText[i].getHeight()) / 2);
			break;
		case 1://"Enemies name" TEXT BELLOW "Wave: ???" TEXT
			waveText[i].setOutputText(waves[currentWave][0].getName());
			waveText[i].setOpacity(255);
			waveText[i].setPosition(map.getMapWidth() - (waveText[0].getWidth() + waveText[i].getWidth()) / 2, (map.getMapHeight() + waveText[0].getHeight()) / 2);
			break;
		}
	}
}

void GameEngine::testUpdate()
{
}

void GameEngine::initializeWindow()
{
	mainWindow = Utility(SCREEN_WIDTH, SCREEN_HEIGHT);
	mainWindow.createWindow();
	SDL_SetRenderDrawBlendMode(mainWindow.getRenderer(), SDL_BLENDMODE_BLEND);
}

void GameEngine::initializeInput()
{
	controls = Interaction(&e);
}

void GameEngine::initializeConsole()
{
	developerConsole = Console(mainWindow.getRenderer(), "./images/consoleImage.png", &mainWindow);;
}

void GameEngine::initializeSprites()
{
	//TEXTURES


	//TEST SPRITES
	testSprite = AnimatedSprite(&currentTime);
//	explosion = AnimatedSprite("./images/explosion.png", mainWindow.getRenderer(), &currentTime, 0, 0, 110, 11, 10, 640, 704, 200, 100);
	explosion.setTimeBetweenAnimations(1);

//	spark = AnimatedSprite("./images/TowerDefenceSprites/EnemySprites/spark_Effect.png", mainWindow.getRenderer(), &currentTime, 0, 0, 16, 4, 4, 728, 824, 200, 100);
	spark.setW(50);
	spark.setH(50);
	spark.setTimeBetweenAnimations(30);

	std::vector<std::string> tempString;
	for (int i = 1; i < 10; i++)
	{
		tempString.push_back("./images/TowerDefenceSprites/TowerSprites/bolt_tesla/bolt_tesla_000" + std::to_string(i) + ".png");
	}
	tempString.push_back("./images/TowerDefenceSprites/TowerSprites/bolt_tesla/bolt_tesla_0010.png");
	lightningTeslaCoilEffectSprite = AnimatedSprite(tempString, mainWindow.getRenderer(), &currentTime, 512, 512, 200, 120);
	lightningTeslaCoilEffectSprite.setW(28);
	lightningTeslaCoilEffectSprite.setH(28);
	lightningTeslaCoilEffectSprite.rotate(-30);
	lightningTeslaCoilEffectSprite.setTimeBetweenAnimations(50);
//	lightningTeslaCoilEffectSprite.setPosition(400 + (40 - lightningTeslaCoilEffectSprite.getW()) / 2 - 1, 200 - lightningTeslaCoilEffectSprite.getH() / 2 + 8);

	//MAP SPRITES
	bugMap = AnimatedSprite("./images/TowerDefenceSprites/MapSprites/BugMap.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 607, 353, 0, 0);
	bugMap.setX(0);
	bugMap.setY(0);
	bugMap.setW(1000);
	bugMap.setH(600);
	bugMap.changeColor({200,200,200});
//	bugMap.changeOpactity(150);

	//INTERFACE SPRITES
	towerShopInterfaceBackground = AnimatedSprite("./images/TowerDefenceSprites/Interface Sprites/SideBarInterface.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 503, 717, 1000, 0);
	towerShopInterfaceBackground.setW(225);
	towerShopInterfaceBackground.setH(600);

	welcomeScreenInterfaceBackground = AnimatedSprite("./images/TowerDefenceSprites/wecomeScreen.jpg", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 1280, 720, 0, 0);
	welcomeScreenInterfaceBackground.setW(1000 + 225);
	welcomeScreenInterfaceBackground.setH(600);


	//PROJECTILE SPRITES
	projectileSprites.push_back(AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/bullet1.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 5, 5, 400, 540));
	projectileSprites.push_back(AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/bullet2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 25, 10, 400, 540));
	projectileSprites[projectileSprites.size() - 1].setH(5);
	projectileSprites[projectileSprites.size() - 1].setW(15);
	projectileSprites.push_back(AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/bullet3.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 9, 9, 400, 540));
	projectileSprites.push_back(AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/spinning_blade.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 397, 397, 400, 540));
	projectileSprites[projectileSprites.size() - 1].setH(40);
	projectileSprites[projectileSprites.size() - 1].setW(40);

	//TOWER SPRITES
	pinkTowerBase = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/Tower1.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 50, 50, 400, 540);
	pinkTowerBase.setH(40);
	pinkTowerBase.setW(40);
	pinkTowerCannon1 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/SplitShotTower_1.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 59, 60, 400, 540);
	pinkTowerCannon1.setW(40);
	pinkTowerCannon1.setH(40);
	pinkTowerCannon2 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/SplitShotTower_2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 59, 60, 400, 540);
	pinkTowerCannon2.setW(40);
	pinkTowerCannon2.setH(40);
	pinkTowerCannon3 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/SplitShotTower_3.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 59, 60, 400, 540);
	pinkTowerCannon3.setW(40);
	pinkTowerCannon3.setH(40);

	yellowTowerBase = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/Tower2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 50, 50, 400, 540);
	yellowTowerBase.setH(40);
	yellowTowerBase.setW(40);
	yellowTowerCannon1 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/cannon2Upgrade2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 80, 80, 400, 540);
	yellowTowerCannon1.setW(40);
	yellowTowerCannon1.setH(40);
	yellowTowerCannon2 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/cannon2Upgrade2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 80, 80, 400, 540);
	yellowTowerCannon2.setW(40);
	yellowTowerCannon2.setH(40);
	yellowTowerCannon3 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/cannon2Upgrade2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 80, 80, 400, 540);
	yellowTowerCannon3.setW(40);
	yellowTowerCannon3.setH(40);

	blackTowerBase = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/Tower3.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 50, 50, 400, 540);
	blackTowerBase.setH(40);
	blackTowerBase.setW(40);
	blackTowerCannon1 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/LongRangeTower_1.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 80, 80, 400, 540);
	blackTowerCannon1.setW(40);
	blackTowerCannon1.setH(40);
	blackTowerCannon2 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/LongRangeTower_2.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 80, 80, 400, 540);
	blackTowerCannon2.setW(40);
	blackTowerCannon2.setH(40);
	blackTowerCannon3 = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/LongRangeTower_3.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 80, 80, 400, 540);
	blackTowerCannon3.setW(40);
	blackTowerCannon3.setH(40);

	lightningTowerSprite = AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/Lightning_Tower.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 165, 262, 400, 540);
	lightningTowerSprite.setW(165 * 40 / 262);
	lightningTowerSprite.setH(262 * 40 / 262);

	//TOWERS INITIALIZED
	blueTower = Tower(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, { yellowTowerCannon1,yellowTowerCannon2,yellowTowerCannon3 }, yellowTowerBase, projectileSprites[0], "Blue Tower", "Description", { 29, 40, 50 }, { 10, 15, 20 }, { 300, 300, 300 }, { 100,100,100 }, { 1,2,3 }, 400, 400, TOWER_TYPE_BULLET);
	redTower = Tower(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, { pinkTowerCannon1,pinkTowerCannon2,pinkTowerCannon3 }, pinkTowerBase, projectileSprites[1], "Split Shot Tower", "Description", { 29, 40, 50 }, { 10, 15, 20 }, { 300, 300, 300 }, { 100,100,100 }, { 1,2,3 }, 400, 400, TOWER_TYPE_BULLET);
	yellowTower = Tower(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, { blackTowerCannon1,blackTowerCannon2,blackTowerCannon3 }, blackTowerBase, projectileSprites[2], "Black and Yellow Tower", "Description", { 29, 40, 50 }, { 10, 15, 20 }, { 200, 250, 300 }, { 100,100,100 }, { 1,1,1 }, 400, 400, TOWER_TYPE_BULLET);
	bladeThrowingTower = Tower(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, { AnimatedSprite("./images/TowerDefenceSprites/TowerSprites/spinning_blade_arm.png", mainWindow.getRenderer(), &currentTime, 0, 0, 1, 1, 1, 276, 33, 400, 540) }, blackTowerBase, projectileSprites[3], "Lumberjack Tower", "Rotates and does fast contact damage", { 20, 20, 20, 20, 20, 20, 20 }, { 20, 20, 20, 20, 20, 20, 20 }, { 140, 140, 140, 140, 140, 140, 140 }, { 100,100,100, 100, 100, 100, 100 }, { 1,2,3,4, 5, 6, 16}, 400, 400, TOWER_TYPE_SPINNINGBLADE);
	bladeThrowingTower.setTimeBetweenShots(100);
	lightningTower = Tower(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, { lightningTowerSprite }, lightningTowerSprite, projectileSprites[3], "Lightning Tower", "Shocks jump to enemies behind the initial target.", { 100, 200, 300 }, { 20, 25, 30 }, { 200, 250, 275 }, { 100,100,100 }, { 1,2,4 }, 400, 400, TOWER_TYPE_LIGHTNING);
	lightningTower.setEffectSprite(lightningTeslaCoilEffectSprite);
	uniqueTowers.push_back(blueTower);
	uniqueTowers.push_back(redTower);
	uniqueTowers.push_back(yellowTower);
	uniqueTowers.push_back(bladeThrowingTower);
	uniqueTowers.push_back(lightningTower);

	//ENEMY SPRITES
	girlEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/girlEnemy.png");
	enemyDownAnimation = AnimatedSprite(girlEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 0, 8, 1, 8, 480, 60, 200, 100);
	enemyLeftAnimation = AnimatedSprite(girlEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 60, 8, 1, 8, 480, 60, 200, 300);
	enemyRightAnimation = AnimatedSprite(girlEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 120, 8, 1, 8, 480, 60, 200, 440);
	enemyUpAnimation = AnimatedSprite(girlEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 180, 8, 1, 8, 480, 60, 200, 540);
	enemyDownAnimation.setW(40);
	enemyDownAnimation.setH(40);
	enemyLeftAnimation.setW(40);
	enemyLeftAnimation.setH(40);
	enemyRightAnimation.setW(40);
	enemyRightAnimation.setH(40);
	enemyUpAnimation.setW(40);
	enemyUpAnimation.setH(40);

	int tempW = 40/* 120 * */, tempH = 43;
	//DIMENSIONS: 120x130
	linkEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/link.png");
	linkUpAnimation = AnimatedSprite(linkEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 780, 10, 8, 10, 1200, 1040, 100, 0);
	linkUpAnimation.setW(tempW);
	linkUpAnimation.setH(tempH);
	linkDownAnimation = AnimatedSprite(linkEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 520, 10, 8, 10, 1200, 1040, 100, 0);
	linkDownAnimation.setW(tempW);
	linkDownAnimation.setH(tempH);
	linkLeftAnimation = AnimatedSprite(linkEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 650, 10, 8, 10, 1200, 1040, 100, 0);
	linkLeftAnimation.setW(tempW);
	linkLeftAnimation.setH(tempH);
	linkRightAnimation = AnimatedSprite(linkEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 910, 10, 8, 10, 1200, 1040, 100, 0);
	linkRightAnimation.setW(tempW);
	linkRightAnimation.setH(tempH);

	tempW = 40, tempH = 40;
	//DIMENSIONS: 64x64
	princessEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/princess.png");
	princessUpAnimation = AnimatedSprite(princessEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 0, 9, 4, 9, 576, 256, 200, 0);
	princessUpAnimation.setW(tempW);
	princessUpAnimation.setH(tempH);
	princessDownAnimation = AnimatedSprite(princessEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 128, 9, 4, 9, 576, 256, 200, 0);
	princessDownAnimation.setW(tempW);
	princessDownAnimation.setH(tempH);
	princessLeftAnimation = AnimatedSprite(princessEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 64, 9, 4, 9, 576, 256, 200, 0);
	princessLeftAnimation.setW(tempW);
	princessLeftAnimation.setH(tempH);
	princessRightAnimation = AnimatedSprite(princessEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192, 9, 4, 9, 576, 256, 200, 0);
	princessRightAnimation.setW(tempW);
	princessRightAnimation.setH(tempH);

	tempW = 95 / 4, tempH = 159 / 4;
	//DIMENSIONS: 95x159
	backpackDudeEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/backpackDude.png");
	backpackDudeUpAnimation = AnimatedSprite(backpackDudeEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 477, 12, 4, 12, 1142, 635, 300, 0);
	backpackDudeUpAnimation.setW(tempW);
	backpackDudeUpAnimation.setH(tempH);
	backpackDudeDownAnimation = AnimatedSprite(backpackDudeEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 0, 12, 4, 12, 1142, 635, 300, 0);
	backpackDudeDownAnimation.setW(tempW);
	backpackDudeDownAnimation.setH(tempH);
	backpackDudeLeftAnimation = AnimatedSprite(backpackDudeEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 159, 12, 4, 12, 1142, 635, 300, 0);
	backpackDudeLeftAnimation.setW(tempW);
	backpackDudeLeftAnimation.setH(tempH);
	backpackDudeRightAnimation = AnimatedSprite(backpackDudeEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 318, 12, 4, 12, 1142, 635, 300, 0);
	backpackDudeRightAnimation.setW(tempW);
	backpackDudeRightAnimation.setH(tempH);

	tempW = 60;
	tempH = 60;
	//DIMENSIONS: 96x96
	golemEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/BOSS.png");
	golemUpAnimation = AnimatedSprite(golemEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 288, 3, 4, 3, 288, 384, 300, 0);
	golemUpAnimation.setW(tempW);
	golemUpAnimation.setH(tempH);
	golemDownAnimation = AnimatedSprite(golemEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 0, 3, 4, 3, 288, 384, 300, 0);
	golemDownAnimation.setW(tempW);
	golemDownAnimation.setH(tempH);
	golemLeftAnimation = AnimatedSprite(golemEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 96, 3, 4, 3, 288, 384, 300, 0);
	golemLeftAnimation.setW(tempW);
	golemLeftAnimation.setH(tempH);
	golemRightAnimation = AnimatedSprite(golemEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192, 3, 4, 3, 288, 384, 300, 0);
	golemRightAnimation.setW(tempW);
	golemRightAnimation.setH(tempH);
	
	ufoEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/UFO.png");
	for (int i = 0; i < 4; i++)
	{
		ufoAnimations.push_back(AnimatedSprite(ufoEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * i, 3, 4, 3, 144, 192, 400, 0));
		ufoAnimations[ufoAnimations.size() - 1].setTimeBetweenAnimations(200);
	}

	maleChildEnemyWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/maleChild.png");
	maleChildAnimations.push_back(AnimatedSprite(maleChildEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 256 / 4 * 2, 6, 4, 6, 192, 256, 400, 0));
	maleChildAnimations.push_back(AnimatedSprite(maleChildEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 256 / 4 * 3, 6, 4, 6, 192, 256, 400, 0));
	maleChildAnimations.push_back(AnimatedSprite(maleChildEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 256 / 4 * 1, 6, 4, 6, 192, 256, 400, 0));
	maleChildAnimations.push_back(AnimatedSprite(maleChildEnemyWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 256 / 4 * 0, 6, 4, 6, 192, 256, 400, 0));
	for (int i = 0; i < maleChildAnimations.size(); i++)
	{
		maleChildAnimations[i].setTimeBetweenAnimations(100);
	}

	boysAndFoxesWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/BoysAndFoxes.png");
	redBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 0, 0 + 256 / 2 / 4 * 1, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	redBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 0, 0 + 256 / 2 / 4 * 2, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	redBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 0, 0 + 256 / 2 / 4 * 3, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	redBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 0, 0 + 256 / 2 / 4 * 0, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	for (int i = 0; i < redBoyAnimations.size(); i++)
	{
		redBoyAnimations[i].setTimeBetweenAnimations(150);
	}

	blueBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 1, 0 + 256 / 2 / 4 * 1, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	blueBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 1, 0 + 256 / 2 / 4 * 2, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	blueBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 1, 0 + 256 / 2 / 4 * 3, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	blueBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 1, 0 + 256 / 2 / 4 * 0, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	for (int i = 0; i < blueBoyAnimations.size(); i++)
	{
		blueBoyAnimations[i].setTimeBetweenAnimations(150);
	}

	greenBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 2, 0 + 256 / 2 / 4 * 1, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	greenBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 2, 0 + 256 / 2 / 4 * 2, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	greenBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 2, 0 + 256 / 2 / 4 * 3, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	greenBoyAnimations.push_back(AnimatedSprite(boysAndFoxesWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 292 / 3 * 2, 0 + 256 / 2 / 4 * 0, 3, 4, 3, 292 / 3, 256 / 2, 400, 0));
	for (int i = 0; i < greenBoyAnimations.size(); i++)
	{
		greenBoyAnimations[i].setTimeBetweenAnimations(150);
	}

	lizardKidWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/LizardKid.png");
	lizardKidAnimations.push_back(AnimatedSprite(lizardKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 1, 3, 4, 3, 144, 192, 400, 0));
	lizardKidAnimations.push_back(AnimatedSprite(lizardKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 2, 3, 4, 3, 144, 192, 400, 0));
	lizardKidAnimations.push_back(AnimatedSprite(lizardKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 3, 3, 4, 3, 144, 192, 400, 0));
	lizardKidAnimations.push_back(AnimatedSprite(lizardKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 0, 3, 4, 3, 144, 192, 400, 0));
	for (int i = 0; i < lizardKidAnimations.size(); i++)
	{
		lizardKidAnimations[i].setTimeBetweenAnimations(150);
	}

	cameleonKidWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/CameleonKid.png");
	cameleonKidAnimations.push_back(AnimatedSprite(cameleonKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 1, 3, 4, 3, 144, 192, 400, 0));
	cameleonKidAnimations.push_back(AnimatedSprite(cameleonKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 2, 3, 4, 3, 144, 192, 400, 0));
	cameleonKidAnimations.push_back(AnimatedSprite(cameleonKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 3, 3, 4, 3, 144, 192, 400, 0));
	cameleonKidAnimations.push_back(AnimatedSprite(cameleonKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 0, 3, 4, 3, 144, 192, 400, 0));
	for (int i = 0; i < cameleonKidAnimations.size(); i++)
	{
		cameleonKidAnimations[i].setTimeBetweenAnimations(150);
	}

	wolfKidWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/WolfKid.png");
	wolfKidAnimations.push_back(AnimatedSprite(wolfKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 1, 3, 4, 3, 144, 192, 400, 0));
	wolfKidAnimations.push_back(AnimatedSprite(wolfKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 2, 3, 4, 3, 144, 192, 400, 0));
	wolfKidAnimations.push_back(AnimatedSprite(wolfKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 3, 3, 4, 3, 144, 192, 400, 0));
	wolfKidAnimations.push_back(AnimatedSprite(wolfKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 0, 3, 4, 3, 144, 192, 400, 0));
	for (int i = 0; i < wolfKidAnimations.size(); i++)
	{
		wolfKidAnimations[i].setTimeBetweenAnimations(150);
	}

	tigerKidWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/TigerKid.png");
	tigerKidAnimations.push_back(AnimatedSprite(tigerKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 1, 3, 4, 3, 144, 192, 400, 0));
	tigerKidAnimations.push_back(AnimatedSprite(tigerKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 2, 3, 4, 3, 144, 192, 400, 0));
	tigerKidAnimations.push_back(AnimatedSprite(tigerKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 3, 3, 4, 3, 144, 192, 400, 0));
	tigerKidAnimations.push_back(AnimatedSprite(tigerKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 0, 3, 4, 3, 144, 192, 400, 0));
	for (int i = 0; i < tigerKidAnimations.size(); i++)
	{
		tigerKidAnimations[i].setTimeBetweenAnimations(150);
	}

	foxKidWalkingSpriteSheetTexture = IMG_LoadTexture(mainWindow.getRenderer(), "./images/TowerDefenceSprites/EnemySprites/FoxKid.png");
	foxKidAnimations.push_back(AnimatedSprite(foxKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 1, 3, 4, 3, 144, 192, 400, 0));
	foxKidAnimations.push_back(AnimatedSprite(foxKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 2, 3, 4, 3, 144, 192, 400, 0));
	foxKidAnimations.push_back(AnimatedSprite(foxKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 3, 3, 4, 3, 144, 192, 400, 0));
	foxKidAnimations.push_back(AnimatedSprite(foxKidWalkingSpriteSheetTexture, mainWindow.getRenderer(), &currentTime, 0, 192 / 4 * 0, 3, 4, 3, 144, 192, 400, 0));
	for (int i = 0; i < foxKidAnimations.size(); i++)
	{
		foxKidAnimations[i].setTimeBetweenAnimations(150);
	}


	robot = AnimatedSprite("./images/TowerDefenceSprites/EnemySprites/robot.png", mainWindow.getRenderer(), &currentTime, 0, 0, 36, 3, 12, 768, 192, 400, 0);
	bat = AnimatedSprite("./images/TowerDefenceSprites/EnemySprites/bat.png", mainWindow.getRenderer(), &currentTime, 0, 0, 16, 4, 4, 192, 192, 500, 0);
	bird = AnimatedSprite("./images/bird.png", mainWindow.getRenderer(), &currentTime, 0, 0, 14, 3, 5, 551, 304, 200, 300);
	smurf = AnimatedSprite("./images/smurf_sprite.png", mainWindow.getRenderer(), &currentTime, 0,0, 16, 4, 4, 512, 512, 300, 300);
	volt = AnimatedSprite("./images/volt_running.png", mainWindow.getRenderer(), &currentTime, 0,0, 10, 2, 5, 900, 495, 500, 300);
}

void GameEngine::initializeMap()
{
	map = Map(&developerConsole, &controls, mainWindow.getRenderer());
	map.loadMapFromFile("./Maps/bugMap.txt");
}

void GameEngine::initializeEnemies()
{
	timeBetweenSpawns = 1000;
	timeOfLastSpawn = 0;

	girlEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Running Girl", 3, 400, { enemyLeftAnimation,enemyRightAnimation,enemyUpAnimation, enemyDownAnimation }, &mainPlayer);
	linkEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Link", 3, 100, { linkLeftAnimation, linkRightAnimation, linkUpAnimation, linkDownAnimation }, &mainPlayer);
	princessEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Princess", 3, 100, { princessLeftAnimation,princessRightAnimation,princessUpAnimation, princessDownAnimation }, &mainPlayer);
	backpackDudeEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Cameron", 1, 100, { backpackDudeLeftAnimation,backpackDudeRightAnimation,backpackDudeUpAnimation, backpackDudeDownAnimation }, &mainPlayer);
	golemEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Golem", 4, 100, { golemLeftAnimation,golemRightAnimation, golemUpAnimation, golemDownAnimation }, &mainPlayer);
	ufoEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "UFO", 3, 100, ufoAnimations, &mainPlayer);
	maleChildEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Child", 3, 100, maleChildAnimations, &mainPlayer);
	redBoyEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Red Boy", 3, 100, redBoyAnimations, &mainPlayer);
	blueBoyEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Blue Boy", 3, 100, blueBoyAnimations, &mainPlayer);
	greenBoyEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Green Boy", 3, 100, greenBoyAnimations, &mainPlayer);
	lizardKidEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Lizard Kid", 3, 100, lizardKidAnimations, &mainPlayer);
	cameleonKidEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Cameleon Kid", 3, 100, cameleonKidAnimations, &mainPlayer);
	wolfKidEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Wolf Kid", 3, 100, wolfKidAnimations, &mainPlayer);
	tigerKidEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Tiger Kid", 3, 100, tigerKidAnimations, &mainPlayer);
	foxKidEnemy = Enemy(mainWindow.getRenderer(), &controls, &developerConsole, &currentTime, &map, "Fox Kid", 3, 100, foxKidAnimations, &mainPlayer);

	for (int i = 0; i < 4; i++)
	{
		enemies.push_back(girlEnemy);
//		enemies[i].setPosition(-1 * i * girlEnemy.getHitBox().getW(), 0);
		enemies[i].setPosition(-100,-100);
		enemies[i].setMap(&map);
	}
//	enemies[0].setPosition(0,600);
}

void GameEngine::initializeText()
{
	clockText = Text(mainWindow.getRenderer(), "./fonts/cinema_st.ttf", 30, { 0,0,0 }, "00:00");

	fpsText = Text(mainWindow.getRenderer(), "./fonts/Quicksand_Regular.otf", 12, { 0,255,255 }, "Calculating FPS...");
	
	waveText.push_back(Text(mainWindow.getRenderer(), "./fonts/Impact Label.ttf", 72, { 0,0,0 }, "Wave:"));
	waveText.push_back(Text(mainWindow.getRenderer(), "./fonts/Impact Label.ttf", 30, { 0,0,0 }, "EnemyName"));


}

void GameEngine::initializeTimeVariables()
{
	currentTime = 0, deltaTime = 0, lastTimeFPS = 0;
	nFPS = 0;
}

void GameEngine::initializeButtons()
{
	buttonManager = Button(mainWindow.getRenderer(), &controls, &developerConsole, SCREEN_WIDTH, SCREEN_HEIGHT);
	buttonManager.setGenericButtonPositions(1275, 100);
	buttonManager.setNCols(1);
	buttonManager.createButton("BEGIN", false);
	buttonManager.createButton("Welcome Screen", false);
	buttonManager.createButton("EDIT MAP", true);
	buttonManager.createButton("SAVE AS FIRST MAP", false);
	buttonManager.createButton("LOAD FIRST MAP", false);
	buttonManager.createButton("SAVE AS SECOND MAP", false);
	buttonManager.createButton("LOAD SECOND MAP", false);
	buttonManager.createButton("SAVE AS THIRD MAP", false);
	buttonManager.createButton("LOAD THIRD MAP", false);
	buttonManager.createButton("NEXT WAVE", false);
	buttonManager.createButton("GIVE GOLD", false);
	buttonManager.createButton("EXIT", false);
}

void GameEngine::initializeInterfaces()
{
	//WELCOME SCREEN
	welcomeScreenInterface = Interface(mainWindow.getRenderer(), &controls, &developerConsole, welcomeScreenInterfaceBackground, "Welcome Screen", 1000, 0);
//	welcomeScreenInterface.addButton("Play", false, 200, 150, 50, 20);
	welcomeScreenInterface.setGenericButtonsPositions(200, 150);
	welcomeScreenInterface.addButtons({ "Play" }, { false });
	welcomeScreenInterface.addButtons({ "Unlocks" }, { false });
	welcomeScreenInterface.addButtons({ "Select Map Left" }, { false });
	welcomeScreenInterface.addButtons({ "Select Map Right" }, { false });
	welcomeScreenInterface.addButtons({ "Quit" }, { false });

	//TOWER SHOP
	towerShopInterface = Interface(mainWindow.getRenderer(), &controls, &developerConsole,towerShopInterfaceBackground, "Tower Shop", 1000,0);
	towerShopInterface.setTowersOnFieldPtr(&towersOnField);
	towerShopInterface.setEnemiesOnFieldPtr(&enemies);
	towerShopInterface.addButtons({"---", "T1", "T2", "T3", "T4", "T5"}, {false, false, false, false, false, false});
}

void GameEngine::initializePlayer()
{
	mainPlayer = Player(&developerConsole);
	mainPlayer.setHealth(100);
	mainPlayer.setCash(1000);
}

void GameEngine::initializeWaves()
{
	currentWave = 0;
	int tempNumberOfEnemies = 15;
	for (int i = 0; i < tempNumberOfEnemies; i++)//NUMBER OF WAVES
	{
		waves.push_back(std::vector<Enemy>());
		for (int j = 0; j < 10; j++)//NUMBER OF ENEMIES PER WAVE
		{
			if (i % tempNumberOfEnemies == 0)
			{
				waves[i].push_back(girlEnemy);
			}
			else if (i % tempNumberOfEnemies == 1)
			{
				waves[i].push_back(linkEnemy);
			}
			else if (i % tempNumberOfEnemies == 2)
			{
				waves[i].push_back(princessEnemy);
			}
			else if (i % tempNumberOfEnemies == 3)
			{
				waves[i].push_back(backpackDudeEnemy);
			}
			else if (i % tempNumberOfEnemies == 4)
			{
				waves[i].push_back(golemEnemy);
			}
			else if (i % tempNumberOfEnemies == 5)
			{
				waves[i].push_back(ufoEnemy);
			}
			else if (i % tempNumberOfEnemies == 6)
			{
				waves[i].push_back(maleChildEnemy);
			}
			else if (i % tempNumberOfEnemies == 7)
			{
				waves[i].push_back(redBoyEnemy);
			}
			else if (i % tempNumberOfEnemies == 8)
			{
				waves[i].push_back(blueBoyEnemy);
			}
			else if (i % tempNumberOfEnemies == 9)
			{
				waves[i].push_back(greenBoyEnemy);
			}
			else if (i % tempNumberOfEnemies == 10)
			{
				waves[i].push_back(lizardKidEnemy);
			}
			else if (i % tempNumberOfEnemies == 11)
			{
				waves[i].push_back(cameleonKidEnemy);
			}
			else if (i % tempNumberOfEnemies == 12)
			{
				waves[i].push_back(wolfKidEnemy);
			}
			else if (i % tempNumberOfEnemies == 13)
			{
				waves[i].push_back(tigerKidEnemy);
			}
			else if (i % tempNumberOfEnemies == 14)
			{
				waves[i].push_back(foxKidEnemy);
			}
		}
	}
	enemies = waves[currentWave];

	//ANNIMATION TO PLAY BETWEEN WAVES
	playWaveAnimation = false;
	timeOfLastWaveAnnimation = 0;
	durrationOfWaveAnimation = 4200;

	boolWaveCompleted = false;
	timeLastWaveWasCleared = 0;
	timeBetweenWaves = 999;
}

void GameEngine::addTower(int n)
{
	if (n < 0 || n >= uniqueTowers.size())
	{
		developerConsole.insertMessage("Invalid entry int n for GameEngine::addTower(int n)");
	}
	else
	{
		if (towersOnField.size() == 0)
		{
			if (mainPlayer.getCash() >= uniqueTowers[n].getCost())
			{
				mainPlayer.setCash(mainPlayer.getCash() - uniqueTowers[n].getCost());
				towersOnField.push_back(uniqueTowers[n]);
				towersOnField[towersOnField.size() - 1].setMap(&map);
			}
		}
		else if (!towersOnField[towersOnField.size() - 1].isSelected())
		{
			if (mainPlayer.getCash() >= uniqueTowers[n].getCost())
			{
				mainPlayer.setCash(mainPlayer.getCash() - uniqueTowers[n].getCost());
				towersOnField.push_back(uniqueTowers[n]);
				towersOnField[towersOnField.size() - 1].setMap(&map);
			}
		}
		else if (towersOnField[towersOnField.size() - 1].isSelected())
		{
			mainPlayer.setCash(mainPlayer.getCash() + towersOnField[towersOnField.size() - 1].getCost());
			towersOnField.pop_back();
			developerConsole.insertMessage("Tower Deselected");
		}
	}
}

void GameEngine::addEnemy()
{
	enemies.push_back(waves[currentWave % waves.size()][0]);
	enemies[enemies.size() - 1].setMap(&map);
}

std::vector<Enemy*> GameEngine::getEnemies(int tileI, int tileJ)
{
	tempEnemies.clear();
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i].getCurrentTileI() == tileI && enemies[i].getCurrentTileJ() == tileJ)
		{
			tempEnemies.push_back(&enemies[i]);
		}
	}
	return tempEnemies;
}

void GameEngine::updateEnemies()
{
	if (controls.keyPressed(SDL_SCANCODE_U))
	{
		addEnemy();
	}

	//SPAWNER
	if (startRound)
	{
		if (timeUntilNextWave > 0)//UPDATES TIME UNTIL NEXT WAVE STARTS
		{
			timeUntilNextWave = timeLastWaveWasCleared + timeBetweenWaves - elapsedGameTime;
		}
		else//SPAWNS ENEMIES
		{
			if (currentTime > timeOfLastSpawn + timeBetweenSpawns && startRound)
			{
				for (int i = 0; i < enemies.size(); i++)
				{
					if (!enemies[i].isSpawned())
					{
						enemies[i].spawn();
						i = enemies.size();
					}
				}
				timeOfLastSpawn = currentTime;
			}
		}
	}

	//UPDATES ALL OF THE ENEMIES POSITIONS
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i].isSpawned() && enemies[i].isAlive())
		{
			enemies[i].updatePosition();
		}
	}

	//CHECKS IF THE WAVE IS CLEARED & STARTS THE NEXT WAVE
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i].isSpawned())
		{
			if (enemies[i].isAlive())
			{
				boolWaveCompleted = false;
				i = enemies.size();
			}
			else
			{
				boolWaveCompleted = true;
			}
		}
	}

	if (boolWaveCompleted)
	{
		changeToWave(currentWave + 1);
		boolWaveCompleted = false;
	}
}

void GameEngine::renderMap()
{
	//MAPS
	map.renderSkeleton();
	map.renderMiniMap();
	bugMap.render();
}

void GameEngine::renderEnemies()
{
	//Render Enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].render();
	}
}

void GameEngine::renderTowersOnField()
{
	//Render Towers
	for (int i = 0; i < towersOnField.size(); i++)
	{
		towersOnField[i].render();
	}

	for (int i = 0; i < towersOnField.size(); i++)
	{
		towersOnField[i].renderProjectiles();
	}
}

void GameEngine::renderWaves()
{
	if (playWaveAnimation)
	{
		for (int i = 0; i < waveText.size(); i++)
		{
			waveText[i].render();
		}
	}
}

void GameEngine::updateGameTime()
{
	//GLOBAL TIMMER
	deltaTime = SDL_GetTicks() - currentTime;
	currentTime += deltaTime;

	//PLAYING A GAME OF TD TIMMER
}

void GameEngine::updateClock()
{
	//Elapsed Game Time
	if (startRound)
	{
		elapsedGameTime = currentTime - gameStartTime;
	}
	else
	{
		elapsedGameTime = 0;
	}

	if (currentTime > timeOfLastClockTextChange + 500)
	{
		timeOfLastClockTextChange = currentTime;
		int tempM = elapsedGameTime / 60000;
		int tempS = (elapsedGameTime % 60000) / 1000;
		std::string tempMin;
		std::string tempSec;

		if (tempM >= 10)
		{
			tempMin = std::to_string(tempM);
		}
		else
		{
			tempMin = "0" + std::to_string(tempM);
		}

		if (tempS >= 10)
		{
			tempSec = std::to_string(tempS);
		}
		else
		{
			tempSec = "0" + std::to_string(tempS);
		}

		clockText.setOutputText(tempMin + ":" + tempSec);
//		developerConsole.insertMessage(tempMin + ":" + tempSec);

		//Time Between Spawns
	}
}

void GameEngine::updateWindow()
{
	if (controls.wasWindowResized())
	{
		developerConsole.updateSizeAndPosition();
	}
}

void GameEngine::updateConsole()
{
	if (controls.keyPressed(SDL_SCANCODE_Q))
	{
		developerConsole.toggleConsoleDisplay();
	}

	//when the console is displayed/Toggled On, then the consols's textbox will be able to receive text input after the designate hotkey is pressed
	if (developerConsole.isActive())
	{
		//Hotkey for turning on text input mode on the console
		if (controls.keyPressed(SDL_SCANCODE_P))
		{
			controls.turnONTextInputMode();
			developerConsole.updateTextBox("TYPE NOW");
		}

		//manages the text input for writting to the console 
		if (controls.isTextInputModeON())
		{
			developerConsole.update(&controls);
		}
	}
}

void GameEngine::updateFPS()
{
	//FPS Will update every 2 seconds
	if (currentTime > lastTimeFPS + 2000)
	{
		fpsText.setOutputText(std::string("FPS: ") + std::to_string((int)((double)nFPS / 2.0)));
		nFPS = 0;
		lastTimeFPS = currentTime;
	}
	nFPS++;
}


void GameEngine::updateTowers()
{
	if (towerShopInterface.isButtonOn("T1") || controls.keyPressed(SDL_SCANCODE_1))
	{
		addTower(0);
	}
	else if (towerShopInterface.isButtonOn("T2") || controls.keyPressed(SDL_SCANCODE_2))
	{
		addTower(1);
	}
	else if (towerShopInterface.isButtonOn("T3") || controls.keyPressed(SDL_SCANCODE_3))
	{
		addTower(2);
	}
	else if (towerShopInterface.isButtonOn("T4") || controls.keyPressed(SDL_SCANCODE_4))
	{
		addTower(3);
	}
	else if (towerShopInterface.isButtonOn("T5") || controls.keyPressed(SDL_SCANCODE_5))
	{
		addTower(4);
	}
	else if (controls.keyPressed(SDL_SCANCODE_6))
	{
		enemies.clear();
		enemies.resize(0);
		towersOnField.clear();
		towersOnField.resize(0);
		map.reloadMap();
	}

		//Shooting
	for (int i = 0; i < towersOnField.size(); i++)
	{
		if (!towersOnField[i].isSelected())
		{
			towersOnField[i].findTarget(&enemies);
//			towersOnField[i].findTarget(getEnemies(2,10));
		}
		towersOnField[i].updateTower();
	}
}

void GameEngine::updateTowerShopInterface()
{
	towerShopInterface.setWaveTimmer(timeUntilNextWave);
	towerShopInterface.setHealthText(mainPlayer.getHealth());
	towerShopInterface.setCashText(mainPlayer.getCash());

	//TOWER SELECTION FROM FIELD
	if (controls.mouseClicked())//TODO: Add in optomization so that this is only run if the mouse is clicked on the map
	{
		bool tempClickTargetFound = false;//used to keep track of if something was clicked, used for optomization 

		//checks if a tower was clicked
		if (!tempClickTargetFound)
		{								  
			for (int i = 0; i < towersOnField.size(); i++)
			{
				if (towersOnField[i].towerClicked() && !towersOnField[i].isSelected())
				{
					towerShopInterface.setIndexOfChosenTower(i);
					i = towersOnField.size();
					tempClickTargetFound = true;
				}
			}
		}

		//Checks if an enemy was clicked
		if (!tempClickTargetFound)
		{
			for (int i = 0; i < enemies.size() && enemies[i].isSpawned(); i++)//goes through all enemies to cleck if they were clicked or not. when an enemy is found to be clicked, the while loop will terminate. Loop will also terminate if the index of enemies that have not been spawned yet.
			{
				if (enemies[i].isAlive() && enemies[i].enemyClicked())//will only check for enemies that are currently alive and clicked
				{
					developerConsole.insertMessage("Enemy clicked" + std::to_string(i));
					towerShopInterface.setIndexOfChosenEnemy(i);
					i = enemies.size();
					tempClickTargetFound = true;
				}
			}
		}
	}
	
	if (towerShopInterface.isEnemyChosen())
	{
		towerShopInterface.setEnemyDisplayInfoText();
	}

	//UPGRADE THE CURRENT TOWER
	if ((towerShopInterface.upgradeTowerButtonPressed() || controls.keyPressed(SDL_SCANCODE_T)) && towerShopInterface.isTowerChosen())
	{
		if (mainPlayer.getCash() >= towerShopInterface.getTowerUpgradeCost())
		{
			mainPlayer.setCash(mainPlayer.getCash() - towerShopInterface.getTowerUpgradeCost());			
			towerShopInterface.upgradeTower();
			if (!towerShopInterface.isTowerUpgradeable())
			{
				towerShopInterface.disableUpgradeButton();
			}
		}
		else
		{
			developerConsole.insertMessage("Not enough cash for upgrade.");
		}
	}

	//SELL THE CURRENT TOWER
	if (towerShopInterface.sellTowerButtonPressed())
	{
		mainPlayer.setCash(mainPlayer.getCash() + towerShopInterface.getChosenTower().getRefundAmmount());
		map.setMapIntValue( towerShopInterface.getChosenTower().getCenterY() / map.getTileHeight(), towerShopInterface.getChosenTower().getCenterX() / map.getTileWidth(), 1);
		towerShopInterface.removeCurrentTowerTower();
		towerShopInterface.disableSellTowerButton();
		towerShopInterface.disableUpgradeButton();
	}

	towerShopInterface.update();
}

void GameEngine::updatePlayer()
{
	if (mainPlayer.getHealth() <= 0)
	{

	}
}

void GameEngine::updateWaves()//this shouldn't be its own function, it updates both enemies and towers, so this should be changed. It doesn't feel right.
//TODO: change to nextWave() function so that it can it can fit nicer when we change waves after completion.
{
	if (buttonManager.isButtonOn("NEXT WAVE"))
	{
		changeToWave(currentWave + 1);
	}

	if (playWaveAnimation)
	{
		if (currentTime > timeOfLastWaveAnnimation + durrationOfWaveAnimation)
		{
			playWaveAnimation = false;
		}
		else
		{
			int tempOpacity = 255 * ((double)(timeOfLastWaveAnnimation + durrationOfWaveAnimation - currentTime) / (double)durrationOfWaveAnimation);
			for (int i = 0; i < waveText.size(); i++)
			{
				waveText[i].setPosition(waveText[i].getX() - 3, waveText[i].getY());
				waveText[i].setOpacity(tempOpacity);
			}
		}
	}
}

void GameEngine::updateDeveloperTools()
{
	if (buttonManager.isButtonOn("BEGIN"))
	{
		gameStartTime = currentTime;
		changeToWave(0);
		startRound = true;
	}

	if (buttonManager.isButtonOn("EDIT MAP"))
	{
		bugMap.changeOpactity(175);
		if (controls.mouseClicked())//when the mouse is clicked then it changes the value of the map tile by 1 or will be reset to 0 if value is bigger than the number of tiles
		{
			int tempI = controls.getMouseY() / 40;
			int tempJ = controls.getMouseX() / 40;

			map.setMapIntValue(tempI, tempJ, (map.getMapIntValue(tempI, tempJ) + 1) % 5); //5 is the number of tiles + 1
		}
	}
	else if (buttonManager.buttonJustClickedOff("EDIT MAP"))
	{
		bugMap.changeOpactity(255);
		developerConsole.insertMessage("TEST");
	}

	if (buttonManager.isButtonOn("SAVE AS FIRST MAP"))
	{
		map.saveMapToFile("./Maps/firstMap.txt");
	}
	else if (buttonManager.isButtonOn("LOAD FIRST MAP"))
	{
		map.loadMapFromFile("./Maps/firstMap.txt");
	}
	else if (buttonManager.isButtonOn("SAVE AS SECOND MAP"))
	{
		map.saveMapToFile("./Maps/secondMap.txt");
	}
	else if (buttonManager.isButtonOn("LOAD SECOND MAP"))
	{
		map.loadMapFromFile("./Maps/secondMap.txt");
	}
	else if (buttonManager.isButtonOn("SAVE AS THIRD MAP"))
	{
		map.saveMapToFile("./Maps/thirdMap.txt");
	}
	else if (buttonManager.isButtonOn("LOAD THIRD MAP"))
	{
		map.loadMapFromFile("./Maps/thirdMap.txt");
	}


	if (buttonManager.isButtonOn("GIVE GOLD"))
	{
		mainPlayer.setCash(mainPlayer.getCash() + 10000);
	}


	if (buttonManager.isButtonOn("EXIT"))
	{
		run = false;
	}

}
