#include "context.h"
#include "shader.h"
//#include <wglew.h>
#include <IL/il.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



namespace SCore {

    context_linux::context_linux()
    {
        save_to_video = false;
    }

    context_linux::~context_linux()
    {
        if (save_to_video) finishVideoOutput();
    }

    bool context_linux::create( contextConfig* cfg )
    {
        XVisualInfo *vi;
        Colormap cmap;
        XSetWindowAttributes swa;
        GLXContext cx;
        GLboolean doubleBuffer = GL_TRUE;
        int dummy;
        
        this->cfg = cfg;
        
        static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};
        static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

        dpy = XOpenDisplay(NULL);
        if (dpy == NULL)
            printf("could not open display");

        if (!glXQueryExtension(dpy, &dummy, &dummy))
            printf("X server has no OpenGL GLX extension");

        /* find an OpenGL-capable RGB visual with depth buffer */
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
        if (vi == NULL) {
            vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
            if (vi == NULL) printf("no RGB visual with depth buffer");
            doubleBuffer = GL_FALSE;
        }

        /* create an OpenGL rendering context */
        cx = glXCreateContext(dpy, vi, /* no shared displaylists */ None, GL_TRUE);
        if (cx == NULL)
            printf("could not create rendering context");

        /* create an X colormap since probably not using default visual */
        cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.border_pixel = 0;
        swa.event_mask = KeyPressMask | ExposureMask
                | ButtonPressMask | StructureNotifyMask;
        win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                cfg->sx, cfg->sy, 0, vi->depth, InputOutput, vi->visual,
                CWBorderPixel | CWColormap | CWEventMask, &swa);
        /*
        XSetStandardProperties(dpy, win, "main", "main", None,
                argv, argc, NULL);*/

        glXMakeCurrent(dpy, win, cx);

        XMapWindow(dpy, win);

        glEnable(GL_DEPTH_TEST); /* enable depth test */
        glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
        glClearDepth(1.0);       /* pedantic, 1.0 is the default */

        /* frame buffer clears should be to black */
        glClearColor(0.0, 0.0, 0.0, 0.0);

        /* set up projection transform */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
        /* establish initial viewport */
        /* pedantic, full window size is default viewport */
        glViewport(0, 0, cfg->sx, cfg->sy);
        
        if (save_to_video) startVideoOutput();

        return true;
    }

    bool context_linux::update()
    {
        XEvent event;

        do {
            XNextEvent(dpy, &event);
            switch (event.type) {
                case KeyPress:
                {
                    KeySym keysym;
                    XKeyEvent *kevent;
                    char buffer[1];
                    /* It is necessary to convert the keycode to a
                     * keysym before checking if it is an escape */
                    kevent = (XKeyEvent *) & event;
                    if ((XLookupString((XKeyEvent *) & event, buffer, 1, &keysym, NULL) == 1)
                            && (keysym == (KeySym) XK_Escape))
                        exit(0);
                    break;
                }
                case ButtonPress:
                    XButtonEvent *buttonevt;
                    buttonevt = (XButtonEvent *) & event;
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width,
                            event.xconfigure.height);
                    /* fall through... */
                case Expose:
                    break;
                default:
                    break;
            }
        } while (XPending(dpy)); /* loop to compress events */

        // update animation state
        // spawn new layers
        // go through layers and cleanup

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        
        if (save_to_video) appendVideoFrame();
        glXSwapBuffers(dpy, win);

        return true;
    }
    
    void context_linux::startVideoOutput()
    {
        
    }
    
    void context_linux::appendVideoFrame()
    {
        
    }
    
    void context_linux::finishVideoOutput()
    {
        
    }

    void context_linux::destroy()
    {
    }

    double context_linux::getTimerSeconds()
    {
        return 0.0;
    }

};
