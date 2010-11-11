#include "Bullet.h"

#include "Constants.h"
#include "OpenGL.h"

using namespace ThoseMagnificentMen;

Bullet::Bullet(Plane *owner) : owner_(owner)
{
    position_ = owner_->GetPosition();
    // Push the bullet outside of the firing plane in the direction it is facing
    position_.x += cos(owner_->GetRotation()) * 20;
    position_.y += sin(owner_->GetRotation()) * 20;

    velocity_ = owner_->GetVelocity().Normalise() * BULLET_SPEED;
}

bool Bullet::Update(float msecs)
{
    position_ += velocity_ * msecs;

    bool isOffscreen = position_.x < 0 || position_.x > 1024 || position_.y < GROUND_LEVEL || position_.y > 768;
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
