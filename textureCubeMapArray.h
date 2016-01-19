#pragma once

namespace SCore {

	class textureCubeMapArray : public texture
	{
    public:

        textureCubeMapArray( const unsigned int sizeX,
                            const unsigned int sizeY,
                            const unsigned int nb,
                            const textureInternalFormat fm );
		~textureCubeMapArray();

	};

}
