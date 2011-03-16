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
    struct KeyBindings
    {
        Key left;
        Key right;
        Key fire;
    };

    class Game
    {
    public:
        Game(HINSTANCE hInstance);
        ~Game();

        int Run();

    private:
        void HandleControls(float msecs);
        void HandleAI(float msecs);
        void UpdateAndRender(float msecs);
        void GameLogic();
        int TotalPlayers() { return activePlayers_ + botPlayers_; }

        Window window_;
        Input input_;
        Plane players_[4];
        KeyBindings keyBindings_[4];
        std::vector<Bullet *> bullets_;
        std::vector<Explosion *> explosions_;
        std::vector<Smoke *> smokes_;
        unsigned int activePlayers_;
        unsigned int botPlayers_;
        GLuint texture_;
    };
}

#endif // THOSEMAGNIFICENTMEN_GAME_H
