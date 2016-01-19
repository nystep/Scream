

#include "screamCore.h"
#include <IL/il.h>


namespace SCore {

	unsigned int texture::getIlExternalFormat( textureInternalFormat fm )
	{
		switch (fm)
		{
		case TEX_RGB_8: return IL_RGB;
		case TEX_RGBA_8: return IL_RGBA;
		case TEX_RG_8: return IL_LUMINANCE_ALPHA;
		case TEX_LUMI_8: return IL_LUMINANCE;
			/*
			case TEX_RGB_S8: return ;
			case TEX_RGBA_S8: return ;
			case TEX_RG_S8: return ;
			case TEX_LUMI_S8: return ;
			*/
		case TEX_RGB_16: return IL_RGB;
		case TEX_RGBA_16: return IL_RGBA;
		case TEX_RG_16: return IL_LUMINANCE_ALPHA;
		case TEX_LUMI_16: return IL_LUMINANCE;
			/*
			case TEX_RGB_S16: return ;
			case TEX_RGBA_S16: return ;
			case TEX_RG_S16: return ;
			case TEX_LUMI_S16: return ;
			*/
		case TEX_RGB_F16: return IL_RGB;
		case TEX_RGBA_F16: return IL_RGBA;
		case TEX_RG_F16: return IL_LUMINANCE_ALPHA;
		case TEX_LUMI_F16: return IL_LUMINANCE;

		case TEX_RGB_F32: return IL_RGB;
		case TEX_RGBA_F32: return IL_RGBA;
		case TEX_RG_F32: return IL_LUMINANCE_ALPHA;
		case TEX_LUMI_F32: return IL_LUMINANCE;

		case TEX_RGB_I32: return IL_RGB;
		case TEX_RGBA_I32: return IL_RGBA;
		case TEX_RG_I32: return IL_LUMINANCE_ALPHA;
		case TEX_LUMI_I32: return IL_LUMINANCE;

		case TEX_RGB_UI32: return IL_RGB;
		case TEX_RGBA_UI32: return IL_RGBA;
		case TEX_RG_UI32: return IL_LUMINANCE_ALPHA;
		case TEX_LUMI_UI32: return IL_LUMINANCE;

		default: return IL_RGBA;
		};
	}


	unsigned int texture::getGlInternalFormat( textureInternalFormat fm )
	{
		switch (fm)
		{
		case TEX_RGB_8: return GL_RGB8;
		case TEX_RGBA_8: return GL_RGBA8;
		case TEX_RG_8: return GL_RG8;
		case TEX_LUMI_8: return GL_LUMINANCE8;
			/*
			case TEX_RGB_S8: return GL_RGB8I_EXT;
			case TEX_RGBA_S8: return GL_RGBA8I_EXT;
			case TEX_RG_S8: return GL_RG8I;
			case TEX_LUMI_S8: return GL_LUMINANCE8I_EXT;
			*/
		case TEX_RGB_16: return GL_RGB16;
		case TEX_RGBA_16: return GL_RGBA16;
		case TEX_RG_16: return GL_RG16;
		case TEX_LUMI_16: return GL_LUMINANCE16;
			/*
			case TEX_RGB_S16: return ;
			case TEX_RGBA_S16: return ;
			case TEX_RG_S16: return ;
			case TEX_LUMI_S16: return ;
			*/
		case TEX_RGB_F16: return GL_RGB16F;
		case TEX_RGBA_F16: return GL_RGBA16F;
		case TEX_RG_F16: return GL_RG16F;
		case TEX_LUMI_F16: return GL_LUMINANCE16F_ARB;

		case TEX_RGB_F32: return GL_RGB32F;
		case TEX_RGBA_F32: return GL_RGBA32F;
		case TEX_RG_F32: return GL_RG32F;
		case TEX_LUMI_F32: return GL_LUMINANCE32F_ARB;

		case TEX_RGB_I32: return GL_RGB32I;
		case TEX_RGBA_I32: return GL_RGBA32I;
		case TEX_RG_I32: return GL_RG32I;
		case TEX_LUMI_I32: return GL_R32I;

		case TEX_RGB_UI32: return GL_RGB32UI;
		case TEX_RGBA_UI32: return GL_RGBA32UI;
		case TEX_RG_UI32: return GL_RG32UI;
		case TEX_LUMI_UI32: return GL_R32UI;
		default: return 0;
		};
	}


