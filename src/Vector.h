#ifndef THOSEMAGNIFICENTMEN_VECTOR_H
#define THOSEMAGNIFICENTMEN_VECTOR_H

#include <math.h>

namespace ThoseMagnificentMen
{
    struct Vector2
    {
        float x, y;

        Vector2() : x(0), y(0) { }
        Vector2(float ax, float ay) : x(ax), y(ay) { }

        inline float Length() const 
        {
            return ::sqrt(x * x + y * y);
        }

        inline float Distance(const Vector2 &b) const
        {
            float ax = x - b.x;
            float ay = y - b.y;
            return ::sqrt(ax * ax + ay * ay);
        }

        inline Vector2 Normalise() const
        {
            Vector2 v(*this);
            float scale = v.Length();
            v.x /= scale;
            v.y /= scale;
            return v;
        }

        inline float Dot(const Vector2 &b) const
        {
            return x * b.x + y * b.y;
        }

        inline float Cross(const Vector2 &b) const
        {
            return x * b.y - b.x * y;
        }

        inline Vector2 operator +(const Vector2 &b) const
        {
            Vector2 v(*this);
            v += b;
            return v;
        }

        inline Vector2 operator +=(const Vector2 &b)
        {
            x += b.x;
            y += b.y;
            return *this;
        }

        inline Vector2 operator -(const Vector2 &b) const
        {
            Vector2 v(*this);
            v -= b;
            return v;
        }

        inline Vector2 operator -=(const Vector2 &b)
        {
            x -= b.x;
            y -= b.y;
            return *this;
        }

        inline Vector2 operator *(const float b) const
        {
            Vector2 v(*this);
            v *= b;
            return v;
        }

        inline Vector2 operator *=(const float b)
        {
            x *= b;
            y *= b;
            return *this;
        }

        static Vector2 CollisionPoint(Vector2 aPos, Vector2 aVel, Vector2 bPos, Vector2 bVel)
        {
            Vector2 aPos2 = aPos + aVel;
            Vector2 bPos2 = bPos + bVel;

            // Get both lines in the form ax + by = c
            float a1 = aPos2.y - aPos.y;
            float b1 = aPos.x - aPos2.x;
            float c1 = a1 * aPos.x + b1 * aPos.y;

            float a2 = bPos2.y - bPos.y;
            float b2 = bPos.x - bPos2.x;
            float c2 = a2 * bPos.x + b2 * bPos.y;

            float det = a1 * b2 - a2 * b1;
            if (det == 0) // Parallel lines
                return Vector2();
            else
                return Vector2((b2 * c1 - b1 * c2) / det, (a1 * c2 - a2 * c1) / det);
        }

    };
}

#endif // THOSEMAGNIFICENTMEN_VECTOR_H
