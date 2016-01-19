#include "shader.h"


#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

#define MAX_SHADER_LOG_LENGTH 8192

namespace SCore {

	shaderStringBuffer shader::vshStrB = shaderStringBuffer( GL_VERTEX_SHADER );
	shaderStringBuffer shader::fshStrB = shaderStringBuffer( GL_FRAGMENT_SHADER );
	shaderStringBuffer shader::gshStrB = shaderStringBuffer( GL_GEOMETRY_SHADER );
	shaderStringBuffer shader::cmpStrB = shaderStringBuffer( GL_COMPUTE_SHADER );

	shader::~shader()
	{
		if (programID) glDeleteProgram( programID );
		programID = 0;
	}

	const char* shader::getTargetExt( unsigned int target )
	{
		switch (target)
		{
		case GL_VERTEX_SHADER:
			return ".vert";
			break;
		case GL_GEOMETRY_SHADER:
			return ".geom";
			break;
		case GL_FRAGMENT_SHADER:
			return ".frag";
			break;
		case GL_COMPUTE_SHADER:
			return ".comp";
			break;
		default:
			return "";
			break;
		}
	}

	shaderStringBuffer& shader::getShaderStringBuffer( unsigned int target )
	{
		switch (target)
		{
		case GL_VERTEX_SHADER:
			return vshStrB;
		case GL_GEOMETRY_SHADER:
			return gshStrB;
		case GL_FRAGMENT_SHADER:
			return fshStrB;
		case GL_COMPUTE_SHADER:
			return cmpStrB;
		default:
			return vshStrB;
		}
	}

	std::string shader::getSingleName( std::vector<const char*> &names, const char* extbase )
	{
		std::string sname;

		for (unsigned int i=0; i<names.size(); i++)
		{
			int j=0;

			while (j<15 && names[i][j] != 0 && names[i][j] != '.')
			{
				char elem[2];

				elem[0] = names[i][j];
				elem[1] = 0;

				sname += std::string( elem );

				j++;
			}

			if (i<names.size()-1) sname += '_';
			else
			{
				sname += std::string( extbase );
				sname += std::string( ".log.txt" );
			}
		}

		return sname;
	}

	unsigned int shader::loadShader( std::vector<const char*> &names,
		shaderStringBuffer &shStrBuffer, unsigned int target )
	{
		unsigned int shTotalLen = 0;
		unsigned int shNum = (unsigned int) names.size();

		// gather source code lambs from buffer
		unsigned int *shaderLengths = new unsigned int [shNum];
		unsigned int *shaderSourceIndices = new unsigned int [shNum];

		for (unsigned int i=0; i<shNum; i++)
		{
			shStrBuffer.getSourceLamb( names[i], shaderSourceIndices[i], shaderLengths[i] );
			shTotalLen += shaderLengths[i];
		}

		// no source code for this shader.
		if (shTotalLen == 0) return 0;

		// shader source code blabling
		const char** shaderSources = new const char* [shNum];
		const char* strBase = shStrBuffer.getSourcePtr();

		for (unsigned int i=0; i<shNum; i++)
			shaderSources[i] = strBase + shaderSourceIndices[i];

		// shader creation and compilation
		unsigned int shaderID = glCreateShader( target );

		glShaderSource( shaderID, shNum, shaderSources, (const int*) shaderLengths );
		glCompileShader( shaderID );

		// error handling
		int compiled;
		glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compiled );

		if (compiled == GL_FALSE)
		{
			std::string logname = getSingleName( names, getTargetExt(target) );

			char log[MAX_SHADER_LOG_LENGTH];
			int loglength;
			glGetShaderInfoLog( shaderID, MAX_SHADER_LOG_LENGTH-1, &loglength, log );
			log[loglength] = '\0';

			//FILE *errLog;
			//fopen_s( &errLog, logname.c_str(), "w" );
			FILE *errLog = fopen( logname.c_str(), "w" );

			for (unsigned int i=0; i<names.size(); i++)
				for (unsigned int j=0; j<shaderLengths[i]; j++)
					fputc( shaderSources[i][j], errLog );

			fprintf( errLog, "+-----------------------------------------------------------+\n" );
			fprintf( errLog, "%s\n", log );
			fprintf( errLog, "+----------------------------------------------------------+\n" );

			for (unsigned int i=0; i<names.size(); i++)
				fprintf( errLog, "+%s start %u length %u\n", names[i], shaderSourceIndices[i], shaderLengths[i] );

			fprintf( errLog, "+----------------------------------------------------------+\n" );

			fclose( errLog );

			//printf( "# Shader Compilation Errors were detected in %s\n", logname );

			glDeleteShader( shaderID );
			shaderID = 0;
		}

		delete [] shaderSourceIndices;
		delete [] shaderLengths;
		delete [] shaderSources;

