#pragma once

namespace SCore {

	class shaderBuffer
	{
	public:

		unsigned int target, bufferId;
		size_t size;

		shaderBuffer( const std::string &name, unsigned int programId );
		shaderBuffer( const size_t sz );

		void* getContent();

	};

}
