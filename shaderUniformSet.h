#pragma once


#include "smath.h"
#include "stype.h"
#include "shaderUniform.h"
#include "meshVBO.h"
#include <string>


#define SHADER_UNIFORM_INVALID_INDICE 0xFFFFFFFFFFFFFFFFL
#define SHADER_UNIFORM_INDICE_MASK    0x000000000FFFFFFFL


namespace SCore
{

	typedef uint64_t shaderUniformIndice;


	typedef enum
	{
		UNIFORM_INT,
		UNIFORM_UINT,
		UNIFORM_UINT64,
		UNIFORM_FLOAT,
		UNIFORM_VEC2,
		UNIFORM_VEC3,
		UNIFORM_VEC4,
		UNIFORM_MAT3,
		UNIFORM_MAT4,
		UNIFORM_PTR
	}
	shaderUniformType;


	class shaderUniformSet
	{
	protected:

		shaderUniformIndice searchUniform( const std::string &name, shaderUniformType type );

		/// search/add/update templates
		template <typename T>
		shaderUniformIndice searchUniformVector( const std::string &name,
			const std::vector< shaderUniformConstant<T> > &uniforms )
		{
			shaderUniformIndice indice = SHADER_UNIFORM_INVALID_INDICE;
			for (size_t i=0; i<uniforms.size(); i++)
				if (name == uniforms[i].name) { indice=i; break; }
			return indice;
		}

		template <typename T>
		shaderUniformIndice addUniformVector( const std::string &name,
			const T *value, size_t nValues,
			std::vector< shaderUniformConstant<T> > &uniforms )
		{
			if (nValues == 0) return SHADER_UNIFORM_INVALID_INDICE;
			shaderUniformIndice indice = searchUniformVector<T>( name, uniforms );
			unsigned int localInd = (unsigned int) (indice & SHADER_UNIFORM_INDICE_MASK);
			if (indice == SHADER_UNIFORM_INVALID_INDICE)
			{
				indice = uniforms.size();
				uniforms.push_back( shaderUniformConstant<T>( name, nValues, value ) );
			}
			else if ((unsigned int) uniforms[localInd].values.size() != nValues)
				indice = SHADER_UNIFORM_INVALID_INDICE;
			return indice;
		}

		template <typename T>
		void updateUniformVector( shaderUniformIndice indice,
			T* value, unsigned int nValues,
			std::vector< shaderUniformConstant<T> > &uniforms )
		{
			unsigned int localInd = (unsigned int) (indice & SHADER_UNIFORM_INDICE_MASK);
			if (value == NULL) return;
			if (localInd >= (unsigned int) uniforms.size()) return;
			if ((unsigned int) uniforms[localInd].values.size() != nValues) return;
			std::copy( value, value+nValues, uniforms[localInd].values.begin() );
			uniforms[localInd].updated = true;
		}

		/// arrays of uniforms sorted by type
		std::vector< shaderUniformConstant<int> > uniformsINT;
		std::vector< shaderUniformConstant<unsigned int> > uniformsUINT;
		std::vector< shaderUniformConstant<uint64_t> > uniformsUINT64;
		std::vector< shaderUniformConstant<float> > uniformsFLOAT;
		std::vector< shaderUniformConstant<vec2> > uniformsVEC2;
		std::vector< shaderUniformConstant<vec3> > uniformsVEC3;
		std::vector< shaderUniformConstant<vec4> > uniformsVEC4;
		std::vector< shaderUniformConstant<mat3> > uniformsMAT3;
		std::vector< shaderUniformConstant<mat4> > uniformsMAT4;
		std::vector< shaderUniformConstant<uint64_t> > uniformsPTR;

		/// tree structure (namespaces/uniform shadowing)
		shaderUniformSet *parent;

	public:

		shaderUniformSet( shaderUniformSet *par=0 ) : uniformsINT(), uniformsUINT(), uniformsUINT64(),
			uniformsFLOAT(), uniformsVEC2(), uniformsVEC3(), uniformsVEC4(),
			uniformsMAT3(), uniformsMAT4(), uniformsPTR(), parent(par) {}
		~shaderUniformSet() {}

		/// returns a handle to the uniform,
		/// SHADER_UNIFORM_INVALID_INDICE is returned if there is a type or array size mismatch.
		/// if the identifier was already there with same type and array size, it returns the handle to it.
		shaderUniformIndice add( shaderUniformType type, const std::string &name, void *value, unsigned int nValues=1 );

		shaderUniformIndice addInt( const std::string &name, int i );
		shaderUniformIndice addUInt( const std::string &name, unsigned int u );
		shaderUniformIndice addUInt64( const std::string &name, uint64_t u );
		shaderUniformIndice addFloat( const std::string &name, float f );
		shaderUniformIndice addVec2( const std::string &name, vec2 v );
		shaderUniformIndice addVec3( const std::string &name, vec3 v );
		shaderUniformIndice addVec4( const std::string &name, vec4 v );
		shaderUniformIndice addMat3( const std::string &name, mat3 m );
		shaderUniformIndice addMat4( const std::string &name, mat4 m );
		shaderUniformIndice addPtr( const std::string &name, meshVBO* v, vboAccess access );

		void update( shaderUniformType type, shaderUniformIndice indice, void* value, unsigned int nValues=1 );

		void updateInt( shaderUniformIndice indice, int* value, unsigned int nValues=1 );
		void updateUInt( shaderUniformIndice indice, unsigned int* value, unsigned int nValues=1 );
		void updateUInt64( shaderUniformIndice indice, uint64_t* value, unsigned int nValues=1 );
		void updateFloat( shaderUniformIndice indice, float* value, unsigned int nValues=1 );
		void updateVec2( shaderUniformIndice indice, vec2* value, unsigned int nValues=1 );
		void updateVec3( shaderUniformIndice indice, vec3* value, unsigned int nValues=1 );
		void updateVec4( shaderUniformIndice indice, vec4* value, unsigned int nValues=1 );
		void updateMat3( shaderUniformIndice indice, mat3* value, unsigned int nValues=1 );
		void updateMat4( shaderUniformIndice indice, mat4* value, unsigned int nValues=1 );
		void updatePtr( shaderUniformIndice indice, meshVBO* value, vboAccess access, unsigned int nValues=1 );

		/// call to bind identifiers in this scope to the shader s
		void setUniforms( shader *s );

	};

}
