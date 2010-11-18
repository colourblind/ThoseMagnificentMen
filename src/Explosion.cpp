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
            debris_[i].Velocity *= 0.95;
            debris_[i].Velocity.y *= -1;
        }

        debris_[i].Velocity *= 0.99;
        debris_[i].Velocity.y -= GRAVITY * 3 * msecs;
        debris_[i].Position += debris_[i].Velocity * msecs;
        debris_[i].Rotation += debris_[i].RotationSpeed * msecs;
    }

    return lifetime_ < 0;
}

void Explosion::Render()
{
    for (int i = 0; i < NUM_SHRAPNEL; i ++)
    {
        float halfSize = debris_[i].Size / 2;
        glColor3f(1, 1, 0);
        glPushMatrix();
        glTranslatef(debris_[i].Position.x, debris_[i].Position.y, 0);
        glRotatef(180 * debris_[i].Rotation / PI, 0, 0, 1);
        glBegin(GL_QUADS);
            glVertex2f(-halfSize, -halfSize);
            glVertex2f(-halfSize, halfSize);
            glVertex2f(halfSize, halfSize);
            glVertex2f(halfSize, -halfSize);
        glEnd();
        glPopMatrix();
    }
}
