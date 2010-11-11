#include "Game.h"

#include "Maths.h"
#include "Constants.h"

using namespace std;
using namespace ThoseMagnificentMen;

Game::Game(HINSTANCE hInstance) : activePlayers_(2)
{
    window_.Create(hInstance);
}

Game::~Game()
{
    for (unsigned int i = 0; i < bullets_.size(); i ++)
        delete bullets_[i];

    for (unsigned int i = 0; i < explosions_.size(); i ++)
        delete explosions_[i];
}

int Game::Run()
{
    if (!window_.Init(&input_))
        return 1;

    InitRandom();
    for (unsigned int i = 0; i < activePlayers_; i ++)
        players_[i].Reset();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 0, 768, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    MSG msg;
    while(true)
    {
        float msecs = 10;

        HandleControls(msecs);
        UpdateAndRender(msecs);
        GameLogic();

    	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

        window_.SwapBuffers();
    }

    return 0;
}

void Game::HandleControls(float msecs)
{
    if (!players_[0].IsDying())
    {
        if (input_.GetKeyDown(KeyZ))
            players_[0].TurnLeft(msecs);
        if (input_.GetKeyDown(KeyX))
            players_[0].TurnRight(msecs);
    }
    if (input_.GetKeyDown(KeyC))
    {
        Bullet * bullet = players_[0].Fire();
        if (bullet != NULL)
            bullets_.push_back(bullet);
    }

    if (!players_[1].IsDying())
    {
        if (input_.GetKeyDown(KeyI))
            players_[1].TurnLeft(msecs);
        if (input_.GetKeyDown(KeyO))
            players_[1].TurnRight(msecs);
    }
    if (input_.GetKeyDown(KeyP))
    {
        Bullet * bullet = players_[1].Fire();
        if (bullet != NULL)
            bullets_.push_back(bullet);
    }
}

void Game::UpdateAndRender(float msecs)
{
	glClear(GL_COLOR_BUFFER_BIT);

    for (unsigned int i = 0; i < activePlayers_; i ++)
    {
        players_[i].Update(msecs);
        players_[i].Render();
    }

    vector<Bullet *>::iterator bulletIter = bullets_.begin();
    while (bulletIter != bullets_.end())
    {
        if ((*bulletIter)->Update(msecs))
        {
            delete *bulletIter;
            bulletIter = bullets_.erase(bulletIter);
        }
        else
        {
            (*bulletIter)->Render();
            bulletIter ++;
        }
    }

    vector<Explosion *>::iterator exploIter = explosions_.begin();
    while (exploIter != explosions_.end())
    {
        if ((*exploIter)->Update(msecs))
        {
            delete *exploIter;
            exploIter = explosions_.erase(exploIter);
        }
        else
        {
            (*exploIter)->Render();
            exploIter ++;
        }
    }
}

void Game::GameLogic()
{
    for (unsigned int i = 0; i < activePlayers_; i ++)
    {
        // Crashed into the ground
        if (players_[i].GetPosition().y < GROUND_LEVEL)
        {
            if (!players_[i].IsDying()) // No excuse
                players_[i].ScoreDown();
            Explosion *bang = new Explosion(&players_[i]);
            explosions_.push_back(bang);
            players_[i].Reset();
            continue;
        }

        // Collisions with other players
        for (unsigned int j = i + 1; j < activePlayers_; j ++)
        {
            if ((players_[i].GetPosition() - players_[j].GetPosition()).Length() < 20)
            {
                Explosion *bang1 = new Explosion(&players_[i]);
                Explosion *bang2 = new Explosion(&players_[j]);
                explosions_.push_back(bang1);
                explosions_.push_back(bang2);
                players_[i].Reset();
                players_[j].Reset();
                break;
            }
        }

        // Collisions with bullets
        for (unsigned int j = 0; j < bullets_.size(); j ++)
        {
            if ((players_[i].GetPosition() - bullets_[j]->GetPosition()).Length() < 20)
            {
                players_[i].Kill();
                bullets_[j]->GetOwner()->ScoreUp();
                break;
            }
        }
    }
}
