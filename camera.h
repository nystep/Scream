#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "smath.h"

namespace SCore 
{

    class camera {
    private:

        double dt;
        double rotx, roty;
        vec3 pos;
        float lastx, lasty, cx, cy;

    public:

        camera();
        camera(float centerx, float centery);
        camera(const camera& c);
        camera& operator= (const camera& c);
        ~camera();

        void update(double t, float rightt, float downt, float mouseX, float mouseY);
        void start();
        void end();

    };

}

#endif // CAMERA_H_INCLUDED
