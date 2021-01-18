#include "Interface.h"

Interface::Interface()
{
	x = 0;
	y = 0;
	nameOfInterface = "Default Interface";

}

Interface::Interface(SDL_Renderer* ren, Interaction *controls, Console * dConsole, AnimatedSprite backgroundOfInterfaceSprite, std::string interfaceName, int xpos, int ypos)
{
	renderer = ren;
	devConsole = dConsole;
	interfaceSprite = backgroundOfInterfaceSprite;
	nameOfInterface = interfaceName;
	x = xpos;
	y = ypos;

	buttonManager = Button(ren, controls, dConsole, 0, 0);
	buttonManager.setGenericButtonPositions(xpos + 20, ypos + 200);

	upgradeButton = Button(ren, controls, dConsole, 0, 0);
	int tempUpgradeButtonX = x + (backgroundOfInterfaceSprite.getW() - upgradeButton.getGenericButtonWidth()) / 2;
	int tempUpgradeButtonY = 445;
	upgradeButton.setGenericButtonPositions(tempUpgradeButtonX, tempUpgradeButtonY);
	upgradeButton.createButton("Upgrade", false);
	upgradeButton.disableAllButtons();
	upgradeTowerCoinSprite = AnimatedSprite("./images/TowerDefenceSprites/Interface Sprites/coinSprite.png", renderer, &x, 0, 0, 1, 1, 1, 119, 129, 0, 0);
	upgradeTowerCoinSprite.setW(13);
	upgradeTowerCoinSprite.setH(13);
	upgradeTowerCoinSprite.setPosition(tempUpgradeButtonX + upgradeButton.getGenericButtonWidth() + 3, tempUpgradeButtonY + (upgradeButton.getGenericButtonHeight() - upgradeTowerCoinSprite.getH()) / 2);
	upgradeTowerCostText = Text(ren, "./fonts/CODE Bold.otf", 16, { 125, 95, 8 }, "*-*");
	upgradeTowerCostText.setPosition(upgradeTowerCoinSprite.getX() + upgradeTowerCoinSprite.getW() + 2, upgradeTowerCoinSprite.getY() + (upgradeTowerCoinSprite.getH() - upgradeTowerCostText.getHeight()) / 2);

	sellButton = Button(ren, controls, dConsole, 0, 0);
	int tempSellButtonX = x + (backgroundOfInterfaceSprite.getW() - upgradeButton.getGenericButtonWidth()) / 2;
	int tempSellButtonY = 445 - sellButton.getGenericButtonHeight() - 2;
	sellButton.setGenericButtonPositions(tempSellButtonX, tempSellButtonY);
	sellButton.createButton("Sell", false);
	sellButton.disableAllButtons();
	sellTowerCoinSprite = AnimatedSprite("./images/TowerDefenceSprites/Interface Sprites/coinSprite.png", renderer, &x, 0, 0, 1, 1, 1, 119, 129, 0, 0);
	sellTowerCoinSprite.setW(13);
	sellTowerCoinSprite.setH(13);
	sellTowerCoinSprite.setPosition(tempSellButtonX + sellButton.getGenericButtonWidth() + 3, tempSellButtonY + (sellButton.getGenericButtonHeight() - sellTowerCoinSprite.getH()) / 2);
	sellTowerValueText = Text(ren, "./fonts/CODE Bold.otf", 16, { 125, 95, 8 }, "*-*");
	sellTowerValueText.setPosition(sellTowerCoinSprite.getX() + sellTowerCoinSprite.getW() + 2, sellTowerCoinSprite.getY() + (sellTowerCoinSprite.getH() - sellTowerValueText.getHeight()) / 2);

	towerChosen = false;
	towersOnFieldPtr = nullptr;
	indexOfChosenTower = -1;

	enemyChosen = false;
	enemiesOnFieldPtr = nullptr;
	indexOfChosenEnemy = -1;
	enemyNameInfoText = Text(ren, "./fonts/Quicksand_Regular.otf", 14, { 255,255,255 }, "ENEMY NAME");
	enemyNameInfoText.setPosition(interfaceSprite.getCenterX() - enemyNameInfoText.getWidth() / 2 , 315);
	healthInfoText = Text(ren, "./fonts/Quicksand_Regular.otf", 14, { 255,255,255 }, "---/---");
	healthInfoText.setPosition(interfaceSprite.getCenterX() - healthInfoText.getWidth() / 2, 315 + healthInfoText.getHeight());
	deathXSprite = AnimatedSprite("./images/Red_Cross.png", renderer, &x, 0, 0, 1, 1, 1, 423, 308, 1035, 560);
	deathXSprite.setW(63);
	deathXSprite.setH(46);

	timeToNextWaveText = Text(ren, "./fonts/CODE Bold.otf", 15, { 0,0,0 }, "Next Round Starts In: --:--");
	timeToNextWaveText.setPosition(1010, 100);

	cashTextBackgroundSprite = AnimatedSprite("./images/TowerDefenceSprites/Interface Sprites/CashTextBoxSprite.png", renderer, &x, 0, 0, 1, 1, 1, 280, 102, 1035, 560);
	cashTextBackgroundSprite.setW(280 * 2 / 4);
	cashTextBackgroundSprite.setH(102 * 2 / 4);
	cashTextBackgroundSprite.setPosition(interfaceSprite.getX() + (interfaceSprite.getW() - cashTextBackgroundSprite.getW()) / 2, 470);
	cash = -1;
	cashText = Text(ren, "./fonts/CODE Bold.otf", 28, { 249, 191, 17 }, "---");
	cashText.setPosition(cashTextBackgroundSprite.getX() + 50, cashTextBackgroundSprite.getY() + (cashTextBackgroundSprite.getH() - cashText.getHeight()) / 2 + 3);

	livesTextBackgroundSprite = AnimatedSprite("./images/TowerDefenceSprites/Interface Sprites/LivesTextBoxSprite.png", renderer, &x, 0, 0, 1, 1, 1, 505, 120, 1028, 120);
	livesTextBackgroundSprite.setW(505 * 1 / 3);
	livesTextBackgroundSprite.setH(120 * 1 / 3);
	health = -1;
	healthText = Text(ren, "./fonts/CODE Bold.otf", 25, { 255, 255, 255 }, "---");
	healthText.setPosition(livesTextBackgroundSprite.getX() + 75, livesTextBackgroundSprite.getY() + (livesTextBackgroundSprite.getH() - healthText.getHeight()) / 2 + 1);

	for (int i = 0; i < 10; i++)
	{
		initialYMovingText.push_back(0);
		showMovingText.push_back(false);
		movingText.push_back(Text(ren, "./fonts/Quicksand_Regular.otf", 22, { 255, 0, 0 }, "-NONE"));
	}

	int tempInitialTowerInfoX = interfaceSprite.getX() + 25;
	int tempInitialTowerInfoY = cashTextBackgroundSprite.getY() - 4 * Text(ren, "./fonts/CODE Bold.otf", 14, { 100,25,255 }, "TOWER NAME").getHeight() - 100;
	int tempInitialTowerInfoW = 175;
	int tempInitialTowerInfoH = 4 * (Text(ren, "./fonts/CODE Bold.otf", 14, { 100,25,255 }, "TOWER NAME").getHeight());
	towerInfoBackgroundRect0 = SDL_Rect({ tempInitialTowerInfoX, tempInitialTowerInfoY, tempInitialTowerInfoW, tempInitialTowerInfoH });
	towerInfoBackgroundRect1 = SDL_Rect({ tempInitialTowerInfoX, tempInitialTowerInfoY, tempInitialTowerInfoW, tempInitialTowerInfoH });

	SDL_Color tempInfoTextColor = {255,255,255};
	int tempInitialTowerInfoTextStartX = tempInitialTowerInfoX + 3;
	towerNameInfoText = Text(ren, "./fonts/CODE Bold.otf", 14, tempInfoTextColor, "TOWER NAME");
	towerNameInfoText.setPosition(tempInitialTowerInfoTextStartX, cashTextBackgroundSprite.getY() - 4 * (towerNameInfoText.getHeight()) - 100);
	damageInfoText = Text(ren, "./fonts/CODE Bold.otf", 14, tempInfoTextColor, "DAMAGE");
	damageInfoText.setPosition(tempInitialTowerInfoTextStartX, cashTextBackgroundSprite.getY() - 3 * (towerNameInfoText.getHeight()) - 100);
	numberOfTargetsInfoText = Text(ren, "./fonts/CODE Bold.otf", 14, tempInfoTextColor, "NUMBER OF TARGETS");
	numberOfTargetsInfoText.setPosition(tempInitialTowerInfoTextStartX, cashTextBackgroundSprite.getY() - 2 * (towerNameInfoText.getHeight()) - 100);
	towerRangeInfoText = Text(ren, "./fonts/CODE Bold.otf", 14, tempInfoTextColor, "TOWER RANGE");
	towerRangeInfoText.setPosition(tempInitialTowerInfoTextStartX, cashTextBackgroundSprite.getY() - 1 * (towerNameInfoText.getHeight()) - 100);
}


