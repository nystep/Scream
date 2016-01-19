#pragma once

namespace SCore {

	class textureCubeMap : public texture, public textureUtils
	{
		void load( const char *filename, const unsigned int target );

		void convertToWp( float quality );

	public:

		textureCubeMap( const char *filenamePX,
						const char *filenameNX,
						const char *filenamePY,
						const char *filenameNY,
						const char *filenamePZ,
						const char *filenameNZ );

		textureCubeMap( const char *filenameSingleRepeatedFace );

		textureCubeMap( const unsigned int sizeX,
						const unsigned int sizeY,
						const textureInternalFormat fm,
						const bool mipmaps=false );

		~textureCubeMap();

	};

}