	unsigned int texture::getGlExternalFormat( textureInternalFormat fm )
	{
		switch (fm)
		{
		case TEX_RGB_8: return GL_RGB;
		case TEX_RGBA_8: return GL_RGBA;
		case TEX_RG_8: return GL_RG;
		case TEX_LUMI_8: return GL_LUMINANCE;
			/*
			case TEX_RGB_S8: return GL_RGB;
			case TEX_RGBA_S8: return GL_RGBA;
			case TEX_RG_S8: return GL_RG;
			case TEX_LUMI_S8: return GL_LUMINANCE;
			*/
		case TEX_RGB_16: return GL_RGB;
		case TEX_RGBA_16: return GL_RGBA;
		case TEX_RG_16: return GL_RG;
		case TEX_LUMI_16: return GL_LUMINANCE;

		case TEX_RGB_F16: return GL_RGB;
		case TEX_RGBA_F16: return GL_RGBA;
		case TEX_RG_F16: return GL_RG;
		case TEX_LUMI_F16: return GL_LUMINANCE;

		case TEX_RGB_F32: return GL_RGB;
		case TEX_RGBA_F32: return GL_RGBA;
		case TEX_RG_F32: return GL_RG;
		case TEX_LUMI_F32: return GL_LUMINANCE;

		case TEX_RGB_I32: return GL_RGB_INTEGER;
		case TEX_RGBA_I32: return GL_RGBA_INTEGER;
		case TEX_RG_I32: return GL_RG_INTEGER;
		case TEX_LUMI_I32: return GL_RED_INTEGER;

		case TEX_RGB_UI32: return GL_RGB_INTEGER;
		case TEX_RGBA_UI32: return GL_RGBA_INTEGER;
		case TEX_RG_UI32: return GL_RG_INTEGER;
		case TEX_LUMI_UI32: return GL_RED_INTEGER;

		default: return 0;
		};
	}

	unsigned int texture::getGlExternalPixelTransfertType( textureInternalFormat fm )
	{
		switch (fm)
		{
		case TEX_RGB_8: return GL_FLOAT;
		case TEX_RGBA_8: return GL_FLOAT;
		case TEX_RG_8: return GL_FLOAT;
		case TEX_LUMI_8: return GL_FLOAT;
			/*
			case TEX_RGB_S8: return GL_RGB;
			case TEX_RGBA_S8: return GL_RGBA;
			case TEX_RG_S8: return GL_RG;
			case TEX_LUMI_S8: return GL_LUMINANCE;
			*/
		case TEX_RGB_16: return GL_FLOAT;
		case TEX_RGBA_16: return GL_FLOAT;
		case TEX_RG_16: return GL_FLOAT;
		case TEX_LUMI_16: return GL_FLOAT;

		case TEX_RGB_F16: return GL_FLOAT;
		case TEX_RGBA_F16: return GL_FLOAT;
		case TEX_RG_F16: return GL_FLOAT;
		case TEX_LUMI_F16: return GL_FLOAT;

		case TEX_RGB_F32: return GL_FLOAT;
		case TEX_RGBA_F32: return GL_FLOAT;
		case TEX_RG_F32: return GL_FLOAT;
		case TEX_LUMI_F32: return GL_FLOAT;

		case TEX_RGB_I32: return GL_INT;
		case TEX_RGBA_I32: return GL_INT;
		case TEX_RG_I32: return GL_INT;
		case TEX_LUMI_I32: return GL_INT;

		case TEX_RGB_UI32: return GL_UNSIGNED_INT;
		case TEX_RGBA_UI32: return GL_UNSIGNED_INT;
		case TEX_RG_UI32: return GL_UNSIGNED_INT;
		case TEX_LUMI_UI32: return GL_UNSIGNED_INT;

		default: return 0;
		};
	}

