#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdint.h>


typedef enum {COMPRESS, DECOMPRESS} Mode;


class Encoder
{
protected:

const Mode mode;
FILE *inout;
uint64_t x1, x2, x;

public:

    Encoder(Mode m, FILE* f) : mode(m), inout(f), x1( 0ULL ), x2( 0xFFFFFFFFFFFFFFFFULL ), x( 0ULL )
    {
	if (mode==DECOMPRESS) {  // x = first 8 bytes of inout
	    for (int i=0; i<8; ++i)
	    x = (x<<8) + (getc(inout)&255);
	}
    }

    int code( int y, unsigned int p )
    {
	assert(p>=0 && p<65536);
	p += p<32768;
	uint64_t xmid = x1 + (x2-x1>>16)*p + ((x2-x1&0xffff)*p>>16);
	assert(xmid>=x1 && xmid<x2);

	if (mode==DECOMPRESS) y = x<=xmid;
	if (y) x2 = xmid;
	else x1 = xmid+1;

	while (((x1^x2)&0xff00000000000000L)==0) {  // pass equal leading bytes of range
	    if (mode==COMPRESS) putc(x2>>56, inout);
	    x1<<=8;
	    x2=(x2<<8)+255;
	    if (mode==DECOMPRESS) x=(x<<8)+getc(inout);
	}

	return y;
    }

    void flush()
    {
	if (mode==COMPRESS)
	{
	    putc(x2>>56, inout);
	    putc(255, inout);
	    putc(255, inout);
	    putc(255, inout);
	    putc(255, inout);
	    putc(255, inout);
	    putc(255, inout);
	    putc(255, inout);
	}
    }
};
