#include "Explosion.h"

#include "Constants.h"

using namespace ThoseMagnificentMen;

Explosion::Explosion(Plane *victim) : lifetime_(EXPLOSION_LENGTH)
{
    for (int i = 0; i < NUM_SHRAPNEL; i ++)
        debris_[i] = Debris(victim);
}

bool Explosion::Update(float msecs)
{
    lifetime_ -= msecs;

    return lifetime_ < 0;
}

void Explosion::Render()
{

}
