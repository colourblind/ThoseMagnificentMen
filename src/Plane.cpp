#include "Plane.h"

#include "Maths.h"
#include "OpenGL.h"
#include "Bullet.h"

using namespace ThoseMagnificentMen;

Plane::Plane() : score_(0)
{
    Reset();
}

void Plane::Update(float msecs)
{
    if (invincible_ > 0)
        invincible_ -= msecs;
    if (cooldown_ > 0)
        cooldown_ -= msecs;

    if (speed_ < STALL_SPEED)
    {
        speed_ = 0;
        stalled_ = true;
    }

    if (stalled_)
    {
        speed_ += GRAVITY;
        position_.y -= speed_;
    }
    else
    {
        if (dying_)
        {
            // swan dive
        }
        else
        {
            // collect input, update
        }

        position_.x += cos(rotation_) * speed_ * msecs;
        position_.y += sin(rotation_) * speed_ * msecs;

        if (position_.x < 0)
            position_.x = 1024;
        else if (position_.x > 1024)
            position_.x = 0;

        if (position_.y > 768)
            position_.y = 768;
    }
}

void Plane::Render()
{
    glColor3f(1, 0, 1);
    glPushMatrix();
    glTranslatef(position_.x, position_.y, 0);
    glBegin(GL_QUADS);
        glVertex2f(-10, -10);
        glVertex2f(-10, 10);
        glVertex2f(10, 10);
        glVertex2f(10, -10);
    glEnd();
    glPopMatrix();
}

void Plane::Reset()
{
    position_.x = GetRandom(0.0f, 1024.0f);
    position_.y = GetRandom(200.0f, 768.0f);
    rotation_ = 0;
    speed_ = START_SPEED;
    stalled_ = false;
    dying_ = false;
    invincible_ = INVINCIBILITY_TIME;
    cooldown_ = 0;
}

Bullet *Plane::Fire()
{
    if (cooldown_ <= 0)
    {
        Bullet *bullet = new Bullet(this);
        cooldown_ = FIRE_RATE;
        return bullet;
    }
    else
        return NULL;
}
