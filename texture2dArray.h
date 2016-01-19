#pragma once

namespace SCore {

	class texture2dArray : public texture
	{
	public:

		unsigned int nUsedLayers;
		unsigned int currentLayer;

		texture2dArray( const char *imageFileNamesFilter );
		texture2dArray( unsigned int sx, unsigned int sy, unsigned int nb, textureInternalFormat tif, float* pels );
		~texture2dArray();

		void copyLayerFromFramebuffer( int layer );

	};

}
