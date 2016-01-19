
#include "texture.h"
#include <assert.h>


namespace SCore {

	textureBuffer::textureBuffer( meshVBO *vbo, textureInternalFormat tif )
	{
		target = GL_TEXTURE_BUFFER_ARB;
		internalFormat = getGlInternalFormat( tif );
		bytesPerChannel = getFormatBytesPerChannel( tif );
		nChannels = getFormatNumberOfChannels( tif );

		textureBuffer::sx = vbo->size / (bytesPerChannel*nChannels);
		textureBuffer::sy = 1;
		textureBuffer::sz = 1;

		glBindBufferARB( vbo->type, vbo->vboID );
		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );
		glTexBufferARB( target, internalFormat, vbo->vboID );

		assert( glGetError() == GL_NO_ERROR );
	}

	textureBuffer::~textureBuffer()
	{
		glDeleteTextures( 1, &textureId );
	}

};
