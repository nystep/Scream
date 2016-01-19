#pragma once


#include <string>
#include <vector>
#include <algorithm>


namespace SCore
{

	class shaderUniform
	{
	public:

		virtual void bind( unsigned int programID ) = 0;

	};

	template <typename T> class shaderUniformConstant : public shaderUniform
	{
	protected:
		friend class shaderUniformSet;

		std::string name;
		std::vector<T> values;
		bool updated;

	public:

		shaderUniformConstant( const std::string &n, const unsigned int nVal, const T *initialVal )
			: name( n ), values(), updated( initialVal != NULL )
		{
			values.resize( nVal );
			if ( initialVal != NULL )
				std::copy( initialVal, initialVal+nVal, values.begin() );
		}

		virtual ~shaderUniformConstant() {}

		virtual void bind( unsigned int programID );

	};

}
