
#include "texture.h"

namespace SCore {

	textureCubeMap::textureCubeMap( const char *filenamePX,
		const char *filenameNX,
		const char *filenamePY,
		const char *filenameNY,
		const char *filenamePZ,
		const char *filenameNZ )
	{
		target = GL_TEXTURE_CUBE_MAP_ARB;
		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		sz = 0;
		load( filenamePX, GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB );
		load( filenameNX, GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB );
		load( filenamePY, GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB );
		load( filenameNY, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB );
		load( filenamePZ, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB );
		load( filenameNZ, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB );

		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexParameteri( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );

		glBindTexture( target, 0 );
		assert( glGetError() == GL_NO_ERROR );
	}


	textureCubeMap::textureCubeMap( const char *filenameSingleRepeatedFace )
	{
		target = GL_TEXTURE_CUBE_MAP_ARB;
		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		sz = 0;
		load( filenameSingleRepeatedFace, GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB );
		load( filenameSingleRepeatedFace, GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB );
		load( filenameSingleRepeatedFace, GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB );
		load( filenameSingleRepeatedFace, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB );
		load( filenameSingleRepeatedFace, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB );
		load( filenameSingleRepeatedFace, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB );

		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexParameteri( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );

		glBindTexture( target, 0 );
		assert( glGetError() == GL_NO_ERROR );
	}

	void textureCubeMap::load( const char *filename, const unsigned int target )
	{
		std::string imgFilename( filename );
		bool hasAlphaChannel = false;

		if ( !hasWpExtension(filename) )
		{
			convertImageToWp( filename, &hasAlphaChannel, 90.0f );
			imgFilename = getWpFilename( filename );
		}

		upload2dImageWp( target, imgFilename.c_str() );

		bytesPerChannel = 1;
		nChannels = hasAlphaChannel ? 4 : 3;

		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*) &internalFormat );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_WIDTH, (GLint*) &sx );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_HEIGHT, (GLint*) &sy );
		sz++;

		assert( glGetError() == GL_NO_ERROR );
	}

	textureCubeMap::textureCubeMap( const unsigned int sizeX,
		const unsigned int sizeY,
		const textureInternalFormat fm,
		const bool mipmaps )
	{
		unsigned int internalFmt, externalFmt, pixelTransfertFmt;

		internalFmt = getGlInternalFormat( fm );
		externalFmt = getGlExternalFormat( fm );
		pixelTransfertFmt = getGlExternalPixelTransfertType( fm );
		bytesPerChannel = getFormatBytesPerChannel( fm );
		nChannels = getFormatNumberOfChannels( fm );
		sx = sizeX;
		sy = sizeY;
		sz = 6;
		mipmapping = mipmaps ? 1 : 0;

		internalFormat = internalFmt;
		target = GL_TEXTURE_CUBE_MAP_ARB;
		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, internalFmt, sizeX, sizeY, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, internalFmt, sizeX, sizeY, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, internalFmt, sizeX, sizeY, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, internalFmt, sizeX, sizeY, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, internalFmt, sizeX, sizeY, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, internalFmt, sizeX, sizeY, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexParameteri( target, GL_GENERATE_MIPMAP, mipmaps ? GL_TRUE : GL_FALSE );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexParameteri( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );

		glBindTexture( target, 0 );
		assert( glGetError() == GL_NO_ERROR );
	}

	textureCubeMap::~textureCubeMap()
	{
		assert( glGetError() == GL_NO_ERROR );
		glDeleteTextures( 1, &textureId );
		assert( glGetError() == GL_NO_ERROR );
	}

};
