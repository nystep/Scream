
#include "shader.h"

#include <webp/encode.h>
#include <webp/decode.h>
#include <IL/il.h>
#include <string>
#include <string.h>
#include "dataFile.h"


namespace SCore {

	bool textureUtils::hasWpExtension( const char *filename )
	{
		//printf( "textureUtils::hasWpExtension %s\n", filename );
		size_t flen = strlen( filename );

		if (flen<3) return false;
		if (strncmp(filename+flen-3,".wp",3) == 0) return true;

		return false;
	}

	std::string textureUtils::getWpFilename( const char *filename )
	{
		return std::string(filename) + std::string(".wp");
	}

	bool textureUtils::convertImageToWp( const char *filename, bool *hasAlphaChannel, const float quality )
	{
		unsigned int ImageName;
		unsigned int Width, Height, Bpp, Channels, BPC;

		ilGenImages( 1, &ImageName );
		ilBindImage( ImageName );

#ifdef _WINDOWS
		ilLoadImage( (wchar_t*) filename );
#else
		ilLoadImage( filename );
#endif

		Width = ilGetInteger(IL_IMAGE_WIDTH);
		Height = ilGetInteger(IL_IMAGE_HEIGHT);
		Bpp = ilGetInteger(IL_IMAGE_BPP);
		Channels = ilGetInteger(IL_IMAGE_CHANNELS);
		BPC = Bpp / Channels;

		if (BPC>1)
			printf( "-> convertToWebp precision warning: only 8 bit precision supported\n" );

		if (Channels<3)
		{
			printf( "-> convertToWebp color warning: greyscale images are converted to rgb\n" );
			Channels = 3;
		}

		unsigned int ilFmt = IL_RGB;
		if (Channels == 4)
		{
			ilFmt = IL_RGBA;
			if (hasAlphaChannel != NULL) *hasAlphaChannel = true;
		}

		unsigned char *myPels = new unsigned char [Width*Height*Channels];
		ilCopyPixels( 0, 0, 0, Width, Height, 1, ilFmt, IL_UNSIGNED_BYTE, myPels );

		unsigned int compressedSz;
		unsigned char *compressed;

		if (ilFmt == IL_RGB)
			compressedSz = WebPEncodeRGB( myPels, Width, Height, Width*3, quality, &compressed );
		else
			compressedSz = WebPEncodeRGBA( myPels, Width, Height, Width*4, quality, &compressed );

		printf( "> convertToWebp compressed texture of %dx%d down to %u bytes\n", Width, Height, compressedSz );
		delete [] myPels;

		/// write the file to disk
		std::string outfilename = getWpFilename( filename );
		FILE *outf = fopen( outfilename.c_str(), "wb" );
		if (!outf) return false;
		if (fwrite( compressed, compressedSz, 1, outf ) != 1) return false;
		fclose( outf );

		/// write file to datafile
		dataFile::Instance().addDataFileFromHeap( getWpFilename( filename ), compressedSz, compressed );

		free( compressed );

		return true;
	}

	bool textureUtils::upload2dImageWp( unsigned int target, const char *filename )
	{
		static const char* const vp8StatusMessages[] = {
			"OK", "OUT_OF_MEMORY", "INVALID_PARAM", "BITSTREAM_ERROR",
			"UNSUPPORTED_FEATURE", "SUSPENDED", "USER_ABORT", "NOT_ENOUGH_DATA"
		};

		int imgWidth, imgHeight;
		unsigned char *imgBuffer;
		unsigned char *imgDecompressedBuffer;
		unsigned int imgBufferSz;
		unsigned int glInternalFormat, glExternalFormat;

		if (!hasWpExtension(filename)) return false;

		printf( "textureUtils::upload2dImageWp %s\n", filename );

		FILE *inf = fopen( filename, "rb" );
		if (!inf) return false;
		fseek( inf, 0, SEEK_END );
		imgBufferSz = ftell( inf );
		imgBuffer = new unsigned char [imgBufferSz];
		rewind( inf );
		bool readCompleted = fread( imgBuffer, imgBufferSz, 1, inf ) == 1;
		fclose( inf );

		if (!readCompleted)
		{
			delete [] imgBuffer;
			return false;
		}

		printf( "textureUtils::upload2dImageWp file of %d bytes read with success\n", imgBufferSz );

		//VP8GetCPUInfo = NULL;
		VP8StatusCode status = VP8_STATUS_OK;
		WebPDecoderConfig config;
		WebPDecBuffer* const output_buffer = &config.output;
		WebPBitstreamFeatures* const bitstream = &config.input;
		if (!WebPInitDecoderConfig( &config ))
		{
			delete [] imgBuffer;
			return false;
		}

		status = WebPGetFeatures( imgBuffer, imgBufferSz, bitstream );
		printf( "WebPGetFeatures vp8 status: %s\n", vp8StatusMessages[status] );
		if (status != VP8_STATUS_OK)
		{
			delete [] imgBuffer;
			return false;
		}

		printf( "textureUtils::upload2dImageWp validated the header of the file %s\n", filename );

		output_buffer->colorspace = bitstream->has_alpha ? MODE_RGBA : MODE_RGB;
		glInternalFormat = bitstream->has_alpha ? GL_RGBA8 : GL_RGB8;
		glExternalFormat = bitstream->has_alpha ? GL_RGBA : GL_RGB;
		imgWidth = bitstream->width;
		imgHeight = bitstream->height;

		/// decode the file
		status = WebPDecode( imgBuffer, imgBufferSz, &config );
		printf( "WebPDecode vp8 status: %s\n", vp8StatusMessages[status] );
		delete [] imgBuffer;

		if (status != VP8_STATUS_OK)
		{
			WebPFreeDecBuffer( &config.output );
			return false;
		}

		imgDecompressedBuffer = config.output.u.RGBA.rgba;

		printf( "textureUtils::upload2dImageWp imgDecompressedBuffer=%p imgWidth=%d imgHeight=%d\n",
			imgDecompressedBuffer, imgWidth, imgHeight );

		if (imgDecompressedBuffer == NULL) return false;

		printf( "textureUtils::upload2dImageWp decompressed with success\n" );

		glTexImage2D( target, 0, glInternalFormat, imgWidth, imgHeight, 0,
			glExternalFormat, GL_UNSIGNED_BYTE, imgDecompressedBuffer );

		WebPFreeDecBuffer( &config.output );

		return true;
	}

}
