#pragma once


#include "contextConfig.h"
#include "camera.h"
#include "oggPlayer.h"
#include <stdio.h>


namespace SCore {

	class shader;

	class context
	{
	protected:
		FILE *appLogFile;
                camera *Camera;
                //oggPlayer *oggPlay;
        public:
		contextConfig *cfg;

		context() : appLogFile( 0 ), Camera( 0 ), cfg( 0 ) {}
                virtual ~context() {}

		virtual bool create( contextConfig* cfg ) = 0;
		virtual bool update() = 0;
		virtual void destroy() = 0;

		void startCamera();
		void endCamera();
		void renderScreen( shader *s );

		virtual double getTimerSeconds();

	};

}

#ifdef _WINDOWS
#include "context_win32.h"
#else
#include "context_linux.h"
#endif
