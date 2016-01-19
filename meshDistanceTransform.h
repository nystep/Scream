#pragma once

namespace SCore {

    class meshDistanceTransform
    {
        static shader *shDualDepth;
        static shaderTextureSet *texDualCSM;
        static shaderUniformSet *uniDualCSM;
        static shader *shDualCSM;

    public:

        static int texTemporaryRes;
        static texture2d *texTemporaryDualHM;
        static texture2d *texTemporaryDualCSM;

    public:

        aabb3d meshBoundingBox;

        meshDistanceTransform() : meshBoundingBox() {}

        int load( int resolution, nsfScene *m, int meshNumber );

    };

}
