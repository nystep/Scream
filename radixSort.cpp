#include "radixSort.h"

namespace SCore {

	/** sort keys included in the src array, keep original untouched */
	/** the sorted indices of the src keys is written in inddst */
	void radixSort( unsigned int *src, unsigned int *inddst, unsigned int sz )
	{
		/** because we don't change the src array, we're forced to allocate a
		couple of arrays for the ping pong sorting process */
		unsigned int *tmpsrc = new unsigned int [sz];
		unsigned int *tmpsrc2 = new unsigned int [sz];
		/** This is used to store the indices in the src array */
		unsigned int *tmpind = new unsigned int [sz];

		/** statistics for each byte */
		unsigned int stat[4][256];
		/** destination indice for the scattering operation */
		unsigned int dest[4][256];

		/** initialize statistics to zero */
		for (unsigned int j=0; j<4; j++)
		{
			for (unsigned int i=0; i<256; i++)
			{
				dest[j][i] = 0;
				stat[j][i] = 0;
			}
		}

		/** first O(n) pass, gather byte statistics */
		for (unsigned int i=0; i<sz; i++)
		{
			unsigned int a = src[i];
			unsigned int b0 = a & 0xFF;
			unsigned int b1 = (a>>8) & 0xFF;
			unsigned int b2 = (a>>16) & 0xFF;
			unsigned int b3 = (a>>24) & 0xFF;

			inddst[i] = i;

			stat[0][b0] ++;
			stat[1][b1] ++;
			stat[2][b2] ++;
			stat[3][b3] ++;
		}

		/** calculate the destination indice in dest array */
		for (unsigned int j=0; j<4; j++)
		{
			for (unsigned int i=1; i<256; i++)
			{
				dest[j][i] = stat[j][i-1];
				stat[j][i] += stat[j][i-1];
			}
		}

		/** O(n) highest significant byte sorting pass */
		for (unsigned int i=0; i<sz; i++)
		{
			unsigned int b3 = (src[i] >> 24) & 0xFF;
			tmpsrc[dest[3][b3]] = src[i];
			tmpind[dest[3][b3]] = inddst[i];
			dest[3][b3]++;
		}

		/** O(n) second byte sorting pass */
		for (unsigned int i=0; i<sz; i++)
		{
			unsigned int b2 = (tmpsrc[i] >> 16) & 0xFF;
			tmpsrc2[dest[2][b2]] = tmpsrc[i];
			inddst[dest[2][b2]] = tmpind[i];
			dest[2][b2]++;
		}

		/** O(n) 3rd byte sorting pass */
		for (unsigned int i=0; i<sz; i++)
		{
			unsigned int b1 = (tmpsrc2[i] >> 8) & 0xFF;
			tmpsrc[dest[1][b1]] = tmpsrc2[i];
			tmpind[dest[1][b1]] = inddst[i];
			dest[1][b1]++;
		}

		/** O(n) lowest significant byte sorting pass */
		for (unsigned int i=0; i<sz; i++)
		{
			unsigned int b0 = tmpsrc[i] & 0xFF;
			//tmpsrc2[dest[0][b0]] = tmpsrc[i];
			inddst[dest[0][b0]] = tmpind[i];
			dest[0][b0]++;
		}

		/** cleanup memory */
		delete [] tmpsrc2;
		delete [] tmpsrc;
		delete [] tmpind;
	}

};
