
#include "texture.h"

namespace SCore {

    textureCubeMapArray::textureCubeMapArray( const unsigned int sizeX,
                                            const unsigned int sizeY,
                                            const unsigned int nb,
                                            const textureInternalFormat fm )
    {
		unsigned int internalFmt, externalFmt, pixelTransfertFmt, defaultFiltering;

		target = GL_TEXTURE_CUBE_MAP_ARRAY_ARB;
		internalFmt = getGlInternalFormat( fm );
		externalFmt = getGlExternalFormat( fm );
		pixelTransfertFmt = getGlExternalPixelTransfertType( fm );
		defaultFiltering = getDefaultFiltering( pixelTransfertFmt );
		internalFormat = internalFmt;

		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		glTexImage3D( target, 0, internalFmt, sizeX, sizeY, nb*6, 0, externalFmt, pixelTransfertFmt, NULL );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, defaultFiltering );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, defaultFiltering );
        glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        glTexParameteri( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );

		glBindTexture( target, 0 );
    }

    textureCubeMapArray::~textureCubeMapArray()
    {
		glDeleteTextures( 1, &textureId );
    }

};
