#pragma once


namespace SCore {

	typedef enum {
		filterNearest,
		filterLinear,
		filterMipmaps,
		filterAnisotropic
	} samplerFiltering;

	typedef enum {
		wrapClamp,
		wrapRepeat,
		wrapMirroredRepeat
	} samplerWrapMode;


	class sampler
	{
		int convertFilteringMinToGl( samplerFiltering sf );
		int convertFilteringMagToGl( samplerFiltering sf );
		int convertWrappingToGl( samplerWrapMode wm );

	public:
		unsigned int samp;

		int create( samplerFiltering sf=filterLinear, samplerWrapMode wm=wrapRepeat, int anisotropy=1 );
	};

};