	unsigned int texture::getDefaultFiltering( unsigned int glPixelTransfertType )
	{
		switch (glPixelTransfertType)
		{
		case GL_INT:
		case GL_UNSIGNED_INT:
			return GL_NEAREST;
		default:
			return GL_LINEAR;
		}
	}


	textureInternalFormat texture::getInternalFormat( unsigned int nChannels,
		unsigned int bytesPerChan )
	{
		if (bytesPerChan == 1)
		{
			switch (nChannels)
			{
			case 1: return TEX_LUMI_8;
			case 2: return TEX_RG_8;
			case 3: return TEX_RGB_8;
			default:
			case 4: return TEX_RGBA_8;
			}
		}
		else if (bytesPerChan == 2)
		{
			switch (nChannels)
			{
			case 1: return TEX_LUMI_F16;
			case 2: return TEX_RG_F16;
			case 3: return TEX_RGB_F16;
			default:
			case 4: return TEX_RGBA_F16;
			}
		}
		else if (bytesPerChan > 2)
		{
			switch (nChannels)
			{
			case 1: return TEX_LUMI_F32;
			case 2: return TEX_RG_F32;
			case 3: return TEX_RGB_F32;
			default:
			case 4: return TEX_RGBA_F32;
			}
		}
		else return TEX_RGBA_8;
	}

	unsigned int texture::getFormatNumberOfChannels( textureInternalFormat fm )
	{
		switch (fm)
		{
		case TEX_RGB_8: return 3;
		case TEX_RGBA_8: return 4;
		case TEX_RG_8: return 2;
		case TEX_LUMI_8: return 1;

		case TEX_RGB_16: return 3;
		case TEX_RGBA_16: return 4;
		case TEX_RG_16: return 2;
		case TEX_LUMI_16: return 1;

		case TEX_RGB_F16: return 3;
		case TEX_RGBA_F16: return 4;
		case TEX_RG_F16: return 2;
		case TEX_LUMI_F16: return 1;

		case TEX_RGB_F32: return 3;
		case TEX_RGBA_F32: return 4;
		case TEX_RG_F32: return 2;
		case TEX_LUMI_F32: return 1;

		case TEX_RGB_I32: return 3;
		case TEX_RGBA_I32: return 4;
		case TEX_RG_I32: return 2;
		case TEX_LUMI_I32: return 1;

		case TEX_RGB_UI32: return 3;
		case TEX_RGBA_UI32: return 4;
		case TEX_RG_UI32: return 2;
		case TEX_LUMI_UI32: return 1;

		default: return 0;
		};
	}

	unsigned int texture::getFormatBytesPerChannel( textureInternalFormat fm )
	{
		switch (fm)
		{
		case TEX_RGB_8: return 1;
		case TEX_RGBA_8: return 1;
		case TEX_RG_8: return 1;
		case TEX_LUMI_8: return 1;

		case TEX_RGB_16: return 2;
		case TEX_RGBA_16: return 2;
		case TEX_RG_16: return 2;
		case TEX_LUMI_16: return 2;

		case TEX_RGB_F16: return 2;
		case TEX_RGBA_F16: return 2;
		case TEX_RG_F16: return 2;
		case TEX_LUMI_F16: return 2;

		case TEX_RGB_F32: return 4;
		case TEX_RGBA_F32: return 4;
		case TEX_RG_F32: return 4;
		case TEX_LUMI_F32: return 4;

		case TEX_RGB_I32: return 4;
		case TEX_RGBA_I32: return 4;
		case TEX_RG_I32: return 4;
		case TEX_LUMI_I32: return 4;

		case TEX_RGB_UI32: return 4;
		case TEX_RGBA_UI32: return 4;
		case TEX_RG_UI32: return 4;
		case TEX_LUMI_UI32: return 4;
		default: return 0;
		};
	}

	int texture::convertFilteringMinToGl( textureFiltering sf )
	{
		switch (sf)
		{
		case filterNearest:
			return GL_NEAREST;
			break;
		case filterLinear:
			return GL_LINEAR;
			break;
		case filterMipmaps:
		case filterAnisotropic:
			return GL_LINEAR_MIPMAP_LINEAR;
			break;
		default:
			return 0;
			break;
		};
	}

