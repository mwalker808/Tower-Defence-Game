#pragma once
#include"Console.h"
#include<SDL.h>
#include"HitBox.h"
#include"Text.h"
#include"Sprite.h"
#include"AnimatedSprite.h"

/** TODO
-Make it so that all buttons share the same texture, currently each Button will create their own seperate texture pointer for the button sprites, will gain approximatley 10MBs of memory
-Link buttons together so that if one button is clicked or toggeled on then it will turn off the buttons linked to it
-More options for the look of buttons
-Fix the text and where it appears on the button
-Create a hover info display that will give a breife description of what the buton does after hovering over the button for a set amount of time.
*/

/** FEATURES
-Button class allows you to create a button on a window. 
-Button(s) can be placed at a custom location or you can create a button(s) that will be set on the right side of the screen in two columns. That is based on which "createButton" function you use
-Buttons created can be toggleable.
-When a button is clicked, then the click event will be purged. This is to prevent buttons that activate click effects from happening under the button.
-Most interactions with a button (Clicked//toggled ON or OFF) will be sent to a Console object, where the Console object will display
*/

/**IMPORTANT METHODS
-update(): will update te state of all the buttons
-createButton: will create a button with a name, custom location(or generic location on the right of the window if no x,y coordinate for the button is given), toggleable or not
-isButtonON(string buttonName): Returns true of a button is in the ON state and false otherwise. You can search for a button based on the button's name.
-render(): Will render all of the buttons created and associated with the button object
*/
class Button
{
public:
	Button();
	Button(SDL_Renderer * ren, Interaction *controls, Console *DevConsole, int screenHeight, int screenWidth);
	~Button();

	//Setters
//	void setNRows(int n);
	void setNCols(int n);
//	void setRowsByCols(int i, int j);
	void setGenericButtonPositions(int xpos, int ypos);


	//Getters
	SDL_Renderer *getRenderer();
	Console getDeveloperConsole();
	int getGenericButtonWidth();
	int getGenericButtonHeight();

	//Button Graphics
		/** Holds the creation code to get the images for the buttons
		TODO: Make it so that you can edit how the buttons look*/
	void createButtonGraphics();

	//Button Generation
		/** Generates a new button with top cornner at (x,y), 
		if the botton is over another button then an error message is sent to the console and the button is not generated
		if toggleable is true then the button will be "on" until it is clicked again*/
	bool buttonNameExists(std::string buttonName);
	void createButton(std::string buttonName, bool toggleable, int x, int y, int w, int h);
	void createButton(std::string buttonName, bool toggleable);
	void createAnimatedButton(std::string buttonName, bool toggleable, AnimatedSprite aSprite);

	//Button Pressed Functions
		/** Sends a message to the console that a button was clicked*/
	void sendButtonClickedMessageToConsole(int buttonNumber);

	//Hovered Button
	bool checkForHoveredButton();

	//Update Buttons
	void disableAllButtons();
	void enableButtons();
	bool isButtonsDisabled();
	void updateClickedButtons();
	void updateHeldButtons();
	void updateONStatesOfButtons();
	void update();

	//Boolean buttons to check if a button is clicked
	int findButton(std::string buttonName);
	bool isButtonOn(std::string buttonName);
	bool isButtonOn(int n);
	bool buttonJustClickedOn(std::string buttonName);
	bool buttonJustClickedOff(std::string buttonName);
	void turnOffButton(int n);
	void turnOffAllButtons();

	//Links buttons together so that if one is pushed then all others are shut off
	void linkButtons(std::string names[], int size);

	//Render Function
	/**Renders the i-th button in */
	void render(int i);
	void render();

private:
	SDL_Renderer *renderer;
	Interaction *controls;
	Console *developerConsole;
	Sprite onSprite, offSprite, hoverSprite;
	bool disableButtons;
//	AnimatedSprite aOnSprite, aOffSprite, aHoverSprite;

	//Generic Button Starting point and dimensions
	int genericCounter, initialGenericX, initialGenericY, genericWidth, genericHeight, nCols;

	//MouseHitBox
	HitBox mouseHB;

	bool mouseFirstHeld;

	//Button Properties
		/** A counter and will set all buttons who you link together to the value of linkID*/
	int linkID;
	std::vector<SDL_Rect> buttonRectangles;
	std::vector<std::string> buttonNames;
	std::vector<Text> buttonText;
	std::vector<bool> buttonToggleable;
		/**Holds the boolean value for if a button was clicked*/
	std::vector<bool> buttonClicked;
		/**Holds the boolean value for if a button is currently Hovered over with the mouse*/
	std::vector<bool> buttonHovered;
	std::vector<bool> buttonHeld;
	/**Holds the boolean value if a button was selected, similar to a toggle on/off*/
	std::vector<HitBox> buttonHitBoxes;
	std::vector<bool> buttonON;
	std::vector<bool> buttonAnimated;

	std::vector<int> buttonLinkIDs;

	std::vector<std::vector<int>> allButtonLinkIDs;


};

