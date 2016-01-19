#include "shader.h"
#include "shaderBufferSet.h"

namespace SCore {

	void shaderBufferSet::addBuffer( shaderBuffer *b, std::string &n )
	{
		buffers.push_back( shaderBufferBinding( b, n ) );
	}

	void shaderBufferSet::setBuffers( shader *s )
	{
		for (size_t i=0; i<buffers.size(); i++)
		{
			//glBindBuffer( buffers[i].buffer->target, buffers[i].buffer->bufferId );
			unsigned int idx = glGetProgramResourceIndex( s->programID, 
				GL_BUFFER_VARIABLE, buffers[i].name.c_str() );
			glBindBufferBase( buffers[i].buffer->target, idx, buffers[i].buffer->bufferId );
			//glShaderStorageBlockBinding( s->programID, idx, buffers[i].buffer->bufferId );
			assert( glGetError() == GL_NO_ERROR );
		}
	}

	void shaderBufferSet::unSetBuffers()
	{
		for (size_t i=0; i<buffers.size(); i++)
		{
		}
		glMemoryBarrier( GL_ALL_BARRIER_BITS );
		assert( glGetError() == GL_NO_ERROR );
	}

}
