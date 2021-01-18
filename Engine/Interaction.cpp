#include "Interaction.h"



Interaction::Interaction()
{

}

Interaction::Interaction(SDL_Event * eve)
{
	e = eve;
	mouseX = 0;
	mouseY = 0;
}


Interaction::~Interaction()
{
}

void Interaction::setInputText(std::string iT)
{
	inputText = iT;
}

int Interaction::getMouseX()
{
	return mouseX;
}

int Interaction::getMouseY()
{
	return mouseY;
}

void Interaction::update()
{
	if (TextInputMode)
	{
		SDL_GetMouseState(&mouseX, &mouseY);
		updateTextEvents();
		updateMouseEvents();
	}
	else
	{
		SDL_GetMouseState(&mouseX, &mouseY);
		updateEvents();
		updateMouseEvents();
	}
}

void Interaction::updateEvents()
{
	while (SDL_PollEvent(e))
	{
		//int sC variable used to reduce function calls to retreive the scancode
		int sC = (*e).key.keysym.scancode;

		//Checks for if the window was resized
		if ((*e).window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			windowResized = true;
		}
		else if ((*e).type == SDL_MOUSEBUTTONDOWN && (*e).button.button == SDL_BUTTON_LEFT)
		{
			mouseButtonHeld = true;
		}
		else if ((*e).type == SDL_MOUSEBUTTONUP && (*e).button.button == SDL_BUTTON_LEFT)
		{
			mouseButtonReleased = true;
		}
		else if ((*e).type == SDL_KEYUP)//KEY RELEASED EVENTS
		{
			if (sC < STATE_SIZE)//Will only update events inside the array (i.e. that have a code less than STATE_SIZE)
			{
				heldState[sC] = false;
				pressedState[sC] = false;
				releasedState[sC] = true;
			}
			else
			{
				heldState[sC] = false;
				pressedState[sC] = false;
				releasedState[sC] = true;
			}
		}
		else if ((*e).type == SDL_KEYDOWN)//KEY PRESSED EVENTS
		{
			if (sC < STATE_SIZE)//Will only update events inside the array (i.e. that have a code less than STATE_SIZE)
			{
				if (heldState[sC])
				{
				}
				else if (pressedState[sC])
				{
					heldState[sC] = true;
				}
				else
				{
					pressedState[sC] = true;
					heldState[sC] = true;
				}
			}
		}
		else if ((*e).type == SDL_QUIT)
		{
		}
	}
}

void Interaction::updateTextEvents()
{
	while (SDL_PollEvent(e))
	{
		//Checks for if the window was resized
		if ((*e).window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			windowResized = true;
		}
		else if ((*e).type == SDL_KEYUP)
		{
		}
		//Handles text input for backspace and enter
		else if ((*e).type == SDL_KEYDOWN)
		{
			if (inputText.length() > 0)
			{
				//Handles the backspace of text
				if ((*e).key.keysym.sym == SDLK_BACKSPACE)
				{
					//lop off character
					inputText.pop_back();
					textChanged = true;
				}
				//Handles when they enter button is pressed
				else if ((*e).key.keysym.scancode == SDL_SCANCODE_RETURN || (*e).key.keysym.scancode == SDL_SCANCODE_RETURN2)
				{
					enterPressed = true;
				}
			}
		}
		else if ((*e).type == SDL_QUIT)
		{
			//Handle Quit events

		}
		//Handles text that has been inputted
		else if ((*e).type == SDL_TEXTINPUT)
		{
			//Append character
			inputText += (*e).text.text;
			textChanged = true;
		}
	}
}

//Resets all input variables to false
void Interaction::resetInputs()
{
	for (int i = 0; i < STATE_SIZE; i++)
	{
		heldState[i] = false;
		releasedState[i] = false;
		pressedState[i] = false;
	}
	windowResized = false;
	enterPressed = false;
}

bool Interaction::keyPressed(SDL_Scancode a)
{
	if (pressedState[a])
	{
		pressedState[a] = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Interaction::keyReleased(SDL_Scancode a)
{
	if (releasedState[a])
	{
		releasedState[a] = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Interaction::keyHeld(SDL_Scancode a)
{
	if (heldState[a])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Interaction::mouseClicked()
{
	return mouseButtonClicked;
}

bool Interaction::mouseHeld()
{
	return mouseButtonHeld;
}

bool Interaction::mouseReleased()
{
	return mouseButtonReleased;
}

void Interaction::updateMouseEvents()
{
	if (mouseButtonHeld && mouseButtonReleased)
	{
		mouseButtonClicked = true;
		mouseButtonHeld = false;
		mouseButtonReleased = false;
	}
	else if (mouseButtonClicked)
	{
		mouseButtonClicked = false;
	}

}

//Resets all of the state arrays to false and sets text input mode to true 
void Interaction::turnONTextInputMode()
{
	for (int i = 0; i < STATE_SIZE; i++)
	{
		heldState[i] = false;
		releasedState[i] = false;
		pressedState[i] = false;
	}
	TextInputMode = true;
}

void Interaction::turnOFFTextInputMode()
{
	TextInputMode = false;
}

bool Interaction::isTextInputModeON()
{
	return TextInputMode;
}

bool Interaction::isInputTextChanged()
{
	if (textChanged)
	{
		textChanged = !textChanged;
		return true;
	}
	else
	{
		return false;
	}
}

std::string Interaction::getInputText()
{
	return inputText;
}

bool Interaction::isEnterPressed()
{
	if (enterPressed)
	{
		enterPressed = !enterPressed;
		return true;
	}
	else
	{
		return false;
	}
}

//Returns true if window was resized and then sets it equal to false
//Returns false otherwise
bool Interaction::wasWindowResized()
{
	if (windowResized)
	{
		windowResized = false;
		return true;
	}
	else
	{
		return false;
	}
}
