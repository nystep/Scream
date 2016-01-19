
#include "screamCore.h"

namespace SCore {

	int iobReadInt32( FILE* fin )
	{
		int i;

		i = fgetc( fin );
		i = (i<<8) | fgetc( fin );
		i = (i<<8) | fgetc( fin );
		i = (i<<8) | fgetc( fin );

		return i;
	}


	float iobReadFloat32( FILE* fin )
	{
		float a;
		fread( &a, 1, 4, fin );
		return a;
	}


	void iobWriteInt32( int i, FILE* fout )
	{
		fputc( (i>>24)&0xFF, fout );
		fputc( (i>>16)&0xFF, fout );
		fputc( (i>>8)&0xFF, fout );
		fputc( i&0xFF, fout );
	}


	void iobWriteFloat32( float f, FILE* fout )
	{
		fwrite( &f, 1, 4, fout );
	}

};
