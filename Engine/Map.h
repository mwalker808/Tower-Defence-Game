#pragma once
#include"Console.h"
#include"AnimatedSprite.h"
#include<iostream>
#include<iomanip>
#include<fstream>

class Map
{
public:
	Map();
	Map(Console *devConsole, Interaction * cont, SDL_Renderer * ren );
	~Map();

	//Setters
	void setMapIntValue(int i, int j, int v);//0: default, 1: free tower tiles, 2: taken tower Tile

	//Getters
	int getTileWidth();
	int getTileHeight();
	int getMapWidth();
	int getMapHeight();
	int getMapIntValue(int i, int j); //0: default, 1: tower tiles, 2: taken tower Tile, 3: Starting tile, 4: Ending tile
	int getSpawnI(); //Returns the i value for where the starting tile is located in the Map, if no starting tile exists then -1 is returned by default
	int getSpawnJ(); //Returns the j value for where the starting tile is located in the Map, if no starting tile exists then -1 is returned by default


	//Misc
	void initializeTestMap();
	void saveMapToFile(std::string fileName);
	void loadMapFromFile(std::string fileName);
	void reloadMap();//reloads the current map to its default tiles


	//Render
	void renderSkeleton();
	void renderMiniMap();
	void render();


private:
	Console * developerConsole;
	Interaction * controls;
	SDL_Renderer *renderer;
	std::string currentLoadedMapFileName;
	int mapX, mapY;//The starting point for the map
	int mapWidth, mapHeight, tileWidth, tileHeight;
	std::vector<AnimatedSprite> tiles; //0: default, 1: tower tiles, 2: taken tower Tile, 3: Starting tile, 4: Ending tile
	
	static const int nRows = 15, nCols = 25;
	int mapIntValue[nRows][nCols];
	AnimatedSprite mapSpriteTemplate[nCols][nRows];


};