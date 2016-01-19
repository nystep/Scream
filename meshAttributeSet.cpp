#include "screamCore.h"


namespace SCore {

	unsigned int meshAttributeSet::vertexAttributeTypeToGL( vertexAttributeType vat )
	{
		switch (vat)
		{
		case vaFloat: return GL_FLOAT;
		case vaUByte: return GL_UNSIGNED_BYTE;
		case vaHalf: return GL_HALF_FLOAT;
		default: return 0;
		}
	}

	unsigned int meshAttributeSet::vertexAttributeTypeSz( vertexAttributeType vat )
	{
		switch (vat)
		{
		case vaFloat: return 4;
		case vaUByte: return 1;
		case vaHalf: return 2;
		default: return 0;
		}
	}

	int meshAttributeSet::addVertexAttribute( std::string attributeName, vertexAttributeType vaType,
		unsigned int numberOfComponents, unsigned int size, bool normalized )
	{
		vertexAttribute va( attributeName, vertexAttributeTypeToGL( vaType ), numberOfComponents, normalized,
			numberOfComponents * vertexAttributeTypeSz( vaType ), size, currentVBOOffset,
			vertexAttributeTypeSz( vaType ) * numberOfComponents * size );

		currentVBOOffset += va.totalSz;
		vertexAttribs.push_back( va );

		return int(vertexAttribs.size())-1;
	}

	void meshAttributeSet::updateVertexAttribute( int vaIndex, meshVBO* vbo, unsigned int sz, void *data )
	{
		if (vaIndex >= 0 && vaIndex < int(vertexAttribs.size()) && vertexAttribs[vaIndex].totalSz == sz)
		{
			assert( vertexAttribs[vaIndex].offset+sz <= vbo->size );
			vbo->update( vertexAttribs[vaIndex].offset, sz, data );
		}
	}

	void meshAttributeSet::setAttributes( shader *s )
	{
		for (unsigned int i=0; i<unsigned(vertexAttribs.size()); i++)
		{
#if 0
			//if ( vertexAttribs[i].index == 0 )
			if ( false )
			{
				glEnableClientState( GL_VERTEX_ARRAY );
				glVertexPointer( vertexAttribs[i].nelements,
					vertexAttribs[i].type,
					vertexAttribs[i].stride,
					(char*)vertexAttribs[i].offset );
			}
			else
			{
				glEnableVertexAttribArrayARB( vertexAttribs[i].index );
				glVertexAttribPointerARB( vertexAttribs[i].index,
					vertexAttribs[i].nelements,
					vertexAttribs[i].type,
					vertexAttribs[i].normalized,
					vertexAttribs[i].stride,
					(char*)vertexAttribs[i].offset );
			}
#else
			int index = glGetAttribLocationARB( s->programID, vertexAttribs[i].attribName.c_str() );
			if (index != -1)
			{
				glEnableVertexAttribArrayARB( index );
				glVertexAttribPointerARB( index,
					vertexAttribs[i].nelements,
					vertexAttribs[i].type,
					vertexAttribs[i].normalized,
					vertexAttribs[i].stride,
					(char*)vertexAttribs[i].offset );
			}
#endif
		}

		assert( glGetError() == GL_NO_ERROR );
	}

	void meshAttributeSet::unsetAttributes( shader *s )
	{
		for (unsigned int i=0; i<unsigned(vertexAttribs.size()); i++)
		{
			int index = glGetAttribLocationARB( s->programID, vertexAttribs[i].attribName.c_str() );
			if (index != -1)
			{
				glDisableVertexAttribArrayARB( index );
			}
		}

		assert( glGetError() == GL_NO_ERROR );
	}

};
