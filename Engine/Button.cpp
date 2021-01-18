#include "Button.h"



Button::Button()
{
}

Button::Button(SDL_Renderer * ren, Interaction *control, Console * DevConsole, int screenWidth, int screenHeight)
{
	renderer = ren;
	controls = control;
	developerConsole = DevConsole;
	mouseFirstHeld = false;

	genericCounter = 0;
	genericWidth = 75;
	genericHeight = 25;
	nCols = 2;

	initialGenericX = screenWidth - genericWidth * 2 - 2 * 10;
	initialGenericY = 100;

	disableButtons = false;

	createButtonGraphics();
}

Button::~Button()
{
}

void Button::setNCols(int n)
{
	nCols = n;
	setGenericButtonPositions(initialGenericX, initialGenericY);
}

void Button::setGenericButtonPositions(int xpos, int ypos)
{
	initialGenericX = xpos;
	initialGenericY = ypos;

	int tempX = 0;
	int tempY = 0;

	for (int i = 0; i < buttonRectangles.size(); i++)
	{
		tempX = initialGenericX + (i % nCols) * (genericWidth + 10);
		tempY = initialGenericY + (int)(i / nCols) * (genericHeight + 10);

		buttonRectangles[i].x = tempX;
		buttonRectangles[i].y = tempY;
		buttonHitBoxes[i].setX(tempX);
		buttonHitBoxes[i].setY(tempY);

		buttonText[buttonText.size() - 1].setPosition(tempX, tempY);
	}
}

int Button::getGenericButtonWidth()
{
	return genericWidth;
}

int Button::getGenericButtonHeight()
{
	return genericHeight;
}

void Button::createButtonGraphics()
{
	onSprite = Sprite("./images/Buttons/ButtonClicked.png", renderer, 362, 178, 207, 0, 0);

	offSprite = Sprite("./images/Buttons/ButtonNormal.png", renderer, 362, 178, 0, 0);

	hoverSprite = Sprite("./images/Buttons/ButtonHovered.png", renderer, 362, 178, 0, 0);

}

bool Button::buttonNameExists(std::string buttonName)
{
	for (int i = 0; i < buttonNames[i].size(); i++)
	{
		if (buttonName.compare(buttonNames[i]))
		{
			return true;
		}
	}

	return false;
}

void Button::createButton(std::string buttonName, bool toggleable, int x, int y, int w, int h)
{
	if (!buttonNameExists(buttonName))
	{
		buttonNames.push_back(buttonName);
	}
	else
	{
		buttonNames.push_back(buttonName + "COPY");
		(*developerConsole).insertMessage("Duplicate buttons nammed: " + buttonName);
	}

	linkID = 0;
	buttonNames.push_back(buttonName);
	buttonRectangles.push_back(SDL_Rect({ x,y,w,h }));
	buttonHitBoxes.push_back(HitBox(renderer, { x,y,w,h }));
	buttonToggleable.push_back(toggleable);

	buttonText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", 12, { 255,255,0 }, buttonName));
	buttonText[buttonText.size() - 1].setPosition(x, y);

	buttonClicked.push_back(false);
	buttonHovered.push_back(false);
	buttonHeld.push_back(false);
	buttonON.push_back(false);
	buttonAnimated.push_back(false);
}

void Button::createButton(std::string buttonName, bool toggleable)
{
	int x = initialGenericX + (genericCounter % nCols) * (genericWidth + 10);
	int y = initialGenericY + (int)(genericCounter / nCols) * (genericHeight + 10);

	linkID = 0;
	buttonNames.push_back(buttonName);
	buttonRectangles.push_back(SDL_Rect({ x,y,genericWidth,genericHeight }));
	buttonHitBoxes.push_back(HitBox(renderer, { x,y,genericWidth,genericHeight }));
	buttonToggleable.push_back(toggleable);

	buttonText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", 12, { 100,25,255 }, buttonName));
	buttonText[buttonText.size() - 1].setPosition(x + (buttonRectangles[buttonRectangles.size() - 1].w - buttonText[buttonText.size() - 1].getWidth()) / 2, y + (buttonRectangles[buttonRectangles.size() - 1].h - buttonText[buttonText.size() - 1].getHeight()) / 2);

	buttonClicked.push_back(false);
	buttonHovered.push_back(false);
	buttonHeld.push_back(false);
	buttonON.push_back(false);
	buttonAnimated.push_back(false);
//	(*developerConsole).insertMessage(std::to_string(x) + "," + std::to_string(y));

	genericCounter++;
}

