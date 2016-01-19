#include "shader.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>


namespace SCore {


	unsigned int shaderTextureSet::loadStoreAccessToGl( loadStoreAccess lsa )
	{
		switch (lsa)
		{
		case lsRead:
			return GL_READ_ONLY;
		case lsWrite:
			return GL_WRITE_ONLY;
		case lsReadWrite:
			return GL_READ_WRITE;
		default:
			return 0;
		}
	}

	void shaderTextureSet::add( texture* tex, const std::string& name, int textureUnit )
	{
		textures.push_back( shaderTexture( textureUnit, name, tex ) );
	}

	void shaderTextureSet::addLoadStore( texture* tex, const std::string& name, loadStoreAccess lsa,
		int imageUnit, int layerStart )
	{
		int maxImageUnits;
		glGetIntegerv( GL_MAX_IMAGE_UNITS_EXT, &maxImageUnits );

		if (imageUnit<0 || imageUnit >= maxImageUnits)
		{
			//printf( "# the image unit %s attempts to bind to image unit %d whereas the renderer only provides %d image units.\n", name.c_str(), imageUnit, maxImageUnits );
			return;
		}

		/// the first argument indicates that we don't support any layered rendering yet.
		loadStores.push_back( shaderLoadStore( false, imageUnit, layerStart, loadStoreAccessToGl( lsa ), name, tex ) );
	}

	void shaderTextureSet::setTextures( shader *s )
	{
		const size_t textureSz = textures.size();
		const size_t loadStoreSz = loadStores.size();

		glUseProgram( s->programID );

		for (size_t i=0; i<textureSz; i++)
		{
			int location = glGetUniformLocation( s->programID, textures[i].name.c_str() );
			if (location >= 0)
			{
				glActiveTexture( GL_TEXTURE0+textures[i].textureUnit );
				glBindTexture( textures[i].tex->target, textures[i].tex->textureId );
				glUniform1i( location, textures[i].textureUnit );
			}
		}

		if (loadStoreSz > 0 && GLEW_ARB_shader_image_load_store)
		{

			for (size_t i=0; i<loadStoreSz; i++)
			{
				int location = glGetUniformLocation( s->programID, loadStores[i].name.c_str() );
				if (location >= 0)
				{
					glBindImageTexture( loadStores[i].imageUnit, loadStores[i].tex->textureId, 0,
						loadStores[i].useLayeredRendering, 0,
						loadStores[i].access, loadStores[i].tex->internalFormat );
					glUniform1i( location, loadStores[i].imageUnit );
				}
			}
			assert( glGetError() == GL_NO_ERROR );
		}
	}

	void shaderTextureSet::unSetTextures()
	{
		const size_t textureSz = textures.size();
		const size_t loadStoreSz = loadStores.size();

		for (size_t i=0; i<textureSz; i++)
		{
			glActiveTextureARB( GL_TEXTURE0+textures[i].textureUnit );
			glBindTexture( textures[i].tex->target, 0 );
		}

		if (loadStoreSz > 0 && GLEW_EXT_shader_image_load_store)
		{
			glMemoryBarrier( GL_ALL_BARRIER_BITS );
		}

		assert( glGetError() == GL_NO_ERROR );
	}

};
