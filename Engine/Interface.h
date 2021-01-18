#pragma once
#include"Button.h"
#include"Tower.h"

class Interface
{
public:
	Interface();
	Interface(SDL_Renderer* ren, Interaction *controls, Console * dConsole, AnimatedSprite backgroundOfInterfaceSprite, std::string interfaceName, int xpos, int ypos);
	~Interface();

	//SETTERS
	void setX(int xpos);
	void setY(int ypos);
	void setGenericButtonsPositions(int newX, int newY);
	void setPosistion(int xpos, int ypos);
	void setCashText(int c);
	void setHealthText(int h);
	void setWaveTimmer(int t);
	void setTowersOnFieldPtr(std::vector<Tower> * towersOnField);
	void setIndexOfChosenTower(int n);
	void setTowerDisplayInfoText(Tower towerClickedOn);
	void setEnemiesOnFieldPtr(std::vector<Enemy> *enemiesOnField);
	void setIndexOfChosenEnemy(int n);
	void setEnemyDisplayInfoText();

	//GETTERS
	int getTowerUpgradeCost();
	Tower getChosenTower();

	//UPDATE
	void updateMovingText();
	void updateText();
	void updateChosenFieldTowerUpgradesAndTargetingOptions();
	void updateDisplayTowerText();
	void updateUpgradeAndSellText();
	void update();

	//RENDER
	void render();


	//MISC
	void addMovingText(std::string message, SDL_Color color, int xpos, int ypos);
	void addButtons(std::vector<std::string> buttonNames, std::vector<bool> buttonToggleable);
	void addButton(std::string buttonNames, bool buttonToggleable, int xpos, int ypos, int width, int height);
	bool isButtonOn(std::string buttonName);
	bool upgradeTowerButtonPressed();
	bool sellTowerButtonPressed();
	void removeCurrentTowerTower();
	bool isTowerUpgradeable();
	void upgradeTower();
	void disableUpgradeButton();
	void enableUpgradeButton();
	void disableSellTowerButton();
	void enableSellTowerButton();
	bool isTowerChosen();
	bool isEnemyChosen();


private:
	SDL_Renderer *renderer;
	Console *devConsole;
	AnimatedSprite interfaceSprite;
	AnimatedSprite cashTextBackgroundSprite;
	AnimatedSprite livesTextBackgroundSprite;
	std::string nameOfInterface;
	Button buttonManager;

	Button upgradeButton;
	AnimatedSprite upgradeTowerCoinSprite;
	Text upgradeTowerCostText;

	Button sellButton;
	AnimatedSprite sellTowerCoinSprite;
	Text sellTowerValueText;

	Text nameText;
	Text healthText;
	Text cashText;

	SDL_Rect towerInfoBackgroundRect0;
	SDL_Rect towerInfoBackgroundRect1;
	Text towerNameInfoText;
	Text damageInfoText;
	Text numberOfTargetsInfoText;
	Text towerRangeInfoText;
	std::vector<Tower> *towersOnFieldPtr;
	int indexOfChosenTower;
	bool towerChosen;

	bool enemyChosen;
	std::vector<Enemy> *enemiesOnFieldPtr;
	int indexOfChosenEnemy;
	Text enemyNameInfoText;
	Text healthInfoText;
	AnimatedSprite deathXSprite;

	Text timeToNextWaveText;

	std::vector<int> initialYMovingText;
	std::vector<bool> showMovingText;
	std::vector<Text> movingText;

	int cash, health;
	std::vector<Text> infoText;

	int x, y;

};

