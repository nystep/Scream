#include "texture.h"

namespace SCore {

	texture3d::texture3d( unsigned int sx, unsigned int sy, unsigned int sz,
		textureInternalFormat tif, float* pels )
	{
		unsigned int internalFmt = getGlInternalFormat( tif );
		unsigned int externalFmt = getGlExternalFormat( tif );
		unsigned int pixelTransfertFmt = getGlExternalPixelTransfertType( tif );

		internalFormat = internalFmt;
		target = GL_TEXTURE_3D;
		bytesPerChannel = getFormatBytesPerChannel( tif );
		nChannels = getFormatNumberOfChannels( tif );
		texture3d::sx = sx;
		texture3d::sy = sy;
		texture3d::sz = sz;

		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );
		glTexImage3D( target, 0, internalFmt, sx, sy, sz, 0, externalFmt, pixelTransfertFmt, pels );
		glBindTexture( target, 0 );
	}

	texture3d::~texture3d()
	{
		glDeleteTextures( 1, &textureId );
	}

};
