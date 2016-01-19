#pragma once

namespace SCore {

	// TODO: rewrite using C++ binary IO.
	// TODO: rule of 3 constructors.
	// TODO: use a strongly typed enum for the io mode.
	class iobFile
	{
	protected:

		size_t fsz;
		FILE *iobF;

	public:

		iobFile( const char *filename, bool input );
		~iobFile();

		void rewind();
		bool eof() { return unsigned(ftell(iobF)) >= fsz; }

		void readUC( unsigned char &uc );
		void readC( char &c );
		void readUS( unsigned short &us );
		void readS( short &s );
		void readUI( unsigned int &ui );
		void readI( int &i );
		void readUL( uint64_t &ul );
		void readL( int64_t &l );
		void readF( float &f );
		void readD( double &d );

		void readStr( char **str );
		void readUBArray( unsigned char *ucb, unsigned int len );
		void readUSArray( unsigned short *ucs, unsigned int len );
		void readUIArray( unsigned int *uci, unsigned int len );

		void writeUC( unsigned char uc );
		void writeC( char c );
		void writeUS( unsigned short us );
		void writeS( short s );
		void writeUI( unsigned int us );
		void writeI( int s );
		void writeUL( uint64_t us );
		void writeL( int64_t s );
		void writeF( float s );
		void writeD( double s );

		void writeStr( const char *str );
		void writeUBArray( unsigned char *ucb, unsigned int len );
		void writeUSArray( unsigned short *ucs, unsigned int len );
		void writeUIArray( unsigned int *uci, unsigned int len );

	};

}
