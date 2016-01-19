#pragma once


#include "singleton.h"


namespace SCore
{

	class oggPlayer
	{
	protected:

		void *stream;

	public:

		oggPlayer() : stream(0) {}
		~oggPlayer();

		bool decodeDiskStream( const char *filename );
		bool decodeMemoryStream( void* data, unsigned int sz );
		void startPlaying();
		float getPlayedTimeSeconds();

	};

}
