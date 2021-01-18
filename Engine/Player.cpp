#include "Player.h"



Player::Player()
{
}

Player::Player(Console * dConsole)
{
	devConsole = dConsole;
}


Player::~Player()
{
}

void Player::setCash(int c)
{
	if (c > 99999)//99,999 IS THE MAXIMUM CASH OMEONE CAN HAVE
	{
		cash = 99999;
	}
	else
	{
		cash = c;
	}
}

void Player::setHealth(int h)
{
	health = h;
}

int Player::getCash()
{
	return cash;
}

int Player::getHealth()
{
	return health;
}

bool Player::isAlive()
{
	return getHealth() > 0;
}
