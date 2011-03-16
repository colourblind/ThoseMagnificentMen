#include "Plane.h"

#include "Maths.h"
#include "OpenGL.h"
#include "Bullet.h"
#include "Loaders.h"

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

    // Normalise rotation
    if (rotation_ > PI)
        rotation_ -= PI * 2;
    else if (rotation_ < -PI)
        rotation_ += PI * 2;

    if (stalled_)
    {
        if (speed_ < MAX_SPEED)
            speed_ += GRAVITY * 2 * msecs;

        position_.y -= speed_ * msecs;
    }
    else
    {
        if (speed_ < STALL_SPEED)
        {
            speed_ = 0;
            stalled_ = true;
        }

        if (dying_)
        {
            if (rotation_ > -PI / 2 && rotation_ < PI / 2)
                rotation_ -= TURN_SPEED * msecs;
            else
                rotation_ += TURN_SPEED * msecs;
            timeToSmoke_ -= msecs;
        }

        // Speed up on dive, slow down on climb
        if (rotation_ > (0.25 * PI) && rotation_ < (0.75 * PI))
            speed_ -= GRAVITY * msecs * 0.25f; // Diving is easier than climbing
        else if (rotation_ < (-0.25 * PI) && rotation_ > (-0.75 * PI))
            speed_ += GRAVITY * msecs;

        // Clamp speed
        if (speed_ > MAX_SPEED)
            speed_ = MAX_SPEED;

        position_.x += cos(rotation_) * speed_ * msecs;
        position_.y += sin(rotation_) * speed_ * msecs;

        // Wrap at the screen edges
        if (position_.x < 0)
            position_.x = WIDTH;
        else if (position_.x > WIDTH)
            position_.x = 0;
        if (position_.y > HEIGHT)
            position_.y = HEIGHT;
    }
}

void Plane::Render()
{
    SpriteBlock sprite = SPRITES[playerNumber_ - 1];
    glColor4f(1, 1, 1, invincible_ <= 0 ? 1.0f : 0.5f);
    glPushMatrix();
    glTranslatef(position_.x, position_.y, 0);
    glRotatef(180 * rotation_ / PI, 0, 0, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(sprite.x / 128, (sprite.y + sprite.height) / 128);
        glVertex2f(-10, -10);
        glTexCoord2f(sprite.x / 128, sprite.y / 128);
        glVertex2f(-10, 10);
        glTexCoord2f((sprite.x + sprite.width) / 128, sprite.y / 128);
        glVertex2f(10, 10);
        glTexCoord2f((sprite.x + sprite.height) / 128, (sprite.y + sprite.height) / 128);
        glVertex2f(10, -10);
    glEnd();
    glPopMatrix();
}

void Plane::Reset()
{
    position_.x = GetRandom(0.0f, static_cast<float>(WIDTH));
    position_.y = GetRandom(200.0f, static_cast<float>(HEIGHT));
    rotation_ = 0;
    speed_ = START_SPEED;
    stalled_ = false;
    dying_ = false;
    invincible_ = INVINCIBILITY_TIME;
    cooldown_ = 0;
    timeToSmoke_ = 0;
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

bool Plane::ReleaseSmoke()
{
    if (timeToSmoke_ < 100)
    {
        timeToSmoke_ += 100;
        return true;
    }
    else
        return false;
}
