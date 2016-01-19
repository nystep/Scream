#pragma once


namespace SCore {

	class textureBuffer : public texture
	{
	public:

		textureBuffer( meshVBO *vbo, textureInternalFormat tif );
		~textureBuffer();

	};

}

