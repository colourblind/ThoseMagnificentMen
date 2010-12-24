#ifndef THOSEMAGNIFICENTMEN_OPENGL_H
#define THOSEMAGNIFICENTMEN_OPENGL_H

#include <windows.h>
#include <gl/gl.h>

struct SpriteBlock
{
    float x;
    float y;
    float width;
    float height;
};

const SpriteBlock SPRITES[7] = {
    {0, 0, 16, 16},
    {16, 0, 16, 16},
    {32, 0, 16, 16},
    {48, 0, 16, 16},
    {64, 0, 16, 16},
    {80, 0, 16, 16},
    {0, 16, 64, 64}
};

const int SPRITE_PLAYER_1 = 0;
const int SPRITE_PLAYER_2 = 1;
const int SPRITE_PLAYER_3 = 2;
const int SPRITE_PLAYER_4 = 3;
const int SPRITE_EXPLOSION = 4;
const int SPRITE_SMOKE = 5;
const int SPRITE_GROUND = 6;

#endif // THOSEMAGNIFICENTMEN_OPENGL_H
