#pragma once


#include <vector>
#include <string>

#include "texture.h"
#include "shaderStringBuffer.h"


namespace SCore {

#define SHADER_NO_ERRORS 0
#define ERROR_SHADER_NOT_COMPILED -1
#define ERROR_PROGRAM_NOT_LINKED -2

	class shaderInfo
	{
	public:

		std::string shaderName;
		unsigned int shaderType;

		shaderInfo( const char* name, unsigned int type ) : shaderName(name), shaderType(type) {}

	};

	// TODO: rule of 3 constructors
	class shader
	{
	private:

		static shaderStringBuffer vshStrB;
		static shaderStringBuffer fshStrB;
		static shaderStringBuffer gshStrB;
		static shaderStringBuffer cmpStrB;

		const char* getTargetExt( unsigned int target );
		std::string getSingleName( std::vector<const char*> &names, const char* extbase );

		unsigned int loadShader( std::vector<const char*> &names,
			shaderStringBuffer &shStrBuffer,
			unsigned int target );

		void gatherShaderNames( std::vector<const char*> &names,
			const std::vector<shaderInfo> &shadersnfo,
			unsigned int target );

		shaderStringBuffer& getShaderStringBuffer( unsigned int target );

	public:

		unsigned int programID;

		/// constructor, destructor
		shader() : programID(0) {}
		~shader();

		/// loading
		int load( const shaderInfo &shadernfo );
		int load( const std::vector<shaderInfo> &shadersnfo );

		/// computing
		void computeShader( unsigned int nThreadsX, unsigned int nThreadsY, unsigned int nThreadsZ,
			shaderTextureSet *textures, shaderUniformSet *uniforms, shaderBufferSet *buffers );

	};

}

#include "shaderTextureSet.h"
#include "shaderUniformSet.h"
#include "shaderBufferSet.h"
