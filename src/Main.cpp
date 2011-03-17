#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sstream>

#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    std::stringstream ss(lpCmdLine);

    int humanPlayers = 1;
    int bots = 1;

    ss >> humanPlayers >> bots;

    if (humanPlayers > 4)
        humanPlayers = 4;
    if (humanPlayers + bots > 4)
        bots = 4 - humanPlayers;

    ThoseMagnificentMen::Game game(hInstance, humanPlayers, bots);
	return game.Run();
}
