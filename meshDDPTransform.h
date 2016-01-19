#pragma once

namespace SCore {

    class meshDDPTransform
    {
    protected:

        static shader *shDualDepth;
        static shaderTextureSet *texDualDepthPeel;
        static shader *shClearTextureSet;
        static shaderTextureSet *texClearTextureSet;

        void generateMaximumMipmaps( texture2d *tex1, texture2d *tex2 );

    public:

        static int texTemporaryRes;
        static texture2d *texTemporaryDualHMP;
        static texture2d *texTemporaryDualHMN;
        static texture2d *texTemporaryDualHMP_u;
        static texture2d *texTemporaryDualHMN_u;
        static texture2d *texTemporaryDualHMP_v;
        static texture2d *texTemporaryDualHMN_v;

    public:

        aabb3d meshBoundingBox;

        meshDDPTransform() : meshBoundingBox() {}

        int load( int resolution, nsfScene *m, int meshNumber );

    };

}
