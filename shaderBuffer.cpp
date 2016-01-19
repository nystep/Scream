#include "shader.h"
#include "shaderBuffer.h"
#include <memory.h>

namespace SCore {

	shaderBuffer::shaderBuffer( const std::string &name, unsigned int programId )
	{
		target = GL_SHADER_STORAGE_BUFFER;
		size = 0;

		unsigned int idx = glGetProgramResourceIndex( programId, GL_BUFFER_VARIABLE, name.c_str() );

		int props[] = { GL_BUFFER_DATA_SIZE };
		int paramLen = 4;
		int params;

		glGetProgramResourceiv( programId, GL_BUFFER_VARIABLE, idx, 1, (const GLenum *) props, 4, &paramLen, &params );

		glGenBuffersARB( 1, &bufferId );
		glBindBufferARB( target, bufferId );
		glBufferDataARB( target, params, 0, GL_DYNAMIC_COPY );
		glBindBufferARB( target, 0 );

		assert( glGetError() == GL_NO_ERROR );
	}

	shaderBuffer::shaderBuffer( const size_t sz )
	{
		target = GL_SHADER_STORAGE_BUFFER;
		size = sz;

		glGenBuffersARB( 1, &bufferId );
		glBindBufferARB( target, bufferId );
		glBufferDataARB( target, sz, 0, GL_DYNAMIC_COPY );
		glBindBufferARB( target, 0 );

		assert( glGetError() == GL_NO_ERROR );
	}

	void* shaderBuffer::getContent()
	{
		void* result = 0;

		glBindBufferARB( target, bufferId );
		void* source = glMapBufferARB( target, GL_READ_ONLY_ARB );

		if (source)
		{
			result = malloc( size );

			if (result) memcpy( result, source, size );
			else result = 0;

			glUnmapBufferARB( target );
		}

		glBindBufferARB( target, 0 );

		return result;
	}

}
