#pragma once

namespace SCore {

	class texture3d : public texture
	{
	public:

		texture3d( unsigned int sx, unsigned int sy, unsigned int sz, textureInternalFormat tif, float* pels );
		~texture3d();

	};

}
