#include "shader.h"


namespace SCore {

	shaderUniformIndice shaderUniformSet::searchUniform( const std::string &name, shaderUniformType type )
	{
		shaderUniformIndice indice = SHADER_UNIFORM_INVALID_INDICE;

		switch (type)
		{
		case UNIFORM_INT:
			indice = searchUniformVector<int>( name, uniformsINT );
			break;
		case UNIFORM_UINT:
			indice = searchUniformVector<unsigned int>( name, uniformsUINT );
			break;
		case UNIFORM_FLOAT:
			indice = searchUniformVector<float>( name, uniformsFLOAT );
			break;
		case UNIFORM_VEC2:
			indice = searchUniformVector<vec2>( name, uniformsVEC2 );
			break;
		case UNIFORM_VEC3:
			indice = searchUniformVector<vec3>( name, uniformsVEC3 );
			break;
		case UNIFORM_VEC4:
			indice = searchUniformVector<vec4>( name, uniformsVEC4 );
			break;
		case UNIFORM_MAT3:
			indice = searchUniformVector<mat3>( name, uniformsMAT3 );
			break;
		case UNIFORM_MAT4:
			indice = searchUniformVector<mat4>( name, uniformsMAT4 );
			break;
		default:
			break;
		}

		return indice;
	}

	shaderUniformIndice shaderUniformSet::add( shaderUniformType type, const std::string &name, void *value, unsigned int nValues )
	{
		shaderUniformIndice indice;

		switch (type)
		{
		case UNIFORM_INT:
			indice = addUniformVector<int>( name, (int*) value, nValues, uniformsINT );
			break;
		case UNIFORM_UINT:
			indice = addUniformVector<unsigned int>( name, (unsigned int*) value, nValues, uniformsUINT );
			break;
		case UNIFORM_UINT64:
			indice = addUniformVector<uint64_t>( name, (uint64_t*) value, nValues, uniformsUINT64 );
			break;
		case UNIFORM_FLOAT:
			indice = addUniformVector<float>( name, (float*) value, nValues, uniformsFLOAT );
			break;
		case UNIFORM_VEC2:
			indice = addUniformVector<vec2>( name, (vec2*) value, nValues, uniformsVEC2 );
			break;
		case UNIFORM_VEC3:
			indice = addUniformVector<vec3>( name, (vec3*) value, nValues, uniformsVEC3 );
			break;
		case UNIFORM_VEC4:
			indice = addUniformVector<vec4>( name, (vec4*) value, nValues, uniformsVEC4 );
			break;
		case UNIFORM_MAT3:
			indice = addUniformVector<mat3>( name, (mat3*) value, nValues, uniformsMAT3 );
			break;
		case UNIFORM_MAT4:
			indice = addUniformVector<mat4>( name, (mat4*) value, nValues, uniformsMAT4 );
			break;
		case UNIFORM_PTR:
			indice = addUniformVector<uint64_t>( name, (uint64_t*) value, nValues, uniformsPTR );
			break;
		default:
			indice = SHADER_UNIFORM_INVALID_INDICE;
			break;
		}

		return indice;
	}

	shaderUniformIndice shaderUniformSet::addInt( const std::string &name, int v )
	{
		return addUniformVector<int>( name, &v, 1, uniformsINT );
	}

	shaderUniformIndice shaderUniformSet::addUInt( const std::string &name, unsigned int v )
	{
		return addUniformVector<unsigned int>( name, &v, 1, uniformsUINT );
	}

	shaderUniformIndice shaderUniformSet::addUInt64( const std::string &name, uint64_t u )
	{
		return addUniformVector<uint64_t>( name, &u, 1, uniformsUINT64 );
	}

	shaderUniformIndice shaderUniformSet::addFloat( const std::string &name, float v )
	{
		return addUniformVector<float>( name, &v, 1, uniformsFLOAT );
	}

	shaderUniformIndice shaderUniformSet::addVec2( const std::string &name, vec2 v )
	{
		return addUniformVector<vec2>( name, &v, 1, uniformsVEC2 );
	}

	shaderUniformIndice shaderUniformSet::addVec3( const std::string &name, vec3 v )
	{
		return addUniformVector<vec3>( name, &v, 1, uniformsVEC3 );
	}

	shaderUniformIndice shaderUniformSet::addVec4( const std::string &name, vec4 v )
	{
		return addUniformVector<vec4>( name, &v, 1, uniformsVEC4 );
	}

	shaderUniformIndice shaderUniformSet::addMat3( const std::string &name, mat3 v )
	{
		return addUniformVector<mat3>( name, &v, 1, uniformsMAT3 );
	}

	shaderUniformIndice shaderUniformSet::addMat4( const std::string &name, mat4 v )
	{
		return addUniformVector<mat4>( name, &v, 1, uniformsMAT4 );
	}

	shaderUniformIndice shaderUniformSet::addPtr( const std::string &name, meshVBO* v, vboAccess access )
	{
		uint64_t address = v->getBufferAddress( access );
		return addUniformVector<uint64_t>( name, &address, 1, uniformsPTR );
	}

