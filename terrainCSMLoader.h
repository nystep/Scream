#pragma once


namespace SCore {

	typedef enum {
		CLO_HEIGHTMAP,
		CLO_HEIGHTANGLETANMAP,
		CLO_HEIGHTANGLEMAP,
		CLO_CSMMAP
	} ENUM_CSMLOADOPTIONS;

    texture2d* terrainHMLoad( const char* heightmapfilename );
	texture2d* terrainCSMLoad( const char* csmfilename, const ENUM_CSMLOADOPTIONS opt );

};
