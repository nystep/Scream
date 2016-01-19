#pragma once

#include "stype.h"

namespace SCore {

	typedef enum {
		vboVertex,
		vboIndex,
		vboTextureBuffer
	} vboType;

	typedef enum {
		vboRead,
		vboWrite,
		vboReadWrite
	} vboAccess;

	// TODO: vboTextureBuffer type VBOs aren't working properly atm.
	class meshVBO
	{
		unsigned int vboEnumToType( vboType type );
		unsigned int vboAccessToGl( vboAccess acc );

	public:

		unsigned int vboID, type, size;
		uint64_t gpuPtr;

		meshVBO() : vboID(0), type(0), size(0), gpuPtr(0l) {}
		~meshVBO();

		// data may be NULL.
		void load( vboType t, unsigned int sz, void* data );

		void update( unsigned int start, unsigned int sz, void* data );

		// returns a central memory block containing the contents of the vbo allocated by malloc.
		void* getContent();

		uint64_t getBufferAddress( vboAccess acc );

		// bind and unbind for primitive referencing
		inline void bind()
		{
			glBindBufferARB( type, vboID );
		}

		inline void unbind()
		{
			glBindBufferARB( type, 0 );
		}

	};

}
