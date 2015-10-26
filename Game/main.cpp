#include "GameEngine.h"
#include <windows.h>

#define WIN32_LEAN_AND_MEAN

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	GameEngine gameEngine;
	gameEngine.Start(hInst);

	return 0;
}
