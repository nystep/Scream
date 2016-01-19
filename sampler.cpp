#include "sampler.h"
#define GLEW_STATIC
#include <GL/glew.h>

//#include <cml/cml.h>


namespace SCore {

	int sampler::convertFilteringMinToGl( samplerFiltering sf )
	{
		switch (sf)
		{
		case filterNearest:
			return GL_NEAREST;
			break;
		case filterLinear:
			return GL_LINEAR;
			break;
		case filterMipmaps:
		case filterAnisotropic:
			return GL_LINEAR_MIPMAP_LINEAR;
			break;
		default:
			return 0;
			break;
		};
	}

	int sampler::convertFilteringMagToGl( samplerFiltering sf )
	{
		switch (sf)
		{
		case filterNearest:
			return GL_NEAREST;
			break;
		case filterLinear:
		case filterMipmaps:
		case filterAnisotropic:
			return GL_LINEAR;
			break;
		default:
			return 0;
			break;
		};
	}

	int sampler::convertWrappingToGl( samplerWrapMode wm )
	{
		switch (wm)
		{
		case wrapClamp:
			return GL_CLAMP;
			break;
		case wrapRepeat:
			return GL_REPEAT;
			break;
		case wrapMirroredRepeat:
			return GL_MIRRORED_REPEAT;
			break;
		default:
			return 0;
			break;
		};
	}

	int sampler::create( samplerFiltering sf, samplerWrapMode wm, int anisotropy )
	{
		int minificationFilter = convertFilteringMinToGl( sf );
		int magnificationFilter = convertFilteringMagToGl( sf );
		int wrappingMode = convertWrappingToGl( wm );

		int maxAnisotropy = 16;
		//glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISTROPY, &maxAnisotropy );
		int aniso = anisotropy > maxAnisotropy ? maxAnisotropy : anisotropy;
		if (aniso<1) aniso=1;

		glGenSamplers( 1, &samp );
		glSamplerParameteri( samp, GL_TEXTURE_WRAP_S, wrappingMode );
		glSamplerParameteri( samp, GL_TEXTURE_WRAP_T, wrappingMode );
		glSamplerParameteri( samp, GL_TEXTURE_WRAP_R, wrappingMode );
		glSamplerParameteri( samp, GL_TEXTURE_MIN_FILTER, minificationFilter );
		glSamplerParameteri( samp, GL_TEXTURE_MAG_FILTER, magnificationFilter );
		glSamplerParameteri( samp, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso );

		return glGetError() == GL_NO_ERROR;
	}

};
