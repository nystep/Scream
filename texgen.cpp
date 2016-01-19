#include "texgen.h"


namespace SCore {

    static float random1d( int x )
    {
        int n = x;
        n = (n * (n * n * 75731 + 189221) + 1371312589);
        return (float) (n & 0x7FFFFFFF) * (1.f / 2147483648.f);
    }

    texture2d* texgen::noise8( int sz )
    {
        float *noise = new float [sz*sz*4];

        for (int i=0; i<sz*sz*4; i++)
            noise[i] = (random1d(i) + 1.0f) * 0.5f;

        texture2d* tex = new texture2d( sz, sz, TEX_RGBA_8, filterNearest, wrapRepeat, noise );

        return tex;
    }

}
