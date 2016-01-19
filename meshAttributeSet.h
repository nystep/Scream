#pragma once


#include <vector>
#include <string>


namespace SCore {

	class meshVBO;

	typedef enum {
		vaFloat,
		vaUByte,
		vaHalf
	} vertexAttributeType;

	class meshAttributeSet
	{
	protected:

		class vertexAttribute
		{
		protected:

			friend class meshAttributeSet;

			std::string attribName;
			unsigned int type, nelements, normalized;
                        uint64_t stride, size, offset, totalSz;

		public:

			vertexAttribute( std::string& attributeName, unsigned int ty, unsigned int nelts,
				bool norm, uint64_t strde, uint64_t sz,
				uint64_t off, uint64_t tSz ) :
			attribName( attributeName ), type( ty ), nelements( nelts ),
				normalized( norm ? GL_TRUE : GL_FALSE ),
				stride( strde ), size( sz ), offset( off ), totalSz( tSz )
			{}

		};

		unsigned int currentVBOOffset;
		std::vector<vertexAttribute> vertexAttribs;

		unsigned int vertexAttributeTypeToGL( vertexAttributeType vat );
		unsigned int vertexAttributeTypeSz( vertexAttributeType vat );

	public:

		meshAttributeSet() : currentVBOOffset(0) {}
		~meshAttributeSet() {}

		int addVertexAttribute( std::string attributeName, vertexAttributeType vaType, unsigned int numberOfComponents,
			unsigned int size, bool normalized=false );
		void updateVertexAttribute( int vaIndex, meshVBO* vbo, unsigned int sz, void *data );

		void setAttributes( shader *s );
		void unsetAttributes( shader *s );

	};

}
