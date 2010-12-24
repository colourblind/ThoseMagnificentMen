#include "Explosion.h"

#include "OpenGL.h"
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
    
    for (int i = 0; i < NUM_SHRAPNEL; i ++)
    {
        if (debris_[i].Position.y < GROUND_LEVEL)
        {
            debris_[i].Velocity *= 0.95f;
            debris_[i].Velocity.y *= -1;
            debris_[i].Position.y += GROUND_LEVEL - debris_[i].Position.y;
        }

        debris_[i].Velocity *= 0.99f;
        debris_[i].Velocity.y -= GRAVITY * 3 * msecs;
        debris_[i].Position += debris_[i].Velocity * msecs;
        debris_[i].Rotation += debris_[i].RotationSpeed * msecs;
    }

    return lifetime_ < 0;
}

void Explosion::Render()
{
    SpriteBlock sprite = SPRITES[SPRITE_EXPLOSION];
    for (int i = 0; i < NUM_SHRAPNEL; i ++)
    {
        float halfSize = debris_[i].Size / 2;
        glColor3f(1, 1, 1);
        glPushMatrix();
        glTranslatef(debris_[i].Position.x, debris_[i].Position.y, 0);
        glRotatef(180 * debris_[i].Rotation / PI, 0, 0, 1);
        glBegin(GL_QUADS);
            glTexCoord2f(sprite.x / 128, (sprite.y + sprite.height) / 128);
            glVertex2f(-halfSize, -halfSize);
            glTexCoord2f(sprite.x / 128, sprite.y);
            glVertex2f(-halfSize, halfSize);
            glTexCoord2f((sprite.x + sprite.width) / 128, sprite.y);
            glVertex2f(halfSize, halfSize);
            glTexCoord2f((sprite.x + sprite.height) / 128, (sprite.y + sprite.height) / 128);
            glVertex2f(halfSize, -halfSize);
        glEnd();
        glPopMatrix();
    }
}

Smoke::Smoke(Plane *victim) :  lifetime_(1500), 
                               position_(victim->GetPosition() + Vector2(GetRandom(-8.0f, 8.0f), GetRandom(-8.0f, 8.0f))),
                               rotation_(GetRandom(0.0f, 2.0f * PI)),
                               size_(GetRandom(6.0f, 14.0f))
{

}

bool Smoke::Update(float msecs)
{
    lifetime_ -= msecs;
    return lifetime_ < 0;
}

void Smoke::Render()
{
    SpriteBlock sprite = SPRITES[SPRITE_SMOKE];
    float halfSize = size_ / 2;
    glColor4f(1, 1, 1, lifetime_ / 1500);
    glPushMatrix();
    glTranslatef(position_.x, position_.y, 0);
    glRotatef(180 * rotation_ / PI, 0, 0, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(sprite.x / 128, (sprite.y + sprite.height) / 128);
        glVertex2f(-halfSize, -halfSize);
        glTexCoord2f(sprite.x / 128, sprite.y / 128);
        glVertex2f(-halfSize, halfSize);
        glTexCoord2f((sprite.x + sprite.width) / 128, sprite.y / 128);
        glVertex2f(halfSize, halfSize);
        glTexCoord2f((sprite.x + sprite.height) / 128, (sprite.y + sprite.height) / 128);
        glVertex2f(halfSize, -halfSize);
    glEnd();
    glPopMatrix();
}

