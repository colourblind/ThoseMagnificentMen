#ifndef THOSEMAGNIFICENTMEN_EXPLOSION_H
#define THOSEMAGNIFICENTMEN_EXPLOSION_H

#include "Vector.h"
#include "Plane.h"
#include "Constants.h"
#include "Maths.h"

namespace ThoseMagnificentMen
{
    struct Debris
    {
        Debris() { }
        Debris(Plane *victim) : 
            Position(victim->GetPosition()), 
            Velocity(victim->GetVelocity() + Vector2(GetRandom(-MAX_EXPLOSION_SPEED, MAX_EXPLOSION_SPEED), GetRandom(-MAX_EXPLOSION_SPEED, MAX_EXPLOSION_SPEED)) + victim->GetVelocity()), 
            Rotation(GetRandom(-1.0f, 1.0f)),
            RotationSpeed(GetRandom(-MAX_EXPLOSION_SPEED, MAX_EXPLOSION_SPEED)),
            Size(GetRandom(1.0f, 5.0f))
        { }

        Vector2 Position;
        Vector2 Velocity;
        float Rotation;
        float RotationSpeed;
        float Size;
    };

    class Explosion
    {
    public:
        Explosion(Plane *victim);

        bool Update(float msecs);
        void Render();

    private:
        Debris debris_[NUM_SHRAPNEL];
        float lifetime_;
    };

    class Smoke
    {
    public:
        Smoke(Plane *victim);

        bool Update(float msecs);
        void Render();

    private:
        Vector2 position_;
        float rotation_;
        float size_;
        float lifetime_;
    };
}

#endif // THOSEMAGNIFICENTMEN_EXPLOSION_H
