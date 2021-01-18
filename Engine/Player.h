#pragma once
#include"Console.h"

class Player
{
public:
	Player();
	Player(Console * dConsole);
	~Player();

	//SETTERS
	void setCash(int c);
	void setHealth(int h);

	//GETTERS
	int getCash();
	int getHealth();
	bool isAlive();

	//MISC
	void displayPlayerInfoToConsole();



private:
	Console* devConsole;
	int cash, health;
};