	void shaderUniformSet::update( shaderUniformType type, shaderUniformIndice indice, void* value, unsigned int nValues )
	{
		switch (type)
		{
		case UNIFORM_INT:
			updateUniformVector<int>( indice, (int*) value, nValues, uniformsINT );
			break;
		case UNIFORM_UINT:
			updateUniformVector<unsigned int>( indice, (unsigned int*) value, nValues, uniformsUINT );
			break;
		case UNIFORM_UINT64:
			updateUniformVector<uint64_t>( indice, (uint64_t*) value, nValues, uniformsUINT64 );
			break;
		case UNIFORM_FLOAT:
			updateUniformVector<float>( indice, (float*) value, nValues, uniformsFLOAT );
			break;
		case UNIFORM_VEC2:
			updateUniformVector<vec2>( indice, (vec2*) value, nValues, uniformsVEC2 );
			break;
		case UNIFORM_VEC3:
			updateUniformVector<vec3>( indice, (vec3*) value, nValues, uniformsVEC3 );
			break;
		case UNIFORM_VEC4:
			updateUniformVector<vec4>( indice, (vec4*) value, nValues, uniformsVEC4 );
			break;
		case UNIFORM_MAT3:
			updateUniformVector<mat3>( indice, (mat3*) value, nValues, uniformsMAT3 );
			break;
		case UNIFORM_MAT4:
			updateUniformVector<mat4>( indice, (mat4*) value, nValues, uniformsMAT4 );
			break;
		case UNIFORM_PTR:
			updateUniformVector<uint64_t>( indice, (uint64_t*) value, nValues, uniformsPTR );
			break;
		default:
			break;
		}
	}

	void shaderUniformSet::updateInt( shaderUniformIndice indice, int* value, unsigned int nValues )
	{
		updateUniformVector<int>( indice, value, nValues, uniformsINT );
	}

	void shaderUniformSet::updateUInt( shaderUniformIndice indice, unsigned int* value, unsigned int nValues )
	{
		updateUniformVector<unsigned int>( indice, value, nValues, uniformsUINT );
	}

	void shaderUniformSet::updateUInt64( shaderUniformIndice indice, uint64_t* value, unsigned int nValues )
	{
		updateUniformVector<uint64_t>( indice, value, nValues, uniformsUINT64 );
	}

	void shaderUniformSet::updateFloat( shaderUniformIndice indice, float* value, unsigned int nValues )
	{
		updateUniformVector<float>( indice, value, nValues, uniformsFLOAT );
	}

	void shaderUniformSet::updateVec2( shaderUniformIndice indice, vec2* value, unsigned int nValues )
	{
		updateUniformVector<vec2>( indice, value, nValues, uniformsVEC2 );
	}

	void shaderUniformSet::updateVec3( shaderUniformIndice indice, vec3* value, unsigned int nValues )
	{
		updateUniformVector<vec3>( indice, value, nValues, uniformsVEC3 );
	}

	void shaderUniformSet::updateVec4( shaderUniformIndice indice, vec4* value, unsigned int nValues )
	{
		updateUniformVector<vec4>( indice, value, nValues, uniformsVEC4 );
	}

	void shaderUniformSet::updateMat3( shaderUniformIndice indice, mat3* value, unsigned int nValues )
	{
		updateUniformVector<mat3>( indice, value, nValues, uniformsMAT3 );
	}

	void shaderUniformSet::updateMat4( shaderUniformIndice indice, mat4* value, unsigned int nValues )
	{
		updateUniformVector<mat4>( indice, value, nValues, uniformsMAT4 );
	}

	void shaderUniformSet::updatePtr( shaderUniformIndice indice, meshVBO* v, vboAccess access, unsigned int nValues )
	{
		uint64_t address = v->getBufferAddress( access );
		updateUniformVector<uint64_t>( indice, &address, 1, uniformsPTR );
	}

	void shaderUniformSet::setUniforms( shader *s )
	{
		if (s == NULL) return;

		glUseProgram( s->programID );

		for (size_t i=0; i<uniformsINT.size(); i++) uniformsINT[i].bind( s->programID );
		for (size_t i=0; i<uniformsUINT.size(); i++) uniformsUINT[i].bind( s->programID );
		for (size_t i=0; i<uniformsUINT64.size(); i++) uniformsUINT64[i].bind( s->programID );
		for (size_t i=0; i<uniformsFLOAT.size(); i++) uniformsFLOAT[i].bind( s->programID );
		for (size_t i=0; i<uniformsVEC2.size(); i++) uniformsVEC2[i].bind( s->programID );
		for (size_t i=0; i<uniformsVEC3.size(); i++) uniformsVEC3[i].bind( s->programID );
		for (size_t i=0; i<uniformsVEC4.size(); i++) uniformsVEC4[i].bind( s->programID );
		for (size_t i=0; i<uniformsMAT3.size(); i++) uniformsMAT3[i].bind( s->programID );
		for (size_t i=0; i<uniformsMAT4.size(); i++) uniformsMAT4[i].bind( s->programID );
		for (size_t i=0; i<uniformsPTR.size(); i++) uniformsPTR[i].bind( s->programID );
	}

};
