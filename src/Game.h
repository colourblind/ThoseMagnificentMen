#ifndef THOSEMAGNIFICENTMEN_GAME_H
#define THOSEMAGNIFICENTMEN_GAME_H

#include <vector>

#include "Window.h"
#include "OpenGL.h"
#include "Plane.h"
#include "Bullet.h"
#include "Explosion.h"

namespace ThoseMagnificentMen
{
    class Game
    {
    public:
        Game(HINSTANCE hInstance);
        ~Game();

        int Run();

    private:
        void HandleControls(float msecs);
        void UpdateAndRender(float msecs);
        void GameLogic();

        Window window_;
        Input input_;
        Plane players_[4];
        std::vector<Bullet *> bullets_;
        std::vector<Explosion *> explosions_;
        unsigned int activePlayers_;
    };
}

#endif // THOSEMAGNIFICENTMEN_GAME_H
