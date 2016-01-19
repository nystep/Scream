
#include "texture.h"
#include "smath.h"
#ifdef _WINDOWS
#include <windows.h>
#endif
#include <IL/il.h>

namespace SCore {

#ifdef _WINDOWS
	texture2dArray::texture2dArray( const char *imageFileNamesFilter )
	{
		unsigned char *pels = 0;
		unsigned int nFiles = 0, nStorage = 0, currentLayer = 0;
		unsigned int Width = 0, Height = 0, Bpp = 0;
		WIN32_FIND_DATAA fsearch;
		HANDLE wsearch = FindFirstFileA( imageFileNamesFilter, &fsearch );

		if (wsearch != INVALID_HANDLE_VALUE)
		{
			// count the number of files that match...
			do { nFiles++; } while (FindNextFileA( wsearch, &fsearch )) ;
			FindClose( wsearch );
			nStorage = 1 << getPOTHigher( nFiles );

			// and load them
			wsearch = FindFirstFileA( imageFileNamesFilter, &fsearch );
			nFiles = 0;
			do
			{
				//if (!ilLoadImage( (wchar_t*) &fsearch.cFileName[0] )) continue;
				if (!ilLoadImage( (wchar_t*) &fsearch.cFileName[0] )) continue;

				if (!pels)
				{
					Width = ilGetInteger(IL_IMAGE_WIDTH);
					Height = ilGetInteger(IL_IMAGE_HEIGHT);
					Bpp = ilGetInteger(IL_IMAGE_BPP);
					pels = new unsigned char [Width*Height*4];

					// TODO: fix, lol
					//this->texture2dArray::texture2dArray( Width, Height, nStorage, TEX_RGBA_8, NULL );
				}
				else if ( ilGetInteger(IL_IMAGE_WIDTH) == int(Width) ||
					ilGetInteger(IL_IMAGE_HEIGHT) == int(Height) ||
					ilGetInteger(IL_IMAGE_BPP) == int(Bpp) )
				{
					ilCopyPixels( 0, 0, 0, Width, Height, 1, IL_RGBA, IL_UNSIGNED_BYTE, pels );
					glTexSubImage3D( GL_TEXTURE_2D_ARRAY_EXT, 0, 0, 0, nFiles, Width, Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, pels );
					nFiles++;
				}
			}
			while (FindNextFileA( wsearch, &fsearch )) ;

			nUsedLayers = nFiles;
			FindClose( wsearch );
			delete [] pels;
		}

		assert( glGetError() == GL_NO_ERROR );
	}
#endif


	texture2dArray::texture2dArray( unsigned int sx, unsigned int sy, unsigned int nb, textureInternalFormat tif, float* pels )
	{
		unsigned int internalFmt = getGlInternalFormat( tif );
		unsigned int externalFmt = getGlExternalFormat( tif );
		unsigned int pixelTransfertFmt = getGlExternalPixelTransfertType( tif );

		internalFormat = internalFmt;
		texture2dArray::sx = sx;
		texture2dArray::sy = sy;
		texture2dArray::sz = nb;
		target = GL_TEXTURE_2D_ARRAY_EXT;
		nUsedLayers = nb;

		glGenTextures( 1, &textureId );
		glBindTexture( target, textureId );
		glTexImage3D( target, 0, internalFmt, sx, sy, sz, 0, externalFmt, pixelTransfertFmt, pels );
		//glBindTexture( target, 0 );

		assert( glGetError() == GL_NO_ERROR );
	}


	texture2dArray::~texture2dArray()
	{
		glDeleteTextures( 1, &textureId );
	}


	void texture2dArray::copyLayerFromFramebuffer( int layer )
	{
		glBindTexture( target, textureId );
		glCopyTexSubImage3D( target, 0, 0, 0, layer, 0, 0, sx, sy );
		glBindTexture( target, 0 );

		assert( glGetError() == GL_NO_ERROR );
	}

};
