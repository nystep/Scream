#pragma once

namespace SCore {

	// TODO: introduce strongly typed IDs for float and uint arrays, as
	// currently one float array ID can be used as the ID of an Uint array.
	class DataBuffer
	{
	private:

		//typedef struct { float *f; float minf, maxf; unsigned int *q; unsigned int sz, cur, bits; } floatArray;
		typedef struct { float *f; float minf, maxf; unsigned int qid, sz, cur, bits; } floatArray;
		typedef struct { unsigned int *u; unsigned int sz, cur; } uintArray;

		std::vector<floatArray*> floatArrays;
		std::vector<uintArray*> uintArrays;

	public:

		DataBuffer() : floatArrays(), uintArrays() {}
		~DataBuffer() {}

		// allocation & misc operations
		unsigned int newFloatArray( unsigned int nElements );
		unsigned int newUintArray( unsigned int nElements );
		void deleteFloatArray( unsigned int id );
		void deleteUintArray( unsigned int id );
		void resetFloatArray( unsigned int id );
		void resetUintArray( unsigned int id );
		unsigned int getFloatArraySz( unsigned int id );
		unsigned int getUintArraySz( unsigned int id );

		// serialisation
		void writeQuantizedArray( unsigned int id, iobFile *outfile );
		void writeUintArray( unsigned int id, iobFile *outfile );
		void latticeWriteUintArray( unsigned int id, iobFile *outfile );
		unsigned int readQuantizedArray( iobFile *infile );
		unsigned int readUintArray( iobFile *infile );
		unsigned int latticeReadUintArray( iobFile *infile );

		// access
		void addFloat( unsigned int id, float f );
		void addUint( unsigned int id, unsigned int u );
		float getFloat( unsigned int id, unsigned int indice );
		unsigned int getUint( unsigned int id, unsigned int indice );
		float getMin( unsigned int id );
		float getMax( unsigned int id );

		// helper
		void calculateFloatQuantisationErrors( unsigned int id, float errorArray[32] );
		float quantizeFloatArray( unsigned int id, unsigned int precisionBits );
		float quantizeEpsilonFloatArray( unsigned int id, float eps );
		unsigned int getSortedIndicesFromUintArray( unsigned int id );
		unsigned int getSortedInjectionIndicesFromArray( unsigned int id );
		void deltaPackUintArray( unsigned int id );
		void deltaUnpackUintArray( unsigned int id );


		unsigned int getChf( const char *str ); // 32bits hash value out of a string

		// test
		float compareArraysMeanError( unsigned int id1, unsigned int id2 );
		unsigned int compareArraysNumberOfErrors( unsigned int id1, unsigned int id2 );

	};

}
