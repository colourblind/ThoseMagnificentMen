#include "Game.h"

#include "Maths.h"
#include "Constants.h"
#include "Loaders.h"
#include "Resource.h"

using namespace std;
using namespace ThoseMagnificentMen;

Game::Game(HINSTANCE hInstance) : activePlayers_(2), texture_(0)
{
    window_.Create(hInstance);

    keyBindings_[0].left = KeyZ;
    keyBindings_[0].right = KeyX;
    keyBindings_[0].fire = KeyC;

    keyBindings_[1].left = KeyI;
    keyBindings_[1].right = KeyO;
    keyBindings_[1].fire = KeyP;

    keyBindings_[2].left = KeyLeft;
    keyBindings_[2].right = KeyRight;
    keyBindings_[2].fire = KeyUp;

    keyBindings_[3].left = KeyNumPad4;
    keyBindings_[3].right = KeyNumPad5;
    keyBindings_[3].fire = KeyNumPad6;
}

Game::~Game()
{
    for (unsigned int i = 0; i < bullets_.size(); i ++)
        delete bullets_[i];
    for (unsigned int i = 0; i < explosions_.size(); i ++)
        delete explosions_[i];
    for (unsigned int i = 0; i < smokes_.size(); i ++)
        delete smokes_[i];

    if (texture_)
        glDeleteTextures(1, &texture_);
}

int Game::Run()
{
    if (!window_.Init(&input_))
        return 1;

    texture_ = Loaders::LoadTexture(IDB_SPRITES);

    InitRandom();
    for (unsigned int i = 0; i < activePlayers_; i ++)
        players_[i].Reset(i + 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    LARGE_INTEGER perfFreq;
    ::QueryPerformanceFrequency(&perfFreq);
    float convertToMillis = 1000.0f / static_cast<float>(perfFreq.QuadPart);

    LARGE_INTEGER current;
    LARGE_INTEGER lastFrame;
    ::QueryPerformanceCounter(&lastFrame);

    MSG msg;
    while(true)
    {
    	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
        else
        {
            ::QueryPerformanceCounter(&current);
            float msecs = static_cast<float>(current.QuadPart - lastFrame.QuadPart) * convertToMillis;
            lastFrame = current;

            HandleControls(msecs);
            UpdateAndRender(msecs);
            GameLogic();

            window_.SwapBuffers();
        }
    }

    return 0;
}

void Game::HandleControls(float msecs)
{
    for (int i = 0; i < activePlayers_; i ++)
    {
        if (!players_[i].IsDying())
        {
            if (input_.GetKeyDown(keyBindings_[i].left))
                players_[i].TurnLeft(msecs);
            if (input_.GetKeyDown(keyBindings_[i].right))
                players_[i].TurnRight(msecs);
        }
        if (input_.GetKeyDown(keyBindings_[i].fire))
        {
            Bullet * bullet = players_[i].Fire();
            if (bullet != NULL)
                bullets_.push_back(bullet);
        }
    }
}

void Game::UpdateAndRender(float msecs)
{
	glClear(GL_COLOR_BUFFER_BIT);

    float width = static_cast<float>(WIDTH);
    float height = static_cast<float>(HEIGHT);

    // Render sky
    glBegin(GL_QUADS);
        glColor3f(0.75f, 0.75f, 1.0f);
        glVertex2f(0, 0);
        glVertex2f(width, 0);
        glColor3f(0, 0, 0.5f);
        glVertex2f(width, height);
        glVertex2f(0, height);
    glEnd();

    // Render ground
    // This is hacky as hell because I can't think of a decent way of tiling a subtexture.
    SpriteBlock sprite = SPRITES[SPRITE_GROUND];
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glColor3f(1, 1, 1);
    for (int i = 0; i < width; i += sprite.width)
    {
        for (int j = GROUND_LEVEL; j > 0 - sprite.height; j -= sprite.height)
        {
            glBegin(GL_QUADS);
                glTexCoord2f(sprite.x / 128, (sprite.y + sprite.height) / 128);
                glVertex2f(i, j);
                glTexCoord2f((sprite.x + sprite.height) / 128, (sprite.y + sprite.height) / 128);
                glVertex2f(i + sprite.width, j);
                glTexCoord2f((sprite.x + sprite.width) / 128, sprite.y / 128);
                glVertex2f(i + sprite.width, j - sprite.height);
                glTexCoord2f(sprite.x / 128, sprite.y / 128);
                glVertex2f(i, j - sprite.height);
            glEnd();
        }
    }

    for (unsigned int i = 0; i < activePlayers_; i ++)
    {
        players_[i].Update(msecs);
        players_[i].Render();

        if (players_[i].IsDying() && players_[i].ReleaseSmoke())
            smokes_.push_back(new Smoke(&players_[i]));
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

    vector<Smoke *>::iterator smokeIter = smokes_.begin();
    while (smokeIter != smokes_.end())
    {
        if ((*smokeIter)->Update(msecs))
        {
            delete *smokeIter;
            smokeIter = smokes_.erase(smokeIter);
        }
        else
        {
            (*smokeIter)->Render();
            smokeIter ++;
        }
    }
    glDisable(GL_TEXTURE_2D);

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
}

void Game::GameLogic()
{
    for (unsigned int i = 0; i < activePlayers_; i ++)
    {
        // Crashed into the ground
        if (players_[i].GetPosition().y < GROUND_LEVEL + 12)
        {
            if (!players_[i].IsDying()) // No excuse
                players_[i].ScoreDown();
            Explosion *bang = new Explosion(&players_[i]);
            explosions_.push_back(bang);
            players_[i].Reset();
            continue;
        }

        if (players_[i].IsInvincible())
            continue;

        // Collisions with other players
        for (unsigned int j = i + 1; j < activePlayers_; j ++)
        {
            if ((players_[i].GetPosition() - players_[j].GetPosition()).Length() < 20 && !players_[j].IsInvincible())
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
                if (!players_[i].IsDying())
                    bullets_[j]->GetOwner()->ScoreUp();
                delete bullets_[j];
                vector<Bullet *>::iterator iter = bullets_.begin();
                iter += j;
                bullets_.erase(iter);
                break;
            }
        }
    }
}
