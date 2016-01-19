#pragma once


#include <math.h>
#include <glm/glm.hpp>

//#define max(a,b) ((a)>(b)?(a):(b))
//#define min(a,b) ((a)<(b)?(a):(b))

namespace SCore
{

	// Get the first power of 2 superior to i
	inline int getPOTHigher( int i )
	{
		int j=1;
		while ( (1l<<j) < i ) j++;
		return j;
	}

	// Is i a power of 2?
	inline bool isPOT( int i )
	{
		return (i & (i-1)) == 0;
	}

	inline unsigned short floatToHalf (float f)
	{
		union convertf2i {
			unsigned int i;
			float f;
		} f2i;
		f2i.f = f;
		return ((f2i.i>>16)&0x8000) | ((((f2i.i&0x7f800000)-0x38000000)>>13)&0x7c00) | ((f2i.i>>13)&0x03ff);
	}


	typedef glm::vec2 vec2;
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::mat3 mat3;
	typedef glm::mat4 mat4;
	typedef mat4 matrix;


	class aabb3d
	{
	public:

		vec3 bmin, bmax;

		aabb3d() : bmin( vec3(0.0f,0.0f,0.0f) ), bmax( vec3(0.0f,0.0f,0.0f) ) {}
		aabb3d( vec3 vmin, vec3 vmax ) : bmin( vmin ), bmax( vmax ) {}

		aabb3d( const aabb3d& a ) : bmin( a.bmin ), bmax( a.bmax ) {}

		aabb3d& operator= ( const aabb3d & a )
		{
			bmin = a.bmin;
			bmax = a.bmax;
			return *this;
		}

		void scale( float s );
		void scaleForRotation();

	};

}

