#include "shaderStringBuffer.h"
#include <stdio.h>
#include <string>
#include <assert.h>


namespace SCore {

	void shaderStringBuffer::getSourceLamb( const char* srcname, unsigned int &outSrc, unsigned int &outLen )
	{
		shaderSourceFileLamb* lamb = NULL;

		lamb = searchSourceLamb( srcname );

		if (lamb == NULL)
		{
			lamb = loadSourceLamb( srcname );

			if (lamb == NULL)
			{
				printf( "# Shader Source Code Part Not Found: %s\n", srcname );
				outSrc = 0;
				outLen = 0;
				return;
			}
		}

		assert( lamb != NULL );

		outSrc = lamb->start;
		outLen = lamb->length;
	}

	shaderStringBuffer::shaderSourceFileLamb* shaderStringBuffer::searchSourceLamb( const std::string &name )
	{
		shaderSourceFileLamb* result = NULL;

		for ( std::vector<shaderSourceFileLamb>::iterator i = src.begin(); i != src.end(); ++i )
		{
			if (i->sourceFileName == name)
			{
				result = &(*i);
				break;
			}
		}

		return result;
	}

	shaderStringBuffer::shaderSourceFileLamb* shaderStringBuffer::loadSourceLamb( const std::string &name )
	{
		FILE* sourceFile;
		sourceFile = fopen( name.c_str(), "rb" );

		if (sourceFile == NULL)
			return NULL;

		fseek( sourceFile, 0, SEEK_END );
		size_t sz = ftell( sourceFile );
		fseek( sourceFile, 0, SEEK_SET );

		if (sz >= (1<<18))
		{
			fclose( sourceFile );
			return NULL;
		}

		shaderSourceFileLamb s( name );
		s.start = str.size();

		size_t i=0;
		while (i<sz && !feof(sourceFile))
		{
			str.push_back( fgetc( sourceFile ) );
			i++;
		}

		s.length = str.size() - s.start;
		src.push_back( s );

		fclose( sourceFile );

		return &src.back();
	}

};
