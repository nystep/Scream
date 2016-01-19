#include "keysinput.h"
#include <windows.h>


namespace SCore {
	namespace win32 {

	    static unsigned int numberApplicationKeys=7;

        keyStates::keyStates()
        {
            lastUpdateT = 0.0f;
            accumulator = 0;
            reader = 1;
            for (int i=0; i<256; i++)
                keystates[0][i] = keystates[1][i] = 0.0f;
        }

		void keyStates::update( float t )
		{
			float dt = t-lastUpdateT;
			for (unsigned int i=0; i<numberApplicationKeys; i++)
			{
                int vk = getVirtualKey( applicationKey(i) );
                keystates[accumulator][vk] += GetAsyncKeyState(vk) ? dt : 0.0f;
			}
			lastUpdateT = t;
		}

		void keyStates::reset( float t )
		{
		    accumulator = 1-accumulator;
		    reader = 1-reader;
			for (unsigned int i=0; i<numberApplicationKeys; i++)
			{
			    int vk = getVirtualKey( applicationKey(i) );
			    keystates[accumulator][vk] = 0.0f;
			}
			lastUpdateT = t;
		}

		int keyStates::getVirtualKey( applicationKey k )
		{
			switch (k)
			{
			case KEY_UP:
				return VK_UP;
				break;
			case KEY_DOWN:
				return VK_DOWN;
				break;
			case KEY_LEFT:
				return VK_LEFT;
				break;
			case KEY_RIGHT:
				return VK_RIGHT;
				break;
			case KEY_LMOUSE:
				return VK_LBUTTON;
				break;
			case KEY_RMOUSE:
				return VK_RBUTTON;
				break;
			case KEY_SPACE:
				return VK_SPACE;
				break;
			default:
				return 0;
				break;
			}
		}

		float keyStates::getKey( applicationKey ak )
		{
			int k = getVirtualKey( ak );
			if (k>0 && k<256)
				return keystates[reader][k];
			else
				return 0.0f;
		}

	};

};
