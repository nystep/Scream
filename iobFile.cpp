#include "screamCore.h"

namespace SCore {

	using namespace SCore;

	iobFile::iobFile( const char *filename, bool input )
	{
		//fopen_s( &iobF, filename, input ? "rb" : "wb" );
		iobF = fopen( filename, input ? "rb" : "wb" );
		assert( iobF != NULL );
		if (input)
		{
			fseek( iobF, 0, SEEK_END );
			fsz = ftell( iobF );
			::rewind( iobF );
		}
	}

	iobFile::~iobFile()
	{
		assert( iobF != NULL );
		fclose( iobF );
	}

	void iobFile::rewind()
	{
		::rewind( iobF );
	}


	void iobFile::readUC( unsigned char &uc )
	{
		int c = fgetc( iobF );
		assert( c != EOF );
		uc = c & 0xFF;
	}

	void iobFile::readC( char &c )
	{
		int ch = fgetc( iobF );
		assert( ch != EOF );
		c = ch & 0xFF;
	}

	void iobFile::readUS( unsigned short &us )
	{
		unsigned char low, high;
		readUC( low );
		readUC( high );
		us = low + (high << 8);
	}

	void iobFile::readS( short &s )
	{
		unsigned char low, high;
		readUC( low );
		readUC( high );
		s = low + (high << 8);
	}

	void iobFile::readUI( unsigned int &ui )
	{
		unsigned char ub0, ub1, ub2, ub3;
		readUC( ub0 );
		readUC( ub1 );
		readUC( ub2 );
		readUC( ub3 );
		ui = ub0 + (ub1 << 8) + (ub2 << 16) + (ub3 << 24);
	}

	void iobFile::readI( int &i )
	{
		unsigned char ub0, ub1, ub2, ub3;
		readUC( ub0 );
		readUC( ub1 );
		readUC( ub2 );
		readUC( ub3 );
		i = ub0 + (ub1 << 8) + (ub2 << 16) + (ub3 << 24);
	}

	void iobFile::readUL( uint64_t &ul )
	{
		unsigned char ub0, ub1, ub2, ub3, ub4, ub5, ub6, ub7;
		readUC( ub0 );
		readUC( ub1 );
		readUC( ub2 );
		readUC( ub3 );
		readUC( ub4 );
		readUC( ub5 );
		readUC( ub6 );
		readUC( ub7 );

		ul = ub0 + (ub1 << 8) + (ub2 << 16) + (ub3 << 24) +
			(uint64_t(ub4) << 32) + (uint64_t(ub5) << 40) + (uint64_t(ub6) << 48) + (uint64_t(ub7) << 56);
	}

	void iobFile::readL( int64_t &l )
	{
		unsigned char ub0, ub1, ub2, ub3, ub4, ub5, ub6, ub7;
		readUC( ub0 );
		readUC( ub1 );
		readUC( ub2 );
		readUC( ub3 );
		readUC( ub4 );
		readUC( ub5 );
		readUC( ub6 );
		readUC( ub7 );

		l = ub0 + (ub1 << 8) + (ub2 << 16) + (ub3 << 24) +
			(int64_t(ub4) << 32) + (int64_t(ub5) << 40) + (int64_t(ub6) << 48) + (int64_t(ub7) << 56);
	}

	void iobFile::readF( float &f )
	{
		int i;
		readI( i );
		f = *((float*) &i);
	}

	void iobFile::readD( double &d )
	{
		int64_t l;
		readL( l );
		d = *((double*) &l);
	}

	void iobFile::readStr( char **str )
	{
		unsigned int len;
		readUI( len );
		//printf( "iobFile::readStr len=%u\n", len );
		if (len > (1<<22)) return;
		*str = (char*) malloc( len+1 );
		if (!*str) return;
		for (unsigned int i=0; i<len; i++)
			readC( (*str)[i] );
		(*str)[len] = 0;
	}

	void iobFile::readUBArray( unsigned char *ucb, unsigned int len )
	{
		if (len == 0) return;

		for (unsigned int i=0; i<len; i++)
			ucb[i] = 0;

		unsigned int len8 = (len >> 3) + 1;

		for (unsigned int b=0; b<8; b++)
		{
			for (unsigned int i=0; i<len8; i++)
			{
				unsigned int ind = i<<3;
				unsigned char bitsGather;

				readUC( bitsGather );

				for (unsigned int j=0; ((ind+j) < len) && (j<8); j++)
				{
					ucb[ind+j] |= ((bitsGather>>j)&1) << b;
				}
			}
		}
	}

	void iobFile::readUSArray( unsigned short *ucs, unsigned int len )
	{
		if (len == 0) return;

		for (unsigned int i=0; i<len; i++)
			ucs[i] = 0;

		unsigned int len8 = (len >> 3) + 1;

		for (unsigned int b=0; b<16; b++)
		{
			for (unsigned int i=0; i<len8; i++)
			{
				unsigned int ind = i<<3;
				unsigned char bitsGather;

				readUC( bitsGather );

				for (unsigned int j=0; ((ind+j) < len) && (j<8); j++)
				{
					ucs[ind+j] |= ((bitsGather>>j)&1) << b;
				}
			}
		}
	}