Interface::~Interface()
{
}

void Interface::setX(int xpos)
{
	x = xpos;
	interfaceSprite.setX(x);
//	buttonManager.setX(xpos);
}

void Interface::setY(int ypos)
{
	y = ypos;
	interfaceSprite.setY(y);
}

void Interface::setGenericButtonsPositions(int newX, int newY)
{
	buttonManager.setGenericButtonPositions(newX, newY);
}

void Interface::setPosistion(int xpos, int ypos)
{
	setX(xpos);
	setY(ypos);
}

void Interface::setCashText(int c)
{
	if (cash != c)
	{
		if (cash > c)
		{
			addMovingText("-" + std::to_string(cash - c), { 255, 0, 0 }, cashText.getX() + cashText.getWidth() / 2, cashText.getY() + cashText.getHeight() / 2);
		}
		else
		{
			addMovingText("+" + std::to_string(c - cash), { 0, 255, 0 }, cashText.getX() + cashText.getWidth() / 8, cashText.getY() + cashText.getHeight() / 2);
		}

		cash = c;
		cashText.setOutputText(std::to_string(cash));
	}
}

void Interface::setHealthText(int h)
{
	if (health != h)
	{
		if (health > h)
		{
			addMovingText("-" + std::to_string(health - h), { 255, 0, 0 }, healthText.getX() + healthText.getWidth(), healthText.getY() + healthText.getHeight() / 2);
		}
		else
		{
			addMovingText("+" + std::to_string(h - health), { 0, 255, 0 }, healthText.getX() + healthText.getWidth(), healthText.getY() + healthText.getHeight() / 2);
		}
		health = h;
		int r = 255 * ((100.0 - (double)health) / 100.0);
		int g = 255 - 255 * ((100.0 - (double)health) / 100.0);

		healthText.setOutputText(std::to_string(health));
		SDL_Color tempColor = {r,g,0};
		healthText.setTextColor(tempColor);
	}
}