	int texture::convertFilteringMagToGl( textureFiltering sf )
	{
		switch (sf)
		{
		case filterNearest:
			return GL_NEAREST;
			break;
		case filterLinear:
		case filterMipmaps:
		case filterAnisotropic:
			return GL_LINEAR;
			break;
		default:
			return 0;
			break;
		};
	}

	int texture::convertWrappingToGl( textureWrapMode wm )
	{
		switch (wm)
		{
		case wrapClamp:
			return GL_CLAMP;
			break;
		case wrapRepeat:
			return GL_REPEAT;
			break;
		case wrapMirroredRepeat:
			return GL_MIRRORED_REPEAT;
			break;
		default:
			return 0;
			break;
		};
	}

	static bool checkFBO()
	{
		GLenum status;
		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status) {
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				printf( "GL_FRAMEBUFFER_COMPLETE_EXT\n" );
				return true;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				assert(false && "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				assert(false && "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				assert(false && "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				assert(false && "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				assert(false && "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				assert(false && "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
				return false;
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				assert(false && "GL_FRAMEBUFFER_UNSUPPORTED_EXT");
				return false;
				break;
			default:
				assert(false && "Unknown ERROR");
				return false;
		}
	}

	void texture::render( shader *s, bool depthbuffer,
		shaderUniformSet *uniforms,
		shaderTextureSet *textures,
		unsigned int nLayers,
		nsfScene *someModel,
		unsigned int mipmapLevel )
	{
		int renderSx, renderSy;

		if (nLayers == 0) nLayers = sz;

		glBindTexture( target, textureId );
		glGetTexLevelParameteriv( target, mipmapLevel, GL_TEXTURE_WIDTH, &renderSx );
		glGetTexLevelParameteriv( target, mipmapLevel, GL_TEXTURE_HEIGHT, &renderSy );

		if (fbo == 0)
		{
			glGenFramebuffers( 1, &fbo );
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );

			if (target == GL_TEXTURE_2D)
				glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, textureId, mipmapLevel );
			else if (target == GL_TEXTURE_3D)
				glFramebufferTexture3D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, textureId, mipmapLevel, 0 );
			else if (target == GL_TEXTURE_2D_ARRAY)
				glFramebufferTextureLayer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, mipmapLevel, 0 );
			else if (target == GL_TEXTURE_CUBE_MAP)
				glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, textureId, mipmapLevel );
			else return;

			if (depthbuffer)
			{
				glGenFramebuffers( 1, &fbodepth );
				printf( "texture::render -> opengl error: 0x%x fbodepth %d\n", glGetError(), fbodepth );
				glBindRenderbuffer( GL_RENDERBUFFER, fbodepth );
				printf( "texture::render -> opengl error: 0x%x\n", glGetError() );
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderSx, renderSy );
				printf( "texture::render -> opengl error: 0x%x\n", glGetError() );
				glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbodepth );
				printf( "texture::render -> opengl error: 0x%x\n", glGetError() );
			}

			checkFBO();
		}

		/// render to texture
		glPushAttrib( GL_VIEWPORT_BIT );
		glViewport( 0, 0, renderSx, renderSy );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );

		glUseProgram( s->programID );
		assert( glGetError() == GL_NO_ERROR );
		if (textures) textures->setTextures( s );
		assert( glGetError() == GL_NO_ERROR );
		if (uniforms) uniforms->setUniforms( s );
		assert( glGetError() == GL_NO_ERROR );

		if (someModel)
		{
			someModel->render( s, nLayers );
		}
		else
		{
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
		}

		assert( glGetError() == GL_NO_ERROR );

		if (textures) textures->unSetTextures();
		glUseProgram( 0 );

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glPopAttrib();

		assert( glGetError() == GL_NO_ERROR );
	}

	void texture::generateMipmaps()
	{
		glBindTexture( target, textureId );
		glGenerateMipmap( target );
	}

	void texture::freeFBO()
	{
		if (fbodepth) glDeleteFramebuffers( 1, &fbodepth );
		if (fbo) glDeleteFramebuffers( 1, &fbo );

		fbodepth = 0;
		fbo = 0;
	}

};
