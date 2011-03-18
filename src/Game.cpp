#include "Game.h"

#include "Constants.h"
#include "Loaders.h"
#include "Resource.h"

using namespace std;
using namespace ThoseMagnificentMen;

Game::Game(HINSTANCE hInstance, int humanPlayers, int bots) : activePlayers_(humanPlayers), botPlayers_(bots), texture_(0)
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
    for (unsigned int i = 0; i < TotalPlayers(); i ++)
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
    convertToMillis *= GAME_SPEED;

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

            UpdateAndRender(msecs);
            HandleControls(msecs);
            HandleAI(msecs);
            GameLogic();

            window_.SwapBuffers();
        }
    }

    return 0;
}

void Game::HandleControls(float msecs)
{
    // Get inputs for player controlled planes
    for (int i = 0; i < activePlayers_; i ++)
    {
        if (!players_[i].IsDying())
        {
            if (input_.GetKeyDown(keyBindings_[i].left))
                players_[i].TurnLeft(msecs);
            if (input_.GetKeyDown(keyBindings_[i].right))
                players_[i].TurnRight(msecs);
            if (input_.GetKeyDown(keyBindings_[i].fire))
            {
                Bullet *bullet = players_[i].Fire();
                if (bullet != NULL)
                    bullets_.push_back(bullet);
            }
        }
    }
}

void Game::HandleAI(float msecs)
{
    // Run AI for computer planes
    for (int i = activePlayers_; i < TotalPlayers(); i ++)
    {
        // Bail if it's dying or stalled. We're not allowing sniping while stalling, the
        // bots are just too good a shot
        Plane *current = &players_[i];
        if (current->IsDying() || current->IsStalled())
            continue;

        // Find the closets enemy plane
        Plane *target = NULL;
        float currentRange = 9999;
        for (int j = 0; j < TotalPlayers(); j ++)
        {
            if (j == i) // Can't target self
                continue;

            // We're currently ignoring respawning players. May remove this later to make
            // the AI more aggressive
            if (currentRange > current->GetPosition().Distance(players_[j].GetPosition())
                && !players_[j].IsDying() && !players_[j].IsStalled() && !players_[j].IsInvincible())
            {
                target = &players_[j];
                currentRange = current->GetPosition().Distance(target->GetPosition());
            }
        }

        if (target == NULL) // Level off if we don't have a target
        {
            float targetAngle = 0;
            if (targetAngle > current->GetRotation())
                current->TurnLeft(msecs);
            else
                current->TurnRight(msecs);
        }
        else
        {
            // Plot an intercept course!
            Vector2 currentPosition = current->GetPosition();
            Vector2 targetPosition = target->GetPosition();
            Vector2 interceptPoint = Vector2::CollisionPoint(currentPosition, current->GetVelocity(), targetPosition, target->GetVelocity());

            // If the target is running parallel to us, go for them
            //if (interceptPoint.x == 0 && interceptPoint.y == 0)
            //    interceptPoint = targetPosition;
            
            // If the target is in front of us, try and lead it
            // Calculate time for bullet to reach intercept point
            Vector2 bulletStartPos = currentPosition + current->GetVelocity().Normalise() * (20 + MAX_SPEED + 0.5f);
            float bulletTravelTime = (bulletStartPos - interceptPoint).Length() / BULLET_SPEED;
            // Calculate target position at that time
            Vector2 projectedTargetPos = targetPosition + (target->GetVelocity() * bulletTravelTime);

            // Decide if it's worth a shot
            if (interceptPoint.Distance(projectedTargetPos) < 20)
            {
                Bullet *bullet = current->Fire();
                if (bullet != NULL)
                    bullets_.push_back(bullet);
            }

            // Don't play chicken
            // TODO: Turn away from collisions

            // Don't derp
            // TODO: Don't run into the ground

            // Attempt to maneuver for a better shot
            Vector2 currentVelocity = current->GetVelocity().Normalise();
            Vector2 targetVelocity = (currentPosition - projectedTargetPos).Normalise();

            float diffAngle = currentVelocity.Cross(targetVelocity);

            if (diffAngle < 0)
                current->TurnLeft(msecs);
            else
                current->TurnRight(msecs);

            if (DEBUG_RENDERER)
            {
                glBegin(GL_QUADS);
                    glColor3f(0, 0, 0);
                    glVertex2f(interceptPoint.x - 2, interceptPoint.y - 2);
                    glVertex2f(interceptPoint.x + 2, interceptPoint.y - 2);
                    glVertex2f(interceptPoint.x + 2, interceptPoint.y + 2);
                    glVertex2f(interceptPoint.x - 2, interceptPoint.y + 2);
                glEnd();

                glBegin(GL_QUADS);
                    glColor3f(1, 0, 1);
                    glVertex2f(projectedTargetPos.x - 15, projectedTargetPos.y - 15);
                    glVertex2f(projectedTargetPos.x + 15, projectedTargetPos.y - 15);
                    glVertex2f(projectedTargetPos.x + 15, projectedTargetPos.y + 15);
                    glVertex2f(projectedTargetPos.x - 15, projectedTargetPos.y + 15);
                glEnd();
            }
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

    for (unsigned int i = 0; i < TotalPlayers(); i ++)
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
    for (unsigned int i = 0; i < TotalPlayers(); i ++)
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
        for (unsigned int j = i + 1; j < TotalPlayers(); j ++)
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