void Interface::setWaveTimmer(int t)
{
	timeToNextWaveText.setOutputText("Next Round Starts In: " + std::to_string(t / 1000));
}

void Interface::setTowersOnFieldPtr(std::vector<Tower>* towersOnField)
{
	towersOnFieldPtr = towersOnField;
}

void Interface::setIndexOfChosenTower(int n)
{
	//If a tower is clicked on a second time, then this turns off the range display and stops displaying the tower info on the interface
	if (towerChosen && indexOfChosenTower == n)
	{
		(*towersOnFieldPtr)[indexOfChosenTower].toggleInfoDisplay();//turns off range display around tower
		towerChosen = false; //tower info will no longer show on the interface
		disableSellTowerButton();
		disableUpgradeButton();
	}
	else if (n < towersOnFieldPtr->size())//sets a new tower to be shown on the interface
	{
		if (enemyChosen)//Stops displaying the cicked on enemy if one was clicked prior
		{
			enemyChosen = false;
		}
		else if (towerChosen)//If a tower is currently chosen, then all that info and the tower range info will be cleared and buttons disabled
		{
			(*towersOnFieldPtr)[indexOfChosenTower].toggleInfoDisplay();//turns off range display around tower
			disableSellTowerButton();
			disableUpgradeButton();
		}

		indexOfChosenTower = n; 		//Setting the new index
		(*towersOnFieldPtr)[indexOfChosenTower].toggleInfoDisplay();//Displaying the tower range

		//ENABLES THE DISPLAY OF THE TOWER AND THE UPGRADE AND SELL BUTTONS
		setTowerDisplayInfoText((*towersOnFieldPtr)[indexOfChosenTower]);
		enableSellTowerButton();
		updateUpgradeAndSellText();
		if (isTowerUpgradeable())
		{
			enableUpgradeButton();
		}
		else
		{
			disableUpgradeButton();
		}

		towerChosen = true;
	}
	else if (n < 0)
	{
		devConsole->insertMessage("setIndexOfChosenTower input is too small");
	}
	else
	{
		devConsole->insertMessage("setIndexOfChosenTower input is too large");
	}
}

