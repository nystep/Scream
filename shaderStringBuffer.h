
#pragma once

#include <vector>
#include <string>

namespace SCore {

	// TODO:
	// make a proper use of std::string as it might be a bit more revealant than
	// a std::vector<char> here.
	class shaderStringBuffer
	{
	protected:

		class shaderSourceFileLamb {
		public:
			unsigned int start, length;
			std::string sourceFileName;

			shaderSourceFileLamb( const std::string &name ) : start(0), length(0), sourceFileName( name ) {}
		};

		std::vector<shaderSourceFileLamb> src;
		std::vector<char> str;
		unsigned int type;

		shaderSourceFileLamb* searchSourceLamb( const std::string &name );
		shaderSourceFileLamb* loadSourceLamb( const std::string &name );

	public:

		shaderStringBuffer( unsigned int shtype ) : src(), str(), type(shtype) {}
		~shaderStringBuffer() {}

		const char* getSourcePtr() { return (const char*) &str[0]; }
		void getSourceLamb( const char* srcname, unsigned int &outSrc, unsigned int &outLen );

	};

}
