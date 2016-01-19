#pragma once

#include <assert.h>
#define GLEW_STATIC
#include <GL/glew.h>

namespace SCore {

	typedef enum {
		TEX_RGB_8,
		TEX_RGBA_8,
		TEX_RG_8,
		TEX_LUMI_8,

		TEX_RGB_16,
		TEX_RGBA_16,
		TEX_RG_16,
		TEX_LUMI_16,

		TEX_RGB_F16,
		TEX_RGBA_F16,
		TEX_RG_F16,
		TEX_LUMI_F16,

		TEX_RGB_F32,
		TEX_RGBA_F32,
		TEX_RG_F32,
		TEX_LUMI_F32,

		TEX_RGB_I32,
		TEX_RGBA_I32,
		TEX_RG_I32,
		TEX_LUMI_I32,

		TEX_RGB_UI32,
		TEX_RGBA_UI32,
		TEX_RG_UI32,
		TEX_LUMI_UI32
	} textureInternalFormat;

	typedef enum {
		filterNearest,
		filterLinear,
		filterMipmaps,
		filterAnisotropic
	} textureFiltering;

	typedef enum {
		wrapClamp,
		wrapRepeat,
		wrapMirroredRepeat
	} textureWrapMode;

	class shader;
	class shaderUniformSet;
	class shaderTextureSet;
	class shaderBufferSet;
	class nsfScene;

	class texture
	{
	protected:

		int convertFilteringMinToGl( textureFiltering sf );
		int convertFilteringMagToGl( textureFiltering sf );
		int convertWrappingToGl( textureWrapMode wm );
		unsigned int getGlInternalFormat( textureInternalFormat fm );
		unsigned int getGlExternalFormat( textureInternalFormat fm );
		unsigned int getGlExternalPixelTransfertType( textureInternalFormat fm );
		unsigned int getIlExternalFormat( textureInternalFormat fm );
		textureInternalFormat getInternalFormat( unsigned int nChannels, unsigned int bytesPerChan );
		unsigned int getFormatNumberOfChannels( textureInternalFormat fm );
		unsigned int getFormatBytesPerChannel( textureInternalFormat fm );
		unsigned int getDefaultFiltering( unsigned int glPixelTransfertType );

	public:

		unsigned int target, textureId, bytesPerChannel, nChannels, fbo, fbodepth, internalFormat, mipmapping;
		unsigned int sx, sy, sz;

		texture() : target(0), textureId(0), bytesPerChannel(0), nChannels(0), fbo(0), fbodepth(0),
			internalFormat(0), mipmapping(0), sx(0), sy(0), sz(0) {}

                texture(const texture& t)
                {
                    target = t.target;
                    textureId = t.textureId;
                    bytesPerChannel = t.bytesPerChannel;
                    nChannels = t.nChannels;
                    fbo = t.fbo;
                    fbodepth = t.fbodepth;
                    internalFormat = t.internalFormat;
                    mipmapping = t.mipmapping;
                    sx = t.sx;
                    sy = t.sy;
                    sz = t.sz;
                }

                texture& operator= (const texture& t)
                {
                    target = t.target;
                    textureId = t.textureId;
                    bytesPerChannel = t.bytesPerChannel;
                    nChannels = t.nChannels;
                    fbo = t.fbo;
                    fbodepth = t.fbodepth;
                    internalFormat = t.internalFormat;
                    mipmapping = t.mipmapping;
                    sx = t.sx;
                    sy = t.sy;
                    sz = t.sz;
                    return *this;
                }

		virtual ~texture() {}

		void render( shader *s,
			bool depthbuffer=false,
			shaderUniformSet *uniforms=0,
			shaderTextureSet *textures=0,
			unsigned int nLayers=1,
			nsfScene *someModel=0,
			unsigned int mipmapLevel=0 );
		void generateMipmaps();
		void freeFBO();

	};

}

#include "textureUtils.h"
#include "texture2d.h"
#include "texture3d.h"
#include "textureCubeMap.h"
#include "textureCubeMapArray.h"
#include "texture2dArray.h"
#include "meshVBO.h"
#include "textureBuffer.h"
#include "textureVideo.h"
#include "textureCompressor.h"