void Button::createAnimatedButton(std::string buttonName, bool toggleable, AnimatedSprite aSprite)
{
	int x = initialGenericX + (genericCounter % nCols) * (genericWidth + 10);
	int y = initialGenericY + (int)(genericCounter / nCols) * (genericHeight + 10);

	linkID = 0;
	buttonNames.push_back(buttonName);
	buttonRectangles.push_back(SDL_Rect({ x,y,genericWidth,genericHeight }));
	buttonHitBoxes.push_back(HitBox(renderer, { x,y,genericWidth,genericHeight }));
	buttonToggleable.push_back(toggleable);
	buttonText.push_back(Text(renderer, "./fonts/Quicksand_Regular.otf", 12, { 255,255,0 }, buttonName));
	buttonClicked.push_back(false);
	buttonHovered.push_back(false);
	buttonHeld.push_back(false);
	buttonON.push_back(false);
	buttonAnimated.push_back(true);

	genericCounter++;
}

void Button::sendButtonClickedMessageToConsole(int buttonNumber)
{
	if (buttonNumber < buttonNames.size())
	{
		(*developerConsole).insertMessage(buttonNames[buttonNumber] + " has been clicked.");
	}
	else
	{
		(*developerConsole).insertMessage("SendButtonClickedMessageToConsole(int buttonNumber) function input out of bounds");
	}
}

bool Button::checkForHoveredButton()
{
	bool returnValue = false;
	int x = (*controls).getMouseX();
	int y = (*controls).getMouseY();
	for (int i = 0; i < buttonNames.size(); i++)
	{
		HitBox tempHB = buttonHitBoxes[i];
		if (x > tempHB.getX()
			&& x < tempHB.getX() + tempHB.getW()
			&& y > tempHB.getY()
			&& y < tempHB.getY() + tempHB.getH())
		{
			buttonHovered[i] = true;
			returnValue = true;
		}
		else
		{
			buttonHovered[i] = false;
		}
	}
	return returnValue;
}

void Button::disableAllButtons()
{
	std::string temp = "";
	disableButtons = true;
	turnOffAllButtons();

	(*developerConsole).insertMessage("Buttons disabled: ");
	for (int i = 0; i < buttonNames.size(); i++)
	{
		temp += buttonNames[i];
		temp += " ";
	}
	(*developerConsole).insertMessage(temp);
}

void Button::enableButtons()
{
	disableButtons = false;
}

bool Button::isButtonsDisabled()
{
	return disableButtons;
}

/**When a button is clicked, then the button will be turned on and the click event will be purged
Only 1 button can be clicked at a time*/
void Button::updateClickedButtons()
{
	for (int i = 0; i < buttonHovered.size(); i++)
	{
		if (buttonHovered[i] && (*controls).mouseClicked())
		{
			buttonClicked[i] = true;
			(*controls).updateMouseEvents();
		}
		else
		{
			buttonClicked[i] = false;
		}
	}
}

void Button::updateHeldButtons()
{
	if ((*controls).mouseHeld() && !mouseFirstHeld)
	{
		for (int i = 0; i < buttonHeld.size(); i++)
		{
			if (buttonHovered[i])
			{
				buttonHeld[i] = true;
			}
			else
			{
				buttonHeld[i] = false;
			}
		}
		mouseFirstHeld = true;
	}
	else if (mouseFirstHeld && (*controls).mouseClicked())
	{
		for (int i = 0; i < buttonHeld.size(); i++)
		{
			buttonHeld[i] = false;
		}
		mouseFirstHeld = false;
	}
}

