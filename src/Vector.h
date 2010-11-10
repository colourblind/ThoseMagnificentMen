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

        //inline Vector2 Normalise() const
        //{
        //    Vector2 v(*this);
        //    float scale = v.Length();
        //    v.x /= scale;
        //    x.y /= scale;
        //    return v;
        //}

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
    };
}

#endif // THOSEMAGNIFICENTMEN_VECTOR_H
