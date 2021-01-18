#include"GameEngine.h"

//Main idead behind the game and how the code  should ultimately look

////////////////////int main(int argc, char *argv[])
////////////////////{
////////////////////	CGameEngine game;
////////////////////
////////////////////	// initialize the engine
////////////////////	game.Init("Engine Test v1.0");
////////////////////
////////////////////	// load the intro
////////////////////	game.ChangeState(CIntroState::Instance());
////////////////////
////////////////////	// main loop
////////////////////	while (game.Running())
////////////////////	{
////////////////////		game.HandleEvents();
////////////////////		game.Update();
////////////////////		game.Draw();
////////////////////	}
////////////////////
////////////////////	// cleanup the engine
////////////////////	game.Cleanup();
////////////////////	return 0;
////////////////////}

int main(int argc, char ** argv)
{
	GameEngine gameEngine;

	gameEngine.initialize();
/****************************************************************************************************
------------------MAIN GAME LOOP---------------------------------------------------------------------
*****************************************************************************************************/
	while (gameEngine.isRunning())
	{
		gameEngine.handleInput();

		gameEngine.update();

		gameEngine.render();
	}

	//Destroys all allocated memory to the window, renderer, and SDL
	gameEngine.close();

	return 0;
}
