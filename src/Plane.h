#ifndef THOSEMAGNIFICENTMEN_PLANE_H
#define THOSEMAGNIFICENTMEN_PLANE_H

#include "Vector.h"
#include "Constants.h"

namespace ThoseMagnificentMen
{
    class Bullet;

    class Plane
    {
    public:
        Plane();

        Vector2 GetPosition() const { return position_; }
        float GetRotation() const { return rotation_; }
        Vector2 GetVelocity() const { return Vector2(cos(rotation_) * speed_, sin(rotation_) * speed_); }
        bool IsStalled() const { return stalled_; }
        bool IsDying() const { return dying_; }
        bool IsInvincible() const { return invincible_ <= 0; }

        void Update(float msecs);
        void Render();
        void Reset();
        Bullet *Fire();

        void Kill() { dying_ = true; /* TEMP */ Reset(); }
        void ScoreUp() { score_ ++; }
        void ScoreDown() { score_ --; }
        void TurnLeft(float msecs) { rotation_ -= (TURN_SPEED * msecs); }
        void TurnRight(float msecs) { rotation_ += (TURN_SPEED * msecs); }

    private:
        Vector2 position_;
        float rotation_;
        float speed_;
        bool stalled_;
        bool dying_;
        float invincible_;
        int score_;
        float cooldown_;
    };
}

#endif // THOSEMAGNIFICENTMEN_PLANE_H