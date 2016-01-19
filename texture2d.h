#pragma once

namespace SCore {

	class texture2d : public texture, public textureUtils
	{
	private:

		bool mipmapsGenerated;

	public:

		texture2d( const char *filename, textureFiltering sf=filterLinear, textureWrapMode wm=wrapRepeat, int anisotropy=1 );
		texture2d( unsigned int sx, unsigned int sy, textureInternalFormat tif, textureFiltering sf=filterLinear, textureWrapMode wm=wrapClamp, void* pels=0 );
		~texture2d();

		float *getTextureContent();
		void generateMipmapsMax();

	};

}
