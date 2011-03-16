#include "Bullet.h"

#include "Constants.h"
#include "OpenGL.h"

using namespace ThoseMagnificentMen;

Bullet::Bullet(Plane *owner) : owner_(owner)
{
    position_ = owner_->GetPosition();
    // Push the bullet outside of the firing plane in the direction it is facing
    position_ += owner_->GetVelocity().Normalise() * (20 + MAX_SPEED + 0.5f);

    velocity_ = owner_->GetVelocity().Normalise() * BULLET_SPEED;
}

bool Bullet::Update(float msecs)
{
    position_ += velocity_ * msecs;

    bool isOffscreen = position_.x < 0 || position_.x > WIDTH || position_.y < GROUND_LEVEL || position_.y > HEIGHT;
    return isOffscreen;
}

void Bullet::Render()
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(position_.x, position_.y, 0);
    glBegin(GL_QUADS);
        glVertex2f(-1, -1);
        glVertex2f(-1, 1);
        glVertex2f(1, 1);
        glVertex2f(1, -1);
    glEnd();
    glPopMatrix();
}
