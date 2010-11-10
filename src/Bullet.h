#ifndef THOSEMAGNIFICENTMEN_BULLET_H
#define THOSEMAGNIFICENTMEN_BULLET_H

#include "Vector.h"
#include "Plane.h"

namespace ThoseMagnificentMen
{
    class Bullet
    {
    public:
        Bullet(Plane *owner);

        Vector2 GetPosition() const { return position_; }
        Plane *GetOwner() const { return owner_; }

        bool Update(float msecs);
        void Render();

    private:
        Vector2 position_;
        Vector2 velocity_;
        Plane *owner_;
    };
}

#endif // THOSEMAGNIFICENTMEN_BULLET_H