void Interface::setTowerDisplayInfoText(Tower towerClickedOn)
{
	towerNameInfoText.setOutputText(towerClickedOn.getName() + " " + std::to_string(towerClickedOn.getNCurrentTowerUpgrade()));
	damageInfoText.setOutputText("Damage: " + std::to_string(towerClickedOn.getDamage()));
	numberOfTargetsInfoText.setOutputText("Max Targets: " + std::to_string(towerClickedOn.getNumberOfTargets()));
	towerRangeInfoText.setOutputText("Range: " + std::to_string(towerClickedOn.getTowerRange()));
}

void Interface::setEnemiesOnFieldPtr(std::vector<Enemy>* enemiesOnField)
{
	enemiesOnFieldPtr = enemiesOnField;
}

void Interface::setIndexOfChosenEnemy(int n)
{
	if (n < enemiesOnFieldPtr->size())
	{
		indexOfChosenEnemy = n;
		setEnemyDisplayInfoText();
		enemyChosen = true;

		if (towerChosen)
		{
			(*towersOnFieldPtr)[indexOfChosenTower].toggleInfoDisplay();//turns off range display around tower
			disableSellTowerButton();
			disableUpgradeButton();
			towerChosen = false; //tower info will no longer show on the interface
		}
	}
}

void Interface::setEnemyDisplayInfoText()
{
	if (indexOfChosenEnemy < enemiesOnFieldPtr->size())
	{
		if ((*enemiesOnFieldPtr)[indexOfChosenEnemy].getName().compare(enemyNameInfoText.getOutputText()) != 0)
		{
			enemyNameInfoText.setOutputText((*enemiesOnFieldPtr)[indexOfChosenEnemy].getName());
		}

		std::string tempHealthString = std::to_string((*enemiesOnFieldPtr)[indexOfChosenEnemy].getCurrentHealth()) + "/" + std::to_string((*enemiesOnFieldPtr)[indexOfChosenEnemy].getBaseHealth());
		if (healthInfoText.getOutputText().compare(tempHealthString) != 0)
		{
			healthInfoText.setOutputText(tempHealthString);
		}
	}
	else
	{
		enemyNameInfoText.setOutputText("NO ENEMY CHOSEN");
		healthInfoText.setOutputText("??? / ???");
	}
}

