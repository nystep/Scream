
#include "texture.h"
#include "shader.h"
#include <webp/encode.h>
#include <webp/decode.h>
#include <stdio.h>

#include <IL/il.h>

namespace SCore {

#if 0
	texture2d::texture2d( const char *filename, textureFiltering sf, textureWrapMode wm, int anisotropy )
	{
		ILuint ImageName;
		unsigned int internalFmt, externalFmt, ilFmt, pixelTransfertFmt;
		unsigned int Width, Height, Bpp, Channels, BPC;
		int minificationFilter = convertFilteringMinToGl( sf );
		int magnificationFilter = convertFilteringMagToGl( sf );
		int wrappingMode = convertWrappingToGl( wm );

		int maxAnisotropy = 16;
		int aniso = anisotropy > maxAnisotropy ? maxAnisotropy : anisotropy;
		if (aniso<1) aniso=1;

		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);
		//ilLoadImage((wchar_t*) filename);
		ilLoadImage(filename);

		convertToWp( 90.0f );

		Width = ilGetInteger(IL_IMAGE_WIDTH);
		Height = ilGetInteger(IL_IMAGE_HEIGHT);
		Bpp = ilGetInteger(IL_IMAGE_BPP);
		Channels = ilGetInteger(IL_IMAGE_CHANNELS);
		BPC = Bpp / Channels;

		textureInternalFormat fm = getInternalFormat( Channels, BPC );
		internalFormat = internalFmt = getGlInternalFormat( fm );
		externalFmt = getGlExternalFormat( fm );
		ilFmt = getIlExternalFormat( fm );
		pixelTransfertFmt = getGlExternalPixelTransfertType( fm );

		target = GL_TEXTURE_2D;
		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		float *myPels = new float [Width*Height*Channels];
		ilCopyPixels( 0, 0, 0, Width, Height, 1, ilFmt, IL_FLOAT, myPels );
		glTexImage2D( target, 0, internalFmt, Width, Height, 0, externalFmt, pixelTransfertFmt, myPels );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, minificationFilter );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, magnificationFilter );
		if (aniso>1) glTexParameteri( target, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, wrappingMode );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, wrappingMode );
		// glTexParameteri( target, GL_TEXTURE_WRAP_R, wrappingMode );
		delete [] myPels;

		bytesPerChannel = BPC;
		nChannels = Channels;
		sx = Width;
		sy = Height;
		sz = 1;
		mipmapsGenerated = false;

		ilDeleteImages(1, &ImageName);

		assert( glGetError() == GL_NO_ERROR );
	}
