#pragma once

namespace SCore
{

	class textureVideo : public texture, public textureUtils
	{
	public:

		textureVideo( const char *filename, textureFiltering sf=filterLinear );
		~textureVideo();

	};

}
