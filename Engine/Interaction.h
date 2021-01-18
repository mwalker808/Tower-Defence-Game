#pragma once
#include<SDL.h>
#include<iostream>

/** 
TODO
-Pause/block input
-
*/

/** FEATURES

*/
class Interaction
{
public:
	Interaction();
	Interaction(SDL_Event * eve);
	~Interaction();

	//Setters
	void setInputText(std::string iT);

	//Getters
	int getMouseX();
	int getMouseY();

	//Update Functions
	void update();
	void updateEvents();
	void updateTextEvents();
	void updateMouseEvents();

	//Reset all Input
	void resetInputs();


	//Keystates
	bool keyPressed(SDL_Scancode a);
	bool keyReleased(SDL_Scancode a);
	bool keyHeld(SDL_Scancode a);

	//Mouse state
	bool mouseClicked();
	bool mouseHeld();
	bool mouseReleased();

	//Inputing text
	void turnONTextInputMode();
	void turnOFFTextInputMode();
	bool isTextInputModeON();
	bool isInputTextChanged();
	std::string getInputText();
	bool isEnterPressed();

	//Window Events
	bool wasWindowResized();

private:
	SDL_Event * e;
	static const int STATE_SIZE = 120;
	bool heldState[STATE_SIZE] = { false };
	bool pressedState[STATE_SIZE] = { false };
	bool releasedState[STATE_SIZE] = { false };

	//Input text events	
	std::string inputText;
	bool textChanged = false;
	bool TextInputMode = false;
	bool enterPressed = false;

	//Window Events
	bool windowResized = false;

	//Mouse Location
	int mouseX,mouseY;

	//Click Events
	bool mouseButtonHeld = false, mouseButtonReleased = false, mouseButtonClicked = false;

};