int Interface::getTowerUpgradeCost()
{
	return (*towersOnFieldPtr)[indexOfChosenTower].getUpgradeCost();
}

Tower Interface::getChosenTower()
{
	return (*towersOnFieldPtr)[indexOfChosenTower];
}

void Interface::updateMovingText()
{
	for (int i = 0; i < movingText.size(); i++)
	{
		if (showMovingText[i])
		{
			if (movingText[i].getY() < initialYMovingText[i] - 30)
			{
				showMovingText[i] = false;
			}
			else
			{
				movingText[i].setPosition(movingText[i].getX(), movingText[i].getY() - 1);
			}
		}
	}
}

void Interface::updateText()
{
}

void Interface::updateChosenFieldTowerUpgradesAndTargetingOptions()
{

}

void Interface::updateDisplayTowerText()
{
	setTowerDisplayInfoText((*towersOnFieldPtr)[indexOfChosenTower]);
}

void Interface::updateUpgradeAndSellText()
{
	upgradeTowerCostText.setOutputText(std::to_string((*towersOnFieldPtr)[indexOfChosenTower].getUpgradeCost()));
	sellTowerValueText.setOutputText(std::to_string((*towersOnFieldPtr)[indexOfChosenTower].getRefundAmmount()));
}

void Interface::update()
{
	buttonManager.update();
	upgradeButton.update();
	sellButton.update();
	updateMovingText();
}

void Interface::render()
{
	interfaceSprite.render();
	buttonManager.render();

	//SDL_Rect tempRect = { 1020, 450, 200, 100 };
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(renderer, &tempRect);
	//tempRect = { 1022, 452, 196, 96 };
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderFillRect(renderer, &tempRect);

	timeToNextWaveText.render();


	cashTextBackgroundSprite.render();
	cashText.render();

	livesTextBackgroundSprite.render();
	healthText.render();

	for (int i = 0; i < movingText.size(); i++)
	{
		if (showMovingText[i])
		{
			movingText[i].render();
		}
	}

	if (towerChosen)
	{
		//TOWER COPY
		(*towersOnFieldPtr)[indexOfChosenTower].renderInterfaceCopyAt(interfaceSprite.getX() + (interfaceSprite.getW())/ 2 - (*towersOnFieldPtr)[indexOfChosenTower].getCenterX(), 395 - (*towersOnFieldPtr)[indexOfChosenTower].getCenterY());

		//Background for the text to be displayed on
			//main background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
		SDL_RenderFillRect(renderer, &towerInfoBackgroundRect0);
			//border of background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &towerInfoBackgroundRect1);
		//reset render draw color to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		//TOWER INFO TEXT
		towerNameInfoText.render();
		damageInfoText.render();
		numberOfTargetsInfoText.render();
		towerRangeInfoText.render();

		if (!upgradeButton.isButtonsDisabled())
		{
			upgradeButton.render();
			upgradeTowerCoinSprite.render();
			upgradeTowerCostText.render();
		}
		if (!sellButton.isButtonsDisabled())
		{
			sellButton.render();
			sellTowerCoinSprite.render();
			sellTowerValueText.render();
		}

	}
	else if (enemyChosen)
	{
		//Background for the text to be displayed on
		//main background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
		SDL_RenderFillRect(renderer, &towerInfoBackgroundRect0);
		//border of background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &towerInfoBackgroundRect0);
		//reset render draw color to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		enemyNameInfoText.render();
		healthInfoText.render();

		if (indexOfChosenEnemy < enemiesOnFieldPtr->size())
		{

			if ((*enemiesOnFieldPtr)[indexOfChosenEnemy].isAlive())
			{
				(*enemiesOnFieldPtr)[indexOfChosenEnemy].renderInterfaceCopyAt(interfaceSprite.getX() + (interfaceSprite.getW()) / 2 - (*enemiesOnFieldPtr)[indexOfChosenEnemy].getCenterX(), 395 - (*enemiesOnFieldPtr)[indexOfChosenEnemy].getCenterY());
			}
			else
			{
				//RED INFO DISPLAY BOX
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
				SDL_RenderFillRect(renderer, &towerInfoBackgroundRect0);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

				//RECOLOURED ENEMY
				(*enemiesOnFieldPtr)[indexOfChosenEnemy].setColor({80,80,80});
				(*enemiesOnFieldPtr)[indexOfChosenEnemy].renderInterfaceCopyAt(interfaceSprite.getX() + (interfaceSprite.getW()) / 2 - (*enemiesOnFieldPtr)[indexOfChosenEnemy].getCenterX(), 395 - (*enemiesOnFieldPtr)[indexOfChosenEnemy].getCenterY());
				(*enemiesOnFieldPtr)[indexOfChosenEnemy].setColor({ 255,255,255 });

				//X OVER THE ENEMY
				deathXSprite.renderAt(interfaceSprite.getX() + (interfaceSprite.getW() - deathXSprite.getW() / 1.5) / 2, 395 - deathXSprite.getH() / 2);
			}

		}
	}

}

