#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    ThoseMagnificentMen::Game game(hInstance);
	return game.Run();
}
