#include <math.h>

namespace SCore {

#define pi 3.1415927f

	void dct1_1d( float *signal, float *transformed, int size, int stride, float scale )
	{
		const float pisnm1 = pi / (size-1);
		for (int k=0; k<size; k++)
		{
			float sum = 0.5f * (signal[0] + (k & 1 ? -1.0f : 1.0f) * signal[stride*(size-1)]);
			for (int n=1; n<size-1; n++)
				sum += cosf(pisnm1 * n * k) * signal[stride*n];
			transformed[k] = sum * scale;
		}
	}


	void dct2_1d( float *signal, float *transformed, int size, int stride, float scale )
	{
		const float pisn = pi / size;
		for (int k=0; k<size; k++)
		{
			float sum = 0.0f;
			for (int n=0; n<size; n++)
				sum += cosf(pisn * (n+0.5f) * k) * signal[stride*n];
			transformed[k] = sum * scale;
		}
	}


	void dct3_1d( float *signal, float *transformed, int size, int stride, float scale )
	{
		const float pisn = pi / size;
		for (int k=0; k<size; k++)
		{
			float sum = 0.5f * signal[0];
			for (int n=1; n<size; n++)
				sum += cosf(pisn * n * (k+0.5f)) * signal[stride*n];
			transformed[k] = sum * scale;
		}
	}

	float *make_dct4_cos_table( int size )
	{
		float *cosTable = new float [size*size];

		const float pisn = pi / size;
		for (int k=0; k<size; k++)
			for (int n=0; n<size; n++)
				cosTable[k*size+n] = cosf(pisn * (n+0.5f) * (k+0.5f));

		return cosTable;
	}

	void dct4_1d_ct( float *signal, float *transformed, float *costable, int size, int stride, float scale )
	{
		for (int k=0; k<size; k++)
		{
			float sum = 0.0f;
			for (int n=0; n<size; n++)
				sum += costable[k*size+n] * signal[stride*n];
			transformed[stride*k] = sum * scale;
		}
	}

	void dct4_1d( float *signal, float *transformed, int size, int stride, float scale )
	{
		const float pisn = pi / size;
		for (int k=0; k<size; k++)
		{
			float sum = 0.0f;
			for (int n=0; n<size; n++)
				sum += cosf(pisn * (n+0.5f) * (k+0.5f)) * signal[stride*n];
			transformed[stride*k] = sum * scale;
		}
	}


	void mdct_1d( float *signal, float *transformed, int size, int stride, float scale )
	{
		const float pisn = pi / size;
		const float ns2p05 = 0.5f + (size >> 1);
		for (int k=0; k<size; k++)
		{
			float sum = 0.0f;
			for (int n=0; n<size*2; n++)
				sum += cosf(pisn * (n+ns2p05) * (k+0.5f)) * signal[stride*n];
			transformed[k] = sum * scale;
		}
	}

	void imdct_1d( float *signal, float *transformed, int size, int stride, float scale )
	{
		const float pisn = pi / size;
		const float ns2p05 = 0.5f + (size >> 1);
		for (int n=0; n<2*size; n++)
		{
			float sum=0.0f;
			for (int k=0; k<size; k++)
				sum += cosf(pisn * (n+ns2p05) * (k+0.5f)) * transformed[stride*k];
			signal[n] += sum * scale;
		}
	}

}