void Interface::addMovingText(std::string message, SDL_Color color, int xpos, int ypos)
{
	for (int i = 0; i < showMovingText.size(); i++)
	{
		if (!showMovingText[i])
		{
			initialYMovingText[i] = ypos;
			showMovingText[i] = true;
			movingText[i].setOutputText(message);
			movingText[i].setPosition(xpos, ypos);
			movingText[i].setTextColor(color);
			i = showMovingText.size();
		}
	}
}

void Interface::addButtons(std::vector<std::string> buttonNames, std::vector<bool> buttonToggleable)
{
	if(buttonNames.size() != buttonToggleable.size())
	{ 
		(*devConsole).insertMessage("Could not generate buttons for " + nameOfInterface + ".");
	}
	else
	{
		for (int i = 0; i < buttonNames.size(); i++)
		{
			buttonManager.createButton(buttonNames[i], buttonToggleable[i]);
//			(*devConsole).insertMessage("Button Generated: " + buttonNames[i] + ".");
		}
	}
}

void Interface::addButton(std::string buttonName, bool buttonToggleable, int xpos, int ypos, int width, int height)
{
	buttonManager.createButton(buttonName, buttonToggleable, xpos, ypos, width, height);
}

bool Interface::isButtonOn(std::string buttonName)
{
	return buttonManager.isButtonOn(buttonName);
}

bool Interface::upgradeTowerButtonPressed()
{
	return upgradeButton.isButtonOn("Upgrade");
}

bool Interface::sellTowerButtonPressed()
{
	return sellButton.isButtonOn("Sell");
}

void Interface::removeCurrentTowerTower()
{
	(*towersOnFieldPtr).erase(towersOnFieldPtr->begin() + indexOfChosenTower);
	towerChosen = false;
}

bool Interface::isTowerUpgradeable()
{
	return (*towersOnFieldPtr)[indexOfChosenTower].isUpgradeable();
}

void Interface::upgradeTower()
{
	if (indexOfChosenTower < towersOnFieldPtr->size())
	{
		(*towersOnFieldPtr)[indexOfChosenTower].upgrade();
		updateDisplayTowerText();
		updateUpgradeAndSellText();
		devConsole->insertMessage("Tower upgraded");
	}
	else
	{
		devConsole->insertMessage("Invalid indexOfChosenTower in Interface::upgradeTower()");
	}

}

void Interface::disableUpgradeButton()
{
	upgradeButton.disableAllButtons();
}

void Interface::enableUpgradeButton()
{
	upgradeButton.enableButtons();
}

void Interface::disableSellTowerButton()
{
	sellButton.disableAllButtons();
}

void Interface::enableSellTowerButton()
{
	sellButton.enableButtons();
}

bool Interface::isTowerChosen()
{
	return towerChosen;
}

bool Interface::isEnemyChosen()
{
	return enemyChosen;
}
