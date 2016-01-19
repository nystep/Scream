
#include "screamCore.h"


namespace SCore {

	using namespace SCore;

	extern DataBuffer *dataBuffer;

	static bool importCSM1( iobFile* csmFileIn, float **data, unsigned int &sx, unsigned int &sy, unsigned int stride )
	{
		char c[4];

		csmFileIn->readC(c[0]);
		csmFileIn->readC(c[1]);
		csmFileIn->readC(c[2]);
		csmFileIn->readC(c[3]);

		if (!(c[0] == 'N' && c[1] == 'C' && c[2] == 'S' && c[3] == 'M')) return false;

		csmFileIn->readUI( sx );
		csmFileIn->readUI( sy );

		if (sx<16) return false;
		if (sx>8192) return false;
		if (sy<16) return false;
		if (sy>8192) return false;

		unsigned char version, channels;

		csmFileIn->readUC( version );
		csmFileIn->readUC( channels );

		if (version != 1) return false;
		if (channels != 6) return false;
		if (channels > stride) return false;

		unsigned int *channelArray = new unsigned int [channels];

		for (unsigned int i=0; i<channels; i++)
			channelArray[i] = dataBuffer->readQuantizedArray( csmFileIn );

		unsigned int sz = sx*sy;
		*data = new float [sz*stride];

		for (unsigned int i=0; i<sz; i++)
		{
			for (unsigned int j=0; j<stride; j++)
			{
				if (j<channels)
					(*data)[i*stride+j] = dataBuffer->getFloat( channelArray[j], i );
				else
					(*data)[i*stride+j] = 0.0f;
			}
		}

		for (unsigned int i=0; i<channels; i++)
			dataBuffer->deleteFloatArray( channelArray[i] );

		delete [] channelArray;

		return true;
	}

	texture2d* terrainHMLoad( const char* heightmapfilename )
	{
        return new texture2d( heightmapfilename );
	}

	texture2d* terrainCSMLoad( const char* csmfilename, const ENUM_CSMLOADOPTIONS opt )
	{
		unsigned int sx, sy;
		float *fdata = NULL;
		texture2d *result = NULL;
		iobFile *inputCSM = new iobFile( csmfilename, true );

		assert( glGetError() == GL_NO_ERROR );

		if (inputCSM != NULL)
		{
			bool res = importCSM1( inputCSM, &fdata, sx, sy, 8 );

			if (res)
			{
				unsigned int sz = sx*sy;

				assert( sx >= 16 );
				assert( sy >= 16 );
				assert( sx <= 8192 );
				assert( sy <= 8192 );
				assert( fdata != NULL );

				switch(opt)
				{
				case CLO_HEIGHTMAP:
					{
						float *tdata = new float [sz];

						for (unsigned int i=0; i<sz; i++)
						{
							tdata[i] = fabs( fdata[i*8+2] );
						}

						result = new texture2d( sx, sy, TEX_LUMI_F16, filterLinear, wrapClamp, tdata );

						delete [] tdata;
					}
					break;
				case CLO_HEIGHTANGLETANMAP:
					{
						float *tdata = new float [sz*2];

						for (unsigned int i=0; i<sz; i++)
						{
							tdata[i*2+0] = fdata[i*8+2];
							tdata[i*2+1] = tanf( fdata[i*8+4] );
						}

						result = new texture2d( sx, sy, TEX_RG_F16, filterLinear, wrapClamp, tdata );

						delete [] tdata;
					}
					break;
				case CLO_HEIGHTANGLEMAP:
					{
						float *tdata = new float [sz*2];

						for (unsigned int i=0; i<sz; i++)
						{
							tdata[i*2+0] = fdata[i*8+2];
							tdata[i*2+1] = fdata[i*8+4];
						}

						result = new texture2d( sx, sy, TEX_RG_F16, filterLinear, wrapClamp, tdata );

						delete [] tdata;
					}
					break;
				case CLO_CSMMAP:
					{
						float *tdata = new float [sz*4];

						for (unsigned int i=0; i<sz; i++)
						{
							tdata[i*4+0] = fdata[i*8+0];
							tdata[i*4+1] = fdata[i*8+1];
							tdata[i*4+2] = fdata[i*8+2];
							tdata[i*4+3] = fdata[i*8+3];
						}

						result = new texture2d( sx, sy, TEX_RGBA_F16, filterLinear, wrapClamp, tdata );

						delete [] tdata;
					}
					break;
				default:
					break;
				};
			}
		}

		return result;
	}

};
