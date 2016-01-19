#pragma once

namespace SCore {

    // TODO: rewrite completelly.

	typedef enum {
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_LMOUSE,
		KEY_RMOUSE,
		KEY_SPACE
	} applicationKey;

	//static unsigned int numberApplicationKeys = 7;

	namespace win32 {

	    extern int mouseX, mouseY;

		class keyStates {
		private:
            int accumulator, reader;
			float keystates[2][256];
			float lastUpdateT;
			int getVirtualKey( applicationKey k );

		public:
            keyStates();
			void update( float t );
			void reset( float t );
			float getKey( applicationKey k );
		};

	}
}
