#pragma once
#include<vector>
#include<SDL_image.h>
#include<SDL.h>
#include"Text.h"
#include"Interaction.h"
#include"Utility.h"

/**
TODO
-Scroll
-Save to .txt file
-timestamps
-Show more/less than 7 lines at once(Probably not needed if a scroll feature is added)
-Commands
-Make updateSizeAndPosition() independent from external events/classes(Might NOT be worth it, CPU wise)
	*How to: Check if the size of the window's dimensions changed
-Make the Console completeley independant so that it doesnt require as much conjuction with other classes in the main function to work properley.(Currently there is too much clutter in the main function because of this class)
	*sendMessageToConsole(string)
	*show(): Will be used in the main function to turn on the console display when a Button is clicked or a hotkey is pressed
	*update()
	*render()
		+TOO MANY IMPORTANT METHODS FOR THE CONSOLE TO WORK PROPERLY, but seems easy to simpify everything down
*/

/** FEATURES
-Able to update the size and location of the console based on the windows size
*/

/** IMPORTANT METHODS
-----NOTE:Interaction::turnONTextInputMode() must be called in order for everything to work properly. 
-update(*Interactions): Updates the textbox in the console based on what the user is typing
-updateTextBox(string): Places a message in the console's text box
-isActive(): Returns true if the console is currently displayed and false otherwise
-insertMessage(string): Sends a string to the console's display
-toggleConsoleDisplay(): Turns ON/OFF the console display
-updateSizeAndPosition(): will change the size of the console. Should be used when a window resized event occurs. Can use in conjunction with Interaction::wasWindowResized() method.
-renderConsole(): Renders the console
*/

class Console
{
public:
	Console();
	Console(SDL_Renderer *ren, std::string backgroundFileName, Utility *win);
	~Console();

	//Initializers
		//Console Background
	void initializeBackgroundRect();
	void initializeBackgroundOpactity();
	void initializeBackgroundDimensions();
	void initializeBackground();
		//Text box
	void initializeTextBoxText();

	//Setter
	bool setBackgroundTexture(std::string textureFileName);
	void setnLinesInConsole(int n);

	//Getters
	int getnLinesInConsole();
		/**
		Returns True if the console is open
		returns false otherwise
		*/
	bool isActive();

	//Output Functions
	void insertMessage(std::string m);
	void renderBackground();
	void renderConsoleMessages();
	void renderConsole();
	bool toggleConsoleDisplay();
	
	//Text Box
	void updateTextBox(std::string output);
	void renderTextBoxText();

	//Updates the console with input from the Control class
	void update(Interaction *input);
	
	//Updates the position and size of the console based on the windows dimensions
	void updateSizeAndPosition();

private:
	Utility *window;

	bool displayConsole;
	int nLinesInConsole;
	SDL_Renderer *renderer;

	//Console Text
		//History/Messages
	std::vector<std::string> consoleOutputString;
	Text outputTextArray[7];
	int textHeight;
		//Text Box
	Text textBoxText;

	//background width, height, pos, and opacity
	int bgX, bgY, bgW, bgH, bgAlpha;
	SDL_Texture *bgTexture;
	SDL_Rect bgRect;

	//Error and Optimization
	bool isTooManyMessages;
	Text errorText;
	int messageCounter;

};