		return shaderID;
	}

	void shader::gatherShaderNames( std::vector<const char*> &names,
		const std::vector<shaderInfo> &shadersnfo, unsigned int target )
	{
		names.clear();
		for (size_t i=0; i<shadersnfo.size(); i++)
		{
			if (shadersnfo[i].shaderType == target)
				names.push_back( shadersnfo[i].shaderName.c_str() );
		}
	}

	int shader::load( const shaderInfo &shadernfo )
	{
		std::vector<const char*> shnames;

		shnames.push_back( shadernfo.shaderName.c_str() );

		shaderStringBuffer &shStrBuffer = getShaderStringBuffer( shadernfo.shaderType );
		unsigned int shId = loadShader( shnames, shStrBuffer, shadernfo.shaderType );

		if (shId != 0)
		{
			programID = glCreateProgram();
			glAttachShader( programID, shId );
			glLinkProgram( programID );
			glValidateProgram( programID );

			// check for errors and report them
			int linkStatus, validateStatus;
			glGetProgramiv( programID, GL_LINK_STATUS, &linkStatus );
			glGetProgramiv( programID, GL_VALIDATE_STATUS, &validateStatus );

			if ((linkStatus==GL_FALSE) || (validateStatus==GL_FALSE))
			{
				std::string logname = getSingleName( shnames, ".link" );

				FILE *errLog;
				char log[MAX_SHADER_LOG_LENGTH];
				int loglength;
				glGetProgramInfoLog( programID, MAX_SHADER_LOG_LENGTH-1, &loglength, log );
				log[loglength] = '\0';

				//fopen_s( &errLog, logname.c_str(), "w" );
				errLog = fopen( logname.c_str(), "w" );

				fprintf( errLog, "+----------------------------------------------------------+\n" );
				fprintf( errLog, "%s\n", log );
				fprintf( errLog, "+----------------------------------------------------------+\n" );

				fclose( errLog );

				glDeleteProgram( programID );
				programID = 0;
				return ERROR_PROGRAM_NOT_LINKED;
			}

			return SHADER_NO_ERRORS;
		}
		else return ERROR_SHADER_NOT_COMPILED;
	}

	int shader::load( const std::vector<shaderInfo> &shadersnfo )
	{
		std::vector<const char*> shaderNames;

		gatherShaderNames( shaderNames, shadersnfo, GL_VERTEX_SHADER );
		unsigned int vertexShId = loadShader( shaderNames, vshStrB, GL_VERTEX_SHADER );

		gatherShaderNames( shaderNames, shadersnfo, GL_GEOMETRY_SHADER );
		unsigned int geometryShId = loadShader( shaderNames, gshStrB, GL_GEOMETRY_SHADER );

		gatherShaderNames( shaderNames, shadersnfo, GL_COMPUTE_SHADER );
		unsigned int computeShId = loadShader( shaderNames, cmpStrB, GL_COMPUTE_SHADER );

		gatherShaderNames( shaderNames, shadersnfo, GL_FRAGMENT_SHADER );
		unsigned int fragmentShId = loadShader( shaderNames, fshStrB, GL_FRAGMENT_SHADER );

		if (vertexShId || geometryShId || fragmentShId || computeShId)
		{
			programID = glCreateProgram();

			if (vertexShId)
				glAttachShader( programID, vertexShId );

			if (geometryShId)
				glAttachShader( programID, geometryShId );

			if (fragmentShId)
				glAttachShader( programID, fragmentShId );

			if (computeShId)
				glAttachShader( programID, computeShId );

			glLinkProgram( programID );
			glValidateProgram( programID );

			// check for errors and report them
			int linkStatus, validateStatus;
			glGetProgramiv( programID, GL_LINK_STATUS, &linkStatus );
			glGetProgramiv( programID, GL_VALIDATE_STATUS, &validateStatus );

			if ((linkStatus==GL_FALSE) || (validateStatus==GL_FALSE))
			{
				std::string logname = getSingleName( shaderNames, ".link" );

				FILE *errLog;
				char log[MAX_SHADER_LOG_LENGTH];
				int loglength;
				glGetProgramInfoLog( programID, MAX_SHADER_LOG_LENGTH-1, &loglength, log );
				log[loglength] = '\0';

				//fopen_s( &errLog, logname.c_str(), "w" );
				errLog = fopen( logname.c_str(), "w" );

				fprintf( errLog, "+----------------------------------------------------------+\n" );
				fprintf( errLog, "%s\n", log );
				fprintf( errLog, "+----------------------------------------------------------+\n" );

				fclose( errLog );

				glDeleteProgram( programID );
				programID = 0;
				return ERROR_PROGRAM_NOT_LINKED;
			}

			return SHADER_NO_ERRORS;
		}
		else return ERROR_SHADER_NOT_COMPILED;
	}

	void shader::computeShader( unsigned int nThreadsX, unsigned int nThreadsY, unsigned int nThreadsZ,
		shaderTextureSet *textures, shaderUniformSet *uniforms, shaderBufferSet *buffers )
	{
		glUseProgram( programID );

		if (textures) textures->setTextures( this );
		if (uniforms) uniforms->setUniforms( this );
		if (buffers) buffers->setBuffers( this );

		glDispatchCompute( nThreadsX, nThreadsY, nThreadsZ );

		if (buffers) buffers->unSetBuffers();
		if (textures) textures->unSetTextures();

		glMemoryBarrier( GL_ALL_BARRIER_BITS );

		glUseProgram( 0 );
	}

}
