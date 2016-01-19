#pragma once

#include "shaderBuffer.h"
#include <vector>
#include <string>

namespace SCore {

	class shaderBufferSet
	{
	protected:

		class shaderBufferBinding
		{
		public:

			shaderBuffer *buffer;
			std::string name;
			int binding;

			shaderBufferBinding( shaderBuffer *b, std::string &n ) : buffer(b), name(n), binding(-1) {}

		};

		std::vector<shaderBufferBinding> buffers;

	public:

		shaderBufferSet() : buffers() {}

		void addBuffer( shaderBuffer *b, std::string &n );

		void setBuffers( shader *s );
		void unSetBuffers();

	};

}