	void iobFile::readUIArray( unsigned int *uci, unsigned int len )
	{
		if (len == 0) return;

		for (unsigned int i=0; i<len; i++)
			uci[i] = 0;

		unsigned int len8 = (len >> 3) + 1;

		for (unsigned int b=0; b<32; b++)
		{
			for (unsigned int i=0; i<len8; i++)
			{
				unsigned int ind = i<<3;

				unsigned char bitsGather;
				readUC( bitsGather );

				for (unsigned int j=0; ((ind+j) < len) && (j<8); j++)
				{
					uci[ind+j] |= ((bitsGather>>j)&1) << b;
				}
			}
		}
	}


	void iobFile::writeUC( unsigned char uc )
	{
		assert( iobF != NULL );
		fputc( uc, iobF );
	}

	void iobFile::writeC( char c )
	{
		assert( iobF != NULL );
		fputc( c, iobF );
	}

	void iobFile::writeUS( unsigned short us )
	{
		writeUC( ((unsigned char) us & 0xFF) );
		writeUC( ((unsigned char) (us>>8) & 0xFF) );
	}

	void iobFile::writeS( short s )
	{
		writeUC( ((unsigned char) s & 0xFF) );
		writeUC( ((unsigned char) (s>>8) & 0xFF) );
	}

	void iobFile::writeUI( unsigned int us )
	{
		writeUC( ((unsigned char) us & 0xFF) );
		writeUC( ((unsigned char) (us>>8) & 0xFF) );
		writeUC( ((unsigned char) (us>>16) & 0xFF) );
		writeUC( ((unsigned char) (us>>24) & 0xFF) );
	}

	void iobFile::writeI( int s )
	{
		writeUC( ((unsigned char) s & 0xFF) );
		writeUC( ((unsigned char) (s>>8) & 0xFF) );
		writeUC( ((unsigned char) (s>>16) & 0xFF) );
		writeUC( ((unsigned char) (s>>24) & 0xFF) );
	}

	void iobFile::writeUL( uint64_t us )
	{
		writeUC( ((unsigned char) us & 0xFF) );
		writeUC( ((unsigned char) (us>>8) & 0xFF) );
		writeUC( ((unsigned char) (us>>16) & 0xFF) );
		writeUC( ((unsigned char) (us>>24) & 0xFF) );
		writeUC( ((unsigned char) (us>>32) & 0xFF) );
		writeUC( ((unsigned char) (us>>40) & 0xFF) );
		writeUC( ((unsigned char) (us>>48) & 0xFF) );
		writeUC( ((unsigned char) (us>>56) & 0xFF) );
	}

	void iobFile::writeL( int64_t s )
	{
		writeUC( ((unsigned char) s & 0xFF) );
		writeUC( ((unsigned char) (s>>8) & 0xFF) );
		writeUC( ((unsigned char) (s>>16) & 0xFF) );
		writeUC( ((unsigned char) (s>>24) & 0xFF) );
		writeUC( ((unsigned char) (s>>32) & 0xFF) );
		writeUC( ((unsigned char) (s>>40) & 0xFF) );
		writeUC( ((unsigned char) (s>>48) & 0xFF) );
		writeUC( ((unsigned char) (s>>56) & 0xFF) );
	}

	void iobFile::writeF( float s )
	{
		writeUI( *((unsigned int *) &s) );
	}

	void iobFile::writeD( double s )
	{
		writeUL( *((uint64_t *) &s) );
	}


	void iobFile::writeStr( const char *str )
	{
		unsigned int len = (unsigned int) strlen( str );
		writeUI( len );
		for (unsigned int i=0; i<len; i++) writeC( str[i] );
	}

	void iobFile::writeUBArray( unsigned char *ucb, unsigned int len )
	{
		if (len == 0) return;

		unsigned int len8 = (len >> 3) + 1;
		writeUI( len );

		for (unsigned int b=0; b<8; b++)
		{
			unsigned int maskb = 1 << b;

			for (unsigned int i=0; i<len8; i++)
			{
				unsigned int ind = i << 3;
				unsigned char bitsGather = 0;

				for (unsigned int j=0; ((ind+j) < len) && (j<8); j++)
				{
					bitsGather |= ( (ucb[ind+j] & maskb) >> b ) << j;
				}

				writeUC( bitsGather );
			}
		}
	}

	void iobFile::writeUSArray( unsigned short *ucs, unsigned int len )
	{
		if (len == 0) return;

		unsigned int len8 = (len >> 3) + 1;
		writeUI( len );

		for (unsigned int b=0; b<16; b++)
		{
			unsigned int maskb = 1 << b;

			for (unsigned int i=0; i<len8; i++)
			{
				unsigned int ind = i << 3;
				unsigned char bitsGather = 0;

				for (unsigned int j=0; ((ind+j) < len) && (j<8); j++)
				{
					bitsGather |= ( (ucs[ind+j] & maskb) >> b ) << j;
				}

				writeUC( bitsGather );
			}
		}
	}

	void iobFile::writeUIArray( unsigned int *uci, unsigned int len )
	{
		if (len == 0) return;

		unsigned int len8 = (len >> 3) + 1;
		writeUI( len );

		for (unsigned int b=0; b<32; b++)
		{
			unsigned int maskb = 1 << b;

			for (unsigned int i=0; i<len8; i++)
			{
				unsigned int ind = i << 3;
				unsigned char bitsGather = 0;

				for (unsigned int j=0; ((ind+j) < len) && (j<8); j++)
				{
					bitsGather |= ( (uci[ind+j] & maskb) >> b ) << j;
				}

				writeUC( bitsGather );
			}
		}
	}

};
