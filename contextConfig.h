#pragma once

namespace SCore {

	class contextConfig
	{
	public:

		unsigned int sx, sy;
		unsigned int multisample, anisotropy;
		bool useConsole, fullScreen;
		unsigned int majorVer, minorVer;
		bool useCoreProfile, debugContext;
		const char *logFileName;
		const char *windowTitle;
		const char *audioName;

		contextConfig( bool fs = false,
			bool cons = true,
			unsigned int width=1024,
			unsigned int height=768,
			unsigned int ms=4,
			unsigned int ani=0 ) :
		sx( width ), sy( height ), multisample( ms ), anisotropy( ani ),
			useConsole( cons ), fullScreen( fs ), majorVer( 3 ), minorVer( 3 ),
			useCoreProfile( false ), debugContext( false ),
			logFileName( "log.txt" ), windowTitle( "Scream engine v0.5" ),
			audioName( 0 ) {}

	};

}
