#ifndef THOSEMAGNIFICENTMEN_MATHS_H
#define THOSEMAGNIFICENTMEN_MATHS_H

#include <stdlib.h>
#include <time.h>

namespace ThoseMagnificentMen
{
    const float PI = 3.142f;

    inline void InitRandom()
    {
        int seed = static_cast<int>(time(NULL));
        srand(seed);
    }

    inline float GetRandom(float min, float max)
    {
        return (static_cast<float>(rand()) / RAND_MAX) * (max - min) + min;
    }
    
    inline int GetRandom(int min, int max)
    {
        return static_cast<int>(GetRandom(static_cast<float>(min), static_cast<float>(max)));
    }
}

#endif // THOSEMAGNIFICENTMEN_MATHS_H