#endif

	texture2d::texture2d( const char *filename, textureFiltering sf, textureWrapMode wm, int anisotropy )
	{
		int minificationFilter = convertFilteringMinToGl( sf );
		int magnificationFilter = convertFilteringMagToGl( sf );
		int wrappingMode = convertWrappingToGl( wm );

		int maxAnisotropy = 16;
		int aniso = anisotropy > maxAnisotropy ? maxAnisotropy : anisotropy;
		if (aniso<1) aniso=1;

		std::string imgFilename( filename );
		bool hasAlphaChannel = false;

		if ( !hasWpExtension(filename) )
		{
			convertImageToWp( filename, &hasAlphaChannel, 90.0f );
			imgFilename = getWpFilename( filename );
		}

		target = GL_TEXTURE_2D;

		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		upload2dImageWp( target, imgFilename.c_str() );

		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, minificationFilter );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, magnificationFilter );
		if (aniso>1) glTexParameteri( target, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, wrappingMode );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, wrappingMode );

		bytesPerChannel = 1;
		nChannels = hasAlphaChannel ? 4 : 3;

		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*) &internalFormat );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_WIDTH, (GLint*) &sx );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_HEIGHT, (GLint*) &sy );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_DEPTH, (GLint*) &sz );

		mipmapsGenerated = false;

		assert( glGetError() == GL_NO_ERROR );
	}

	//texture2d::texture2d( unsigned int sx, unsigned int sy, textureInternalFormat tif, float* pels )
	texture2d::texture2d( unsigned int sx, unsigned int sy,
		textureInternalFormat tif, textureFiltering sf,
		textureWrapMode wm, void* pels )
	{
		assert( glGetError() == GL_NO_ERROR );

		int internalFmt = getGlInternalFormat( tif );
		int externalFmt = getGlExternalFormat( tif );
		int pixelTransfertFmt = getGlExternalPixelTransfertType( tif );
		int minificationFilter = convertFilteringMinToGl( sf );
		int magnificationFilter = convertFilteringMagToGl( sf );
		int wrappingMode = convertWrappingToGl( wm );

		internalFormat = internalFmt;
		target = GL_TEXTURE_2D;
		texture2d::sx = sx;
		texture2d::sy = sy;
		texture2d::sz = 1;
		mipmapsGenerated = false;

		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );

		glTexImage2D( target, 0, internalFmt, sx, sy, 0, externalFmt, pixelTransfertFmt, pels );

		if (sf==filterMipmaps)
		{
			unsigned int mipSx = sx;
			unsigned int mipSy = sy;
			unsigned int mipLevel = 1;

			while (((mipSx>>=1) != 0) && ((mipSy>>=1) != 0))
			{
				//printf( "texture2d::texture2d glTexImage2D mip %d sx %d sy %d\n", mipLevel, mipSx, mipSy );
				glTexImage2D( target, mipLevel, internalFmt, mipSx, mipSy, 0, externalFmt, pixelTransfertFmt, pels );
				mipLevel++;
			}
		}

		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, minificationFilter );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, magnificationFilter );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, wrappingMode );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, wrappingMode );

		//if (sf==filterMipmaps) glTexParameteri( target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );

		assert( glGetError() == GL_NO_ERROR );
	}

	texture2d::~texture2d()
	{
		glDeleteTextures( 1, &textureId );
	}

	float *texture2d::getTextureContent()
	{
		float *result = new float [sx*sy*4];

		glBindTexture( target, textureId );
		glGetTexImage( target, 0, GL_RGBA, GL_FLOAT, result );
		glBindTexture( target, 0 );

		assert( glGetError() == GL_NO_ERROR );

		return result;
	}

	void texture2d::generateMipmapsMax()
	{
		static shader* shMaxMipmap;
		static shaderUniformSet *shMaxMipmapUniforms;
		static shaderUniformIndice mipmapSzXID, mipmapSzYID, mipmapLodID;

		int renderSx, renderSy;
		int maxMipmapLevel = 16;

		glBindTexture( target, textureId );
		glGenerateMipmap( target );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_WIDTH, &renderSx );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_HEIGHT, &renderSy );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP );

		if (!mipmapsGenerated)
		{
			mipmapsGenerated = true;
		}

		if (!shMaxMipmap)
		{
			shMaxMipmap = new shader;
			shMaxMipmap->load( shaderInfo( "shMaxMipmap", GL_FRAGMENT_SHADER ) );

			shMaxMipmapUniforms = new shaderUniformSet;
			mipmapSzXID = shMaxMipmapUniforms->add( UNIFORM_INT, "mipmapSzX", &renderSx );
			mipmapSzYID = shMaxMipmapUniforms->add( UNIFORM_INT, "mipmapSzY", &renderSy );
			mipmapLodID = shMaxMipmapUniforms->add( UNIFORM_INT, "lod", &maxMipmapLevel );
		}

		shaderTextureSet *shMaxMipmapTextures = new shaderTextureSet;
		shMaxMipmapTextures->add( this, "tex", 0 );

		unsigned int fboPyramid[16];

		for (int i=0; i<maxMipmapLevel; i++)
		{
			fboPyramid[i] = 0;
		}

		//printf( " - generateMipmapsMax maxMipmapLevel=%d\n", maxMipmapLevel );

		glPushAttrib( GL_VIEWPORT_BIT );

		for (int i=1; i<maxMipmapLevel; i++)
		{
			renderSx /= 2;
			renderSy /= 2;

			if (renderSx == 0 || renderSy == 0) break;

			//printf( "texture2d::generateMipmapsMax -> generating level %d of size %d*%d\n", i, renderSx, renderSy );
			//printf( "texture2d::generateMipmapsMax -> opengl error: 0x%x\n", glGetError() );

			int im1 = i-1;
			shMaxMipmapUniforms->update( UNIFORM_INT, mipmapSzXID, &renderSx );
			shMaxMipmapUniforms->update( UNIFORM_INT, mipmapSzYID, &renderSy );
			shMaxMipmapUniforms->update( UNIFORM_INT, mipmapLodID, &im1 );
			//glTexParameteri( target, GL_TEXTURE_BASE_LEVEL, i-1 );
			//glTexParameteri( target, GL_TEXTURE_MAX_LEVEL, i-1 );

			glGenFramebuffers( 1, &fboPyramid[i] );
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fboPyramid[i] );
			glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, textureId, i );

			/// render to texture
			glViewport( 0, 0, renderSx, renderSy );
			//glClear( GL_COLOR_BUFFER_BIT );

			glUseProgram( shMaxMipmap->programID );
			shMaxMipmapTextures->setTextures( shMaxMipmap );
			shMaxMipmapUniforms->setUniforms( shMaxMipmap );

			static float quadTexCoords[8] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
			static float quadVertex[12] = { -1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f };
			static unsigned int quadIndices[6] = { 0, 1, 2, 0, 2, 3 };

			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexPointer( 3, GL_FLOAT, 0, quadVertex );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glTexCoordPointer( 2, GL_FLOAT, 0, quadTexCoords );

			glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, quadIndices );

			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );

			shMaxMipmapTextures->unSetTextures();
			glUseProgram( 0 );

			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
			glDeleteFramebuffers( 1, &fboPyramid[i] );

			assert( glGetError() == GL_NO_ERROR );
		}

		glBindTexture( target, textureId );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glBindTexture( target, 0 );

		/*
		glBindTexture( target, textureId );
		glTexParameteri( target, GL_TEXTURE_BASE_LEVEL, 0 );
		glBindTexture( target, 0 );
		*/

		glPopAttrib();

#if 0
		/// TESTING / VALIDATING
		glBindTexture( target, textureId );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_WIDTH, &renderSx );
		glGetTexLevelParameteriv( target, 0, GL_TEXTURE_HEIGHT, &renderSy );

		float *buff = new float [renderSx*renderSy];

		for (int i=0; i<maxMipmapLevel; i++)
		{
			glGetTexImage( target, i, GL_RED, GL_FLOAT, buff );

			//int incrX = renderSx/8 < 1 ? 1 : renderSx/8;
			//int incrY = renderSy/8 < 1 ? 1 : renderSy/8;
			int limitX = renderSx<8 ? renderSx:8;
			int limitY = renderSy<8 ? renderSy:8;

			printf( " # buffer contents at level %d:\n", i );

			for (int y=0; y<limitY; y++)
			{
				for (int x=0; x<limitX; x++)
				{
					int indice = y*renderSx + x;

					printf( "%6f ", buff[indice] );
				}
				printf( "\n" );
			}

			renderSx /= 2;
			renderSy /= 2;
			if (renderSx == 0 || renderSy == 0) break;
		}

		delete [] buff;
		glBindTexture( target, 0 );
#endif

		delete shMaxMipmapTextures;
	}

};
