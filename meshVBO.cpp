#include <assert.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include "meshVBO.h"
#include <memory.h>
#include <malloc.h>

namespace SCore {

	unsigned int meshVBO::vboEnumToType( vboType type )
	{
		switch (type)
		{
		case vboVertex:
			return GL_ARRAY_BUFFER_ARB;
		case vboIndex:
			return GL_ELEMENT_ARRAY_BUFFER_ARB;
		case vboTextureBuffer:
			return GL_TEXTURE_BUFFER_ARB;
		default:
			return 0;
		}
	}

	unsigned int meshVBO::vboAccessToGl( vboAccess acc )
	{
		switch (acc)
		{
		case vboRead:
			return GL_READ_ONLY;
		case vboWrite:
			return GL_WRITE_ONLY;
		case vboReadWrite:
			return GL_READ_WRITE;
		default:
			return 0;
		}
	}

	void meshVBO::load( vboType t, unsigned int sz, void* data )
	{
		type = vboEnumToType( t );
		size = sz;

		glGenBuffersARB( 1, &vboID );
		glBindBufferARB( type, vboID );
		glBufferDataARB( type, sz, data, GL_STATIC_DRAW );
		glBindBufferARB( type, 0 );

		assert( glGetError() == GL_NO_ERROR );
	}

	void meshVBO::update( unsigned int start, unsigned int sz, void* data )
	{
		glBindBufferARB( type, vboID );
		glBufferSubDataARB( type, start, sz, data );
		glBindBufferARB( type, 0 );

		assert( glGetError() == GL_NO_ERROR );
	}

	void* meshVBO::getContent()
	{
		void *memblock = malloc( size );
		if (!memblock) return NULL;
		glBindBufferARB( type, vboID );
		void *buffer = glMapBufferARB( type, GL_READ_ONLY_ARB );
		memcpy( memblock, buffer, size );
		
		if ((glUnmapBufferARB( type ) == GL_FALSE) | (glGetError() != GL_NO_ERROR))
		{
			glBindBufferARB( type, 0 );
			free( memblock );
			return NULL;
		}

		glBindBufferARB( type, 0 );
		return memblock;
	}

	uint64_t meshVBO::getBufferAddress( vboAccess acc )
	{
		uint64_t address = 0;

		glMakeNamedBufferResidentNV( vboID, vboAccessToGl( acc ) );
		glGetNamedBufferParameterui64vNV( vboID, GL_BUFFER_GPU_ADDRESS_NV, &address );

		return address;
	}

	meshVBO::~meshVBO()
	{
		glDeleteBuffersARB( 1, &vboID );
	}

};
