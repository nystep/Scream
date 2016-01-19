#pragma once

namespace SCore {

	int iobReadInt32( FILE* fin );
	float iobReadFloat32( FILE* fin );

	void iobWriteInt32( int i, FILE* fout );
	void iobWriteFloat32( float f, FILE* fout );

}
