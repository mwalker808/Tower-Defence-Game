#pragma once
#include"HitBox.h"
#include"AnimatedSprite.h"
#include"Interaction.h"

class MainPlayer
{
public:
	MainPlayer();
	MainPlayer(AnimatedSprite aS, int *gameTime, int x0, int y0);
	~MainPlayer();

	//initialize

	//Setters
	void setAnimationDelay(int delay);
	void setPosition(int x0, int y0);
	void setHitBox(HitBox hb);

	//Getters
	int getX();
	int getY();
	HitBox getHitBox();

	//Motion Functions
	void moveBy(int x0, int y0);
	void walkLeft();
	void walkRight();
	void jump();

	//HANDLE EVENTS
	void handleEvents(Interaction controls);

	//UPDATE
	void updatePosition(std::vector<HitBox> hBs, Interaction controls);
	void update();

	//Rendering functions
	void render();

private:
	int x, y, dX, dY, walkSpeed, fallSpeed;

	int *elapsedTime;
	HitBox hitBox;
	AnimatedSprite animatedSprite;

	bool jumping;
	int timeOfJump, jumpTime, jumpSpeed;

	bool showHitBox = true;
	
};

