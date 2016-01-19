
#include "screamCore.h"
//#include <windows.h>


namespace SCore {

    camera::camera() {
        rotx = roty = -0.1;
        pos = vec3(0.0f, 10.0f, -100.0f);
        dt = -1.0;
        lastx = cx = 0.0;
        lasty = cy = 0.0;
    }

    camera::camera(float centerx, float centery) {
        rotx = roty = -0.1;
        pos = vec3(0.0f, 10.0f, -100.0f);
        dt = -1.0;
        lastx = cx = centerx;
        lasty = cy = centery;
    }

    camera::camera(const camera& c) {
        rotx = c.rotx;
        roty = c.roty;
        pos = c.pos;
        dt = c.dt;
        lastx = c.lastx;
        cx = c.cx;
        lasty = c.lasty;
        cy = c.cy;
    }

    camera& camera::operator=(const camera& c) {
        rotx = c.rotx;
        roty = c.roty;
        pos = c.pos;
        dt = c.dt;
        lastx = c.lastx;
        cx = c.cx;
        lasty = c.lasty;
        cy = c.cy;

        return *this;
    }

    camera::~camera() {
    }

    void camera::update(double t, float rightt, float downt, float mouseX, float mouseY) {
        static double lastt = -1.0;
        const double mouseSensivity = 5.0;

        if (lastt >= 0.0) {
            float mmatrix[16];

            double deltat = t - lastt;
            if (deltat < 0.001) return;
            dt = deltat;

            float dx = mouseX - lastx;
            float dy = mouseY - lasty;

            rotx += dx * mouseSensivity*dt;
            roty += dy * mouseSensivity*dt;

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glRotatef((float) roty, 1.0f, 0.0f, 0.0f);
            glRotatef((float) rotx, 0.0f, 1.0f, 0.0f);
            glGetFloatv(GL_MODELVIEW_MATRIX, mmatrix);
            vec3 right = -vec3(mmatrix[0], mmatrix[4], mmatrix[8]) * rightt;
            vec3 down = -vec3(mmatrix[2], mmatrix[6], mmatrix[10]) * downt;
            pos = pos + right;
            pos = pos + down;
            glPopMatrix();
        }

        lastx = mouseX;
        lasty = mouseY;
        lastt = t;
    }

    void camera::start() {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPerspective(100.f, 1.33333f, 0.1f, 1000.f);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glRotatef((float) roty, 1.0f, 0.f, 0.f);
        glRotatef((float) rotx, 0.0f, 1.f, 0.f);
        glTranslatef(pos[0], pos[1], pos[2]);
    }

    void camera::end() {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

};
