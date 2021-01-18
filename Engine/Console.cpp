#include "Console.h"

Console::Console()
{
	setBackgroundTexture("./fonts/Quicksand_Regular");
}

Console::Console(SDL_Renderer * ren, std::string backgroundFileName, Utility *win)
{
	window = win;
	displayConsole = false;
	setnLinesInConsole(7);
	renderer = ren;

	textHeight = Text(renderer, "./fonts/Quicksand_Regular.otf", 12, { 255,253,0 }, "WELCOME TO THE CONSOLE!!").getHeight();
	initializeBackgroundDimensions();//consoleOutput array must be initialized before this function

	initializeTextBoxText();

	int initialX = bgX + 10;
	int initialY = bgY + bgH - (textHeight + 3) * 2;


	for (int i = 0; i < 7; i++)
	{
		//		Text tempT(renderer, "./fonts/Quicksand_Regular.otf", 12, { 255,253,0 }, "WELCOME TO THE CONSOLE!!" + std::to_string(i));
		outputTextArray[i] = Text(renderer, "./fonts/Quicksand_Regular.otf", 12, { 255,253,0 }, "WELCOME TO THE CONSOLE!!");
		outputTextArray[i].setX(initialX);
		outputTextArray[i].setY(initialY - (textHeight + 3) * i);
	}

	setBackgroundTexture(backgroundFileName);
	initializeBackgroundOpactity();
	initializeBackgroundRect(); //Only use initializeRect() in the constructor

	//Error Messages Optomization
	isTooManyMessages = false;
	errorText = Text(renderer, "./fonts/Quicksand_Regular.otf", 40, { 255,0,0 }, "Possible that Too Many Messages sent to console too fast");
	messageCounter = 0;
}

Console::~Console()
{
}

void Console::initializeBackground()
{
}

void Console::initializeTextBoxText()
{
	Text tempT(renderer, "./fonts/Quicksand_Regular.otf", 12, { 255,100,255 }, "Press Hotkey to Type");
	textBoxText = tempT;
	textBoxText.setX(bgX + 10);
	textBoxText.setY(bgY + bgH - (textHeight + 3));
}

void Console::setnLinesInConsole(int n)
{
	nLinesInConsole = n;
}

int Console::getnLinesInConsole()
{
	return nLinesInConsole;
}

/**
Returns True if the console is open
returns false otherwise
*/
bool Console::isActive()
{
	return displayConsole;
}

//Only use initializeRect() in the constructor
//Used to initialize the rect object to the origional dimensions of the sprite
void Console::initializeBackgroundRect()
{
	bgRect.x = 0;
	bgRect.y = 0;
	bgRect.w = 1;
	bgRect.h = 1;
}

bool Console::setBackgroundTexture(std::string textureFileName)
{
//	SDL_DestroyTexture(bgTexture);
	bgTexture = IMG_LoadTexture(renderer, textureFileName.c_str());

	return true;
}

//0 is invisible, 255 is opaque
//255 is default
void Console::initializeBackgroundOpactity()
{
	SDL_SetTextureBlendMode(bgTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(bgTexture, 100);
}

//Initializes the size of the console dimensions
//The array consoleOutput MUST be initialized prior to calling this method.
void Console::initializeBackgroundDimensions()
{
	Utility temp = *window;

	bgW = temp.getWidth() - 2 * 10;
	bgH = (textHeight + 3) * (7 + 1);
	bgX = 10;
	bgY = temp.getHeight() - bgH - 10;
}

void Console::renderBackground()
{
	SDL_Rect dst;
	dst.x = bgX;
	dst.y = bgY;
	dst.w = bgW;
	dst.h = bgH;
	SDL_RenderCopy(renderer, bgTexture, &bgRect, &dst);
}

//Adds a message to the console output
//The message is added to the back of the end of the consoleOutput array
void Console::insertMessage(std::string m)
{
	consoleOutputString.push_back(m);
	for (int i = 6; i > 0; i--)
	{
		outputTextArray[i].setOutputText(outputTextArray[i-1].getOutputText());
	}
	outputTextArray[0].setOutputText(m);

	//COUNTS HOW MANY MESSAGES WERE ADDED TO THE CONSOLE
	//TODO: USE A TIMMER TO CREATE THE MESSAGE BASED ON IF INSERTS PER 3 SECONDS > 10
	messageCounter++;
	if (messageCounter > 500)
	{
		isTooManyMessages = true;
	}
}

void Console::renderConsoleMessages()
{
	for (int i = 0; i < 7; i++)
	{
		outputTextArray[i].render();
	}
}

//Prints the last nLinesInConsole messages sent to the console
void Console::renderConsole()
{
	if (displayConsole)
	{
		renderBackground();
		renderConsoleMessages();
		renderTextBoxText();
	}

	if (isTooManyMessages)
	{
		errorText.render();
	}
}

bool Console::toggleConsoleDisplay()
{
	displayConsole = !displayConsole;
	return displayConsole;
}

void Console::updateTextBox(std::string output)
{
	textBoxText.setOutputText(output);
}

void Console::renderTextBoxText()
{
	textBoxText.render();
}

void Console::update(Interaction *input)
{
	if ((*input).isInputTextChanged())
	{
		updateTextBox((*input).getInputText());
	}
	//Clears out the text box and sends the text to the console
	else if ((*input).isEnterPressed())
	{
		if ((*input).getInputText().compare("quit") == 0)
		{
			(*input).turnOFFTextInputMode();
			(*input).setInputText("");
			updateTextBox("Press P to type");
		}
		else
		{
			insertMessage((*input).getInputText());
			(*input).setInputText("");
			updateTextBox("type here");
		}
	}
}

void Console::updateSizeAndPosition()
{
	//Console Background position and size
	initializeBackgroundDimensions();

	//Textbox's position
	textBoxText.setX(bgX + 10);
	textBoxText.setY(bgY + bgH - (textHeight + 3));

	//Console history position
	int initialX = bgX + 10;
	int initialY = bgY + bgH - (textHeight + 3) * 2;
	for (int i = 0; i < 7; i++)
	{
		outputTextArray[i].setX(initialX);
		outputTextArray[i].setY(initialY - (textHeight + 3) * i);
	}

}
