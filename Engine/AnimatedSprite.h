#pragma once
#include<iostream>
#include<string>
#include <SDL.h>
#include<SDL_image.h>
#include <math.h> 
#include<vector>

/**
TODO
-Create an updateAnimation() method that will update the animation based on the time
-Create a changeAnimation(int) function that will change the animation to a different type (i.e. 1 = attack, 2 = defencive, 3 = running)
-renderAt(int, int):

*/

/** FEATURES
-
*/

/** IMPORTANT METHODS
-animate(): Will set the current image to the next one in its sequence
-render(): Renders the sprites current animation and frame to the renderer
*/

class AnimatedSprite
{
public:
	AnimatedSprite();
	AnimatedSprite(int *gTime);
	AnimatedSprite(std::vector<std::string> textureFiles, SDL_Renderer *ren, int *gTime, int width, int height, int xPos, int yPos);
	AnimatedSprite(std::string spriteSheetFileName, SDL_Renderer * ren, int *gTime, int xStart, int yStart, int n, int nRows, int nCols, int width, int height, int xPos, int yPos);
	AnimatedSprite(SDL_Texture* spriteSheetTexture, SDL_Renderer * ren, int *gTime, int xStart, int yStart, int n, int nRows, int nCols, int width, int height, int xPos, int yPos);
	~AnimatedSprite();

	void initializeRect();
	void AnimatedSprite::initializeRect(int xStart, int yStart, int n, int nRows, int nCols);
	void initializeTextures();

	//Getters
	int getX();
	int getY();
	int getH();
	int getW();
	int getCenterX();
	int getCenterY();
	int getCurrentTexture();
	SDL_Texture *getTexture(int n);
	SDL_Renderer *getRenderer();
	int getGameTime();

	//Setters
	bool setX(int xPos);
	bool setY(int yPos);
	void setCenterX();
	void setCenterY();
	void setPosition(int xPos, int yPos);
	bool setW(int width);
	bool setH(int height);
	void setCurrentTexture(int n);
	bool setTexture(std::string textureFileName, int n);
	bool setRenderer(SDL_Renderer* ren);
	void setTimeBetweenAnimations(int tBA);

	//Animation, function is called in the render function
	void animate();

	//Render Function
	void render();
	void changeOpactity(Uint8 alpha);
	void changeColor(SDL_Color color);
	void changeWidth(int iW);
	void changeHeight(int iH);
	void rotate(double degrees);
	void rotateBy(double degrees);

	void renderAt(int xPos, int yPos);
	void renderCopyAt(int xPos, int yPos);


private:
	int * gameTime;
	int size;
	std::vector<std::string> textureFileNames;
	std::vector<SDL_Texture*> textures;
	SDL_Renderer *renderer;
	std::vector<SDL_Rect> rect;
	SDL_Rect dst;

	bool sourceIsSpriteSheet;

	int x, y, w, h, centerX, centerY;
	double mDegrees = 0;
	int currentTexture;
	int mAlpha;

	int timeBetweenAnimations, timeOfLastAnimation;
};

