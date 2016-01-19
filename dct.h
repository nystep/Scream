#pragma once

namespace SCore {

	// discrete cosine transform
	void dct1_1d( float *signal, float *transformed, int size, int stride, float scale=1.0f );
	void dct2_1d( float *signal, float *transformed, int size, int stride, float scale=1.0f );
	void dct3_1d( float *signal, float *transformed, int size, int stride, float scale=1.0f );
	float *make_dct4_cos_table( int size );
	void dct4_1d_ct( float *signal, float *transformed, float *costable, int size, int stride, float scale=1.0f );
	void dct4_1d( float *signal, float *transformed, int size, int stride, float scale=1.0f );

	// mdct, non windowed version
	void mdct_1d( float *signal, float *transformed, int size, int stride, float scale );
	void imdct_1d( float *signal, float *transformed, int size, int stride, float scale );

}
