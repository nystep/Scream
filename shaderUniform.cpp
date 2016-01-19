
#include "shaderUniform.h"
#include "smath.h"
#include "stype.h"
#define GLEW_STATIC
#include <GL/glew.h>


namespace SCore
{

	template <> void shaderUniformConstant<int>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniform1iv( location, values.size(), &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<unsigned int>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniform1iv( location, values.size(), (int*) &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<uint64_t>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniformui64vNV( location, values.size(), &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<float>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniform1fv( location, values.size(), &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<vec2>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniform2fv( location, values.size(), (float*) &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<vec3>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniform3fv( location, values.size(), (float*) &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<vec4>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniform4fv( location, values.size(), (float*) &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<mat3>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniformMatrix3fv( location, values.size(), false, (float*) &values[0] );
			updated = false;
		}
	}

	template <> void shaderUniformConstant<mat4>::bind( unsigned int programID )
	{
		if (updated)
		{
			int location = glGetUniformLocation( programID, name.c_str() );
			if (location >= 0) glUniformMatrix4fv( location, values.size(), false, (float*) &values[0] );
			updated = false;
		}
	}

}
