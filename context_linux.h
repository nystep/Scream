#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/keysym.h>

namespace SCore {

	class context_linux : public context
	{
        protected:
            
            Display   *dpy;
            Window     win;
            
            bool save_to_video;
            void startVideoOutput();
            void appendVideoFrame();
            void finishVideoOutput();

	public:
            context_linux();
            virtual ~context_linux();

            bool create( contextConfig* cfg );
            bool update();
            void destroy();
            double getTimerSeconds();
	};

}