void Button::updateONStatesOfButtons()
{
	for (int i = 0; i < buttonON.size(); i++)
	{
		if (buttonToggleable[i])
		{
			if (buttonClicked[i])
			{
				buttonON[i] = !buttonON[i];
				(*developerConsole).insertMessage("Button: " + buttonNames[i] + " toggled.");
			}
		}
		else
		{
			if (buttonClicked[i] && !buttonON[i])
			{
				buttonON[i] = true;
				(*developerConsole).insertMessage("Button: " + buttonNames[i] + " clicked.");
			}
			else
			{
				buttonON[i] = false;
			}
		}
	}
}

void Button::update()
{
	if (!disableButtons)
	{
		checkForHoveredButton();
		updateHeldButtons();
		updateClickedButtons();
		updateONStatesOfButtons();
	}
}

/** Returns the index of that button if the button with name is found, else if button is not found then returns -1*/
int Button::findButton(std::string buttonName)
{
	for (int i = 0; i < buttonNames.size(); i++)
	{
		if (buttonNames[i].compare(buttonName) == 0)
		{
			return i;
		}
	}
	return -1;
}

bool Button::isButtonOn(std::string buttonName)
{
	int n = findButton(buttonName);

	if (n >= 0)
	{
		if (buttonON[n])
		{
//			(*developerConsole).insertMessage("Button " + buttonName + " is now ON");
		}
		return buttonON[n];
	}
	else
	{
		(*developerConsole).insertMessage("Button " + buttonName + " not Found");
		return false;
	}
}

bool Button::isButtonOn(int n)
{
	if (n > buttonON.size() || n < 0)
	{
		(*developerConsole).insertMessage("Button " + std::to_string(n) + " not Found. Out of Bounds.");
	}
	else
	{
		return buttonON[n];
	}
}

bool Button::buttonJustClickedOn(std::string buttonName)
{
	int temp = findButton(buttonName);

	if (buttonClicked[temp])
	{
		return buttonON[temp];
	}

	return false;
}

bool Button::buttonJustClickedOff(std::string buttonName)
{
	int temp = findButton(buttonName);

	if (buttonClicked[temp])
	{
		return !buttonON[temp];
	}

	return false;
}

void Button::turnOffButton(int n)
{
	buttonON[n] = false;
}

void Button::turnOffAllButtons()
{
	for (int i = 0; i < buttonON.size(); i++)
	{
		turnOffButton(i);
	}
}

void Button::linkButtons(std::string names[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			findButton(names[0]);
		}
	}
}

void Button::render(int i)
{
	if (!disableButtons)
	{
		if (buttonON[i])
		{
			//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			//SDL_RenderFillRect(renderer, &buttonRectangles[i]);
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			onSprite.setW(buttonHitBoxes[i].getW());
			onSprite.setH(buttonHitBoxes[i].getH());
			onSprite.renderAt(buttonHitBoxes[i].getX(), buttonHitBoxes[i].getY());
		}
		else if (buttonHeld[i])
		{
			//SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			//SDL_RenderFillRect(renderer, &buttonRectangles[i]);
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			hoverSprite.setW(buttonHitBoxes[i].getW());
			hoverSprite.setH(buttonHitBoxes[i].getH());
			hoverSprite.renderAt(buttonHitBoxes[i].getX(), buttonHitBoxes[i].getY());
		}
		else if (buttonHovered[i])
		{
			//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			//SDL_RenderFillRect(renderer, &buttonRectangles[i]);
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			hoverSprite.setW(buttonHitBoxes[i].getW());
			hoverSprite.setH(buttonHitBoxes[i].getH());
			hoverSprite.renderAt(buttonHitBoxes[i].getX(), buttonHitBoxes[i].getY());
		}
		else
		{
			//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			//SDL_RenderDrawRect(renderer, &buttonRectangles[i]);
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			offSprite.setW(buttonHitBoxes[i].getW());
			offSprite.setH(buttonHitBoxes[i].getH());
			offSprite.renderAt(buttonHitBoxes[i].getX(), buttonHitBoxes[i].getY());
		}
		buttonText[i].render();
	}
}

void Button::render()
{
	for (int i = 0; i < buttonNames.size(); i++)
	{
		render(i);
	}
}
