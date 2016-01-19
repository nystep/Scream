#pragma once


namespace SCore {

    typedef enum {
        lsRead,
        lsWrite,
        lsReadWrite
    } loadStoreAccess;

	class shaderTextureSet
	{
		class shaderTexture
		{
        protected:
		    friend class shaderTextureSet;

			int textureUnit;
			std::string name;
			texture* tex;
        public:
			shaderTexture( int tu, std::string n, texture *t ) :
                textureUnit( tu ), name( n ), tex( t ) {}
		};

		std::vector<shaderTexture> textures;

		class shaderLoadStore
		{
        protected:
		    friend class shaderTextureSet;

		    bool useLayeredRendering;
			int imageUnit, layerStart;
			unsigned int access;
			std::string name;
			texture* tex;
        public:
			shaderLoadStore( bool useLR, int ui, int ls, unsigned int acc, std::string n, texture* t ) :
                useLayeredRendering( useLR ), imageUnit( ui ), layerStart( ls ), access( acc ), name( n ), tex( t ) {}
		};

		std::vector<shaderLoadStore> loadStores;

		unsigned int loadStoreAccessToGl( loadStoreAccess lsa );

	public:

        shaderTextureSet() : textures(), loadStores() {}
        ~shaderTextureSet() {}

		void add( texture* tex, const std::string& name, int textureUnit=0 );
		void addLoadStore( texture* tex, const std::string& name, loadStoreAccess lsa, int imageUnit=0, int layerStart=0 );

		void setTextures( shader *s );
		void unSetTextures();

	};

}
