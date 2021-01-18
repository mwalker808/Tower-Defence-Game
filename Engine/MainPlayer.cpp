#include "MainPlayer.h"



MainPlayer::MainPlayer()
{
}

MainPlayer::MainPlayer(AnimatedSprite aS, int * gameTime, int x0, int y0)
{
	animatedSprite = aS;
	elapsedTime = gameTime;
	x = x0;
	y = y0;
	dY = 0;
	dX = 0;
	walkSpeed = 4;
	fallSpeed = 3;
	jumping = false;
	timeOfJump = 0;
	jumpTime = 500;
	jumpSpeed = -3;

	hitBox = HitBox(animatedSprite.getRenderer(), animatedSprite.getX(), animatedSprite.getY(), animatedSprite.getW(), animatedSprite.getH());
}


MainPlayer::~MainPlayer()
{
}

void MainPlayer::setPosition(int x0, int y0)
{
	x = x0;
	y = y0;
	animatedSprite.setX(x0);
	animatedSprite.setY(y0);
	hitBox.moveTo(x0, y0);
}

void MainPlayer::setHitBox(HitBox hb)
{
	hitBox = hb;
}

int MainPlayer::getX()
{
	return x;
}

int MainPlayer::getY()
{
	return y;
}

HitBox MainPlayer::getHitBox()
{
	return hitBox;
}

void MainPlayer::moveBy(int x0, int y0)
{
	setPosition(x0 + getX(), y0 + getY());
}

void MainPlayer::walkLeft()
{
	dX = -walkSpeed;
}

void MainPlayer::walkRight()
{
	dX = walkSpeed;
}
void MainPlayer::jump()
{
	jumping = true;
}

void MainPlayer::handleEvents(Interaction controls)
{
}

void MainPlayer::updatePosition(std::vector<HitBox> hitBoxVector, Interaction controls)
{
	dX = 0;
	dY = fallSpeed;

	if (controls.keyHeld(SDL_SCANCODE_LEFT))
	{
		walkLeft();
	}
	if (controls.keyHeld(SDL_SCANCODE_RIGHT))
	{
		walkRight();
	}
	if (controls.keyPressed(SDL_SCANCODE_SPACE))
	{
		jump();
	}

	if (jumping)
	{
		if (timeOfJump == 0)
		{
			timeOfJump = *elapsedTime;
		}
		else if (*elapsedTime >= timeOfJump + jumpTime)
		{
			timeOfJump = 0;
			jumping = false;
		}
		else
		{
			dY = jumpSpeed;
		}
	}

	//////////////Currently testing the functionality of futureCollisionCheck method
	//////////////Will check if there will be a collision in the x and y dirrection, if so then there will be a computation done
	//////////////to see how far hb1 can move before touching hb2 n each dirrection	
	for (int i = 0; i < hitBoxVector.size(); i++)
	{
		//HORIZONTAL CHECK
		if (getHitBox().futureCollisionCheck(hitBoxVector[i], dX, 0))
		{
			if (dX > 0)//Moving right
			{
				dX = getHitBox().getX() + getHitBox().getW() - hitBoxVector[i].getX();
			}
			else if (dX < 0)//Moving left
			{
				dX = getHitBox().getX() - hitBoxVector[i].getX() - hitBoxVector[i].getW();
			}
		}
		//VERITICAL CHECK
		if (getHitBox().futureCollisionCheck(hitBoxVector[i], 0, dY))
		{
			if (dY > 0)//Moving down
			{
				dY = hitBoxVector[i].getY() - getHitBox().getY() - getHitBox().getH();
			}
			else if (dY < 0)//Moving up
			{
				jumping = false;
				timeOfJump = 0;
//				developerConsole.insertMessage(std::to_string(getHitBox().getY()) + " ");
				dY = getHitBox().getY() - hitBoxVector[i].getY() - hitBoxVector[i].getH();
			}
		}
		//DIAGONAL CHECK
		/*NEEDED IN ORDER TO MAKE SURE THAT THE CORNNER DOES NOT GO THROUGH THE CORNNER OF THE HITBOXES*/
		if (getHitBox().futureCollisionCheck(hitBoxVector[i], dX, dY))
		{
			if (dX > 0 && dY > 0)
			{
				dX = getHitBox().getX() + getHitBox().getW() - hitBoxVector[i].getX();
				dY = hitBoxVector[i].getY() - getHitBox().getY() - getHitBox().getH();
			}
			else if (dX > 0 && dY < 0)
			{
				dX = getHitBox().getX() + getHitBox().getW() - hitBoxVector[i].getX();
				dY = getHitBox().getY() - hitBoxVector[i].getY() - hitBoxVector[i].getH();
			}
			else if (dX < 0 && dY > 0)
			{
				dX = getHitBox().getX() - hitBoxVector[i].getX() - hitBoxVector[i].getW();
				dY = hitBoxVector[i].getY() - getHitBox().getY() - getHitBox().getH();
			}
			else if (dX < 0 && dY < 0)
			{
				dX = getHitBox().getX() - hitBoxVector[i].getX() - hitBoxVector[i].getW();
				dY = getHitBox().getY() - hitBoxVector[i].getY() - hitBoxVector[i].getH();
			}
		}
	}

	moveBy(dX, dY);
}

void MainPlayer::update()
{
}

void MainPlayer::render()
{
	animatedSprite.animate();
	animatedSprite.render();

	if (showHitBox)
	{
		hitBox.render();
	}
}
