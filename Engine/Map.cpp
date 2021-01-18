#include "Map.h"

Map::Map()
{
	mapX = 0;
	mapY = 0;
	tileWidth = 40;
	tileHeight = 40;
	//nRows = 15;
	//nCols = 25;
	mapWidth = nCols * tileWidth;
	mapHeight = nRows * tileHeight;
}

Map::Map(Console * devConsole, Interaction * cont, SDL_Renderer * ren)
{
	developerConsole = devConsole;
	renderer = ren;
	mapX = 0;
	mapY = 0;
	tileWidth = 40;
	tileHeight = 40;
	//nRows = 15;
	//nCols = 25;
	mapWidth = nCols * tileWidth;
	mapHeight = nRows * tileHeight;
//	(*developerConsole).insertMessage(std::to_string(mapWidth) + "x" + std::to_string(mapHeight));

	initializeTestMap();
}

Map::~Map()
{
}

void Map::setMapIntValue(int i, int j, int v)
{
	if (i < nRows && j < nCols)
	{
		mapIntValue[i][j] = v;
	}
}

int Map::getTileWidth()
{
	return tileWidth;
}

int Map::getTileHeight()
{
	return tileHeight;
}

int Map::getMapWidth()
{
	mapWidth = nCols * tileWidth;
	return 	mapWidth;
}

int Map::getMapHeight()
{
	mapWidth = nRows * tileHeight;
	return mapHeight;
}

int Map::getMapIntValue(int i, int j)
{
//	(*developerConsole).insertMessage(std::to_string(i) + " " + std::to_string(j));
	if ((i >= 0 && i < nRows) && (j >= 0 && j < nCols))
	{
//		(*developerConsole).insertMessage(std::to_string(mapIntValue[i][j]));
		return mapIntValue[i][j];
	}
	else
	{
		return -1;
	}
}

int Map::getSpawnI()
{
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			if (mapIntValue[i][j] == 3)
			{
				return i;
			}
		}
	}
	return -1;
}

int Map::getSpawnJ()
{
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			if (mapIntValue[i][j] == 3)
			{
				return j;
			}
		}
	}
	return -1;
}

void Map::initializeTestMap()
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if (i == 0 && j == 24)
			{
				mapIntValue[i][j] = 4;//END
			}
			else if (i == nRows - 1 && j == 0)
			{
				mapIntValue[i][j] = 3;//START
			}
			else if (j % 3 != 0)
			{
				if ((j / 3) % 2 == 0 && i == 0)
				{
					mapIntValue[i][j] = 0;
				}
				else if ((j / 3) % 2 == 1 && i == 14)
				{
					mapIntValue[i][j] = 0;
				}
				else
				{
					mapIntValue[i][j] = 1;
				}
			}
			else
			{
				mapIntValue[i][j] = 0;
			}
		}
	}
}

void Map::saveMapToFile(std::string fileName)
{
	std::ofstream outFile;
	outFile.open(fileName);
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			outFile << (std::to_string(mapIntValue[i][j]) + " ");
		}
	}
	outFile.close();
}

void Map::loadMapFromFile(std::string fileName)
{
	std::ifstream inFile;
	int tempInt = 0;
	int sum = 0;
	int n = 0;

	inFile.open(fileName);
	if (!inFile) {
		if (fileName == "")
		{
			(*developerConsole).insertMessage("Unable to open map file: " + std::string("NO FILE NAME GIVEN"));
		}
		else
		{
			(*developerConsole).insertMessage("Unable to open map file: " + fileName);
		}
	}
	else
	{
		currentLoadedMapFileName = fileName;
		while (inFile >> tempInt)
		{
			sum += tempInt;
			mapIntValue[n / nCols][n % nCols] = tempInt;
			n++;
		}
		inFile.close();
	}

}

void Map::reloadMap()
{
	loadMapFromFile(currentLoadedMapFileName);
}

void Map::renderSkeleton()
{
	SDL_Rect temp = { 0,0,0,0 };
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if ((i + j) % 2 == 0)
			{
				switch (mapIntValue[i][j])
				{
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
					break;
				case 4:
					SDL_SetRenderDrawColor(renderer, 255, 20, 147, 255);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				}
			}
			else
			{
				switch (mapIntValue[i][j])
				{
				case 0:
					SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 0, 230, 0, 255);
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 230, 230, 0, 255);
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 230, 0, 230, 255);
					break;
				case 4:
					SDL_SetRenderDrawColor(renderer, 230, 20, 147, 255);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 230, 0, 0, 255);
				}
			}
			temp = { mapX + j * tileWidth, mapY + i * tileHeight,tileWidth, tileHeight };
			SDL_RenderFillRect(renderer, &temp);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}

void Map::renderMiniMap()
{
	SDL_Rect temp = { 0,0,0,0 };
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if ((i + j) % 2 == 0)
			{
				switch (mapIntValue[i][j])
				{
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				}
			}
			else
			{
				switch (mapIntValue[i][j])
				{
				case 0:
					SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 0, 230, 0, 255);
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 230, 230, 0, 255);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 230, 0, 0, 255);
				}
			}
			temp = { 1240 + j * tileWidth / 10, 30 + i * tileHeight / 10,tileWidth / 10 , tileHeight / 10 };
			SDL_RenderFillRect(renderer, &temp);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}
