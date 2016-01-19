
#include "screamCore.h"
#include "radixSort.h"


namespace SCore {

	unsigned int DataBuffer::newFloatArray( unsigned int nElements )
	{
		unsigned int i=0;
		unsigned int rv = (unsigned int) floatArrays.size();
		while ( i < ((unsigned int) floatArrays.size()) && floatArrays[i] != NULL) i++;
		if ( i == (unsigned int) floatArrays.size() ) floatArrays.push_back( NULL );
		else rv = i;
		floatArrays[rv] = new floatArray;
		floatArrays[rv]->f = new float [nElements];
		//floatArrays[rv]->q = new unsigned int [nElements];
		floatArrays[rv]->qid = newUintArray( nElements );
		floatArrays[rv]->sz = nElements;
		floatArrays[rv]->cur = 0;
		floatArrays[rv]->bits = 0;
		return rv;
	}

	unsigned int DataBuffer::newUintArray( unsigned int nElements )
	{
		unsigned int i = 0;
		unsigned int rv = (unsigned int) uintArrays.size();
		while ( i < ((unsigned int) uintArrays.size()) && uintArrays[i] != NULL) i++;
		if ( i == (unsigned int) uintArrays.size() ) uintArrays.push_back( NULL );
		else rv = i;
		uintArrays[rv] = new uintArray;
		uintArrays[rv]->u = new unsigned int [nElements];
		uintArrays[rv]->sz = nElements;
		uintArrays[rv]->cur = 0;
		return rv;
	}

	void DataBuffer::deleteFloatArray( unsigned int id )
	{
		delete [] floatArrays[id]->f;
		deleteUintArray( floatArrays[id]->qid );
		//delete [] floatArrays[id]->q;
		delete floatArrays[id];
		floatArrays[id] = NULL;
	}

	void DataBuffer::deleteUintArray( unsigned int id )
	{
		delete [] uintArrays[id]->u;
		delete uintArrays[id];
		uintArrays[id] = NULL;
	}

	unsigned int DataBuffer::getFloatArraySz( unsigned int id )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );
		return floatArrays[id]->sz;
	}

	unsigned int DataBuffer::getUintArraySz( unsigned int id )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		return uintArrays[id]->sz;
	}

	void DataBuffer::addFloat( unsigned int id, float f )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );
		if (floatArrays[id]->cur >= floatArrays[id]->sz) return;

		if (floatArrays[id]->cur == 0)
			floatArrays[id]->minf = floatArrays[id]->maxf = f;
		else
		{
			floatArrays[id]->minf = f < floatArrays[id]->minf ? f : floatArrays[id]->minf;
			floatArrays[id]->maxf = f > floatArrays[id]->maxf ? f : floatArrays[id]->maxf;
		}

		floatArrays[id]->f[ floatArrays[id]->cur++ ] = f;
	}

	void DataBuffer::addUint( unsigned int id, unsigned int u )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		if (uintArrays[id]->cur >= uintArrays[id]->sz) return;
		uintArrays[id]->u[ uintArrays[id]->cur++ ] = u;
	}

	unsigned int DataBuffer::getUint( unsigned int id, unsigned int indice )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		if (uintArrays[id]->sz <= indice) return 0xFFFFFFFF;
		else return uintArrays[id]->u[indice];
	}

	float DataBuffer::getFloat( unsigned int id, unsigned int indice )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );
		if (floatArrays[id]->sz <= indice) return 0.0f;
		else return floatArrays[id]->f[indice];
	}

	float DataBuffer::getMin( unsigned int id )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		return floatArrays[id]->minf;
	}

	float DataBuffer::getMax( unsigned int id )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		return floatArrays[id]->maxf;
	}

	void DataBuffer::resetFloatArray( unsigned int id )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );
		floatArrays[id]->cur = 0;
	}

	void DataBuffer::resetUintArray( unsigned int id )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		uintArrays[id]->cur = 0;
	}

	float DataBuffer::quantizeFloatArray( unsigned int id, unsigned int precisionBits )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );
		assert( precisionBits < 32 );
		assert( precisionBits > 1 );

		floatArrays[id]->bits = precisionBits;

		double error = 0.0;
		double shift = (double) (1 << precisionBits);
		double interval = double(floatArrays[id]->maxf - floatArrays[id]->minf);
		double invShift = 1.0 / shift;
		double invInterval = 1.0 / interval;

		for (unsigned int i=0; i<floatArrays[id]->sz; i++)
		{
			double normf = double(floatArrays[id]->f[i] - floatArrays[id]->minf) * invInterval;
			unsigned int quantized = unsigned(normf * shift);
			//floatArrays[id]->q[i] = unsigned(normf * shift);
			addUint( floatArrays[id]->qid, quantized );
			error += fabs( floatArrays[id]->f[i] - (quantized*invShift*interval + floatArrays[id]->minf) );
		}

		return float(error);
	}

	void DataBuffer::calculateFloatQuantisationErrors( unsigned int id, float errorArray[32] )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );

		for (unsigned int i=1; i<32; i++)
			errorArray[i] = quantizeFloatArray( id, i );

		errorArray[0] = 1.e20f;
	}

	float DataBuffer::quantizeEpsilonFloatArray( unsigned int id, float eps )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );

		double ratio = ((double) floatArrays[id]->maxf - floatArrays[id]->minf) / eps;

		unsigned int precisionBits = (unsigned int) (- log(ratio) / log(2.0) + 0.5);
		if (precisionBits<2) precisionBits=2;
		if (precisionBits>31) precisionBits=31;

		return quantizeFloatArray( id, precisionBits );
	}

	void DataBuffer::writeQuantizedArray( unsigned int id, iobFile *outfile )
	{
		assert( id < ((unsigned int) floatArrays.size()) );
		assert( floatArrays[id] != NULL );
		assert( outfile != NULL );

		outfile->writeUC( floatArrays[id]->bits );
		outfile->writeF( floatArrays[id]->minf );
		outfile->writeF( floatArrays[id]->maxf );
		//outfile->writeUI( floatArrays[id]->sz );
		writeUintArray( floatArrays[id]->qid, outfile );
	}

	void DataBuffer::writeUintArray( unsigned int id, iobFile *outfile )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		assert( outfile != NULL );

		outfile->writeUIArray( uintArrays[id]->u, uintArrays[id]->sz );
	}

	unsigned int DataBuffer::getChf( const char *str )
	{
		unsigned int crc = 12372591;
		unsigned int len = strlen( str );
		for (unsigned int i=0; i<len; i++)
			crc = crc ^ ((((unsigned int) str[i] + 9131731) * crc) + 1913173731);
		return crc;
	}

	unsigned int DataBuffer::getSortedIndicesFromUintArray( unsigned int id )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );

		unsigned int ri = newUintArray( uintArrays[id]->sz );
		radixSort( uintArrays[id]->u, uintArrays[ri]->u, uintArrays[id]->sz );

		return ri;
	}

	unsigned int DataBuffer::getSortedInjectionIndicesFromArray( unsigned int id )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );

		unsigned int ri = newUintArray( uintArrays[id]->sz );

		for (unsigned int i=0; i<uintArrays[id]->sz; i++)
		{
			if (uintArrays[id]->u[i] < uintArrays[id]->sz) uintArrays[ri]->u[ uintArrays[id]->u[i] ] = i;
			else uintArrays[ri]->u[i] = 0xFFFFFFFF;
		}

		return ri;
	}

	void DataBuffer::deltaPackUintArray( unsigned int id )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		assert( uintArrays[id]->sz > 0 );

		unsigned int prev = uintArrays[id]->u[0];

		for (unsigned int i=1; i<uintArrays[id]->sz; i++)
		{
			unsigned int cur = uintArrays[id]->u[i];
			uintArrays[id]->u[i] = cur - prev;
			prev = cur;
		}
	}

	void DataBuffer::deltaUnpackUintArray( unsigned int id )
	{
		assert( id < ((unsigned int) uintArrays.size()) );
		assert( uintArrays[id] != NULL );
		assert( uintArrays[id]->sz > 0 );

		unsigned int prev = 0;

		for (unsigned int i=0; i<uintArrays[id]->sz; i++)
		{
			uintArrays[id]->u[i] += prev;
			prev = uintArrays[id]->u[i];
		}
	}

	unsigned int DataBuffer::readQuantizedArray( iobFile *infile )
	{
		unsigned int sz, ri;
		unsigned char bits;
		float minf, maxf;

		infile->readUC( bits );
		infile->readF( minf );
		infile->readF( maxf );
		infile->readUI( sz );

		ri = newFloatArray( sz );

		infile->readUIArray( uintArrays[floatArrays[ri]->qid]->u, sz );

		floatArrays[ri]->minf = minf;
		floatArrays[ri]->maxf = maxf;
		floatArrays[ri]->bits = bits;

		double intervalScale = double(floatArrays[ri]->maxf - floatArrays[ri]->minf) / double(1 << bits);

		for (unsigned int i=0; i<floatArrays[ri]->sz; i++)
		{
			double normf = double(getUint( floatArrays[ri]->qid, i )*intervalScale + floatArrays[ri]->minf);
			floatArrays[ri]->f[i] = float(normf);
		}

		return ri;
	}

	float DataBuffer::compareArraysMeanError( unsigned int id1, unsigned int id2 )
	{
		double err = 0.0;

		assert( id1 < ((unsigned int) floatArrays.size()) );
		assert( id2 < ((unsigned int) floatArrays.size()) );

		if (floatArrays[id1]->sz != floatArrays[id2]->sz) return 1.0e20f;

		double invNorm = 1.0 / double(floatArrays[id1]->maxf - floatArrays[id1]->minf);

		for (unsigned int i=0; i<floatArrays[id1]->sz; i++)
		{
			err += fabs( floatArrays[id1]->f[i] - floatArrays[id2]->f[i] ) * invNorm;
		}

		return float(err / double(floatArrays[id1]->sz));
	}

	unsigned int DataBuffer::compareArraysNumberOfErrors( unsigned int id1, unsigned int id2 )
	{
		unsigned int err = 0;

		assert( id1 < ((unsigned int) floatArrays.size()) );
		assert( id2 < ((unsigned int) floatArrays.size()) );

		if (floatArrays[id1]->sz != floatArrays[id2]->sz) return floatArrays[id1]->sz + floatArrays[id2]->sz;

		for (unsigned int i=0; i<floatArrays[id1]->sz; i++)
		{
			err += getUint(floatArrays[id1]->qid,i) != getUint(floatArrays[id2]->qid,i) ? 1 : 0;
		}

		return err;
	}

	unsigned int DataBuffer::readUintArray( iobFile *infile )
	{
		unsigned int sz, ri;

		infile->readUI( sz );
		ri = newUintArray( sz );
		infile->readUIArray( uintArrays[ri]->u, sz );

		return ri;
	}

	void DataBuffer::latticeWriteUintArray( unsigned int id, iobFile *outfile )
	{
		assert( id < ((unsigned int) uintArrays.size()) );

		unsigned int outand = newUintArray( uintArrays[id]->sz );
		unsigned int outor = newUintArray( uintArrays[id]->sz );

		unsigned int prev = 0;

		for (unsigned int i=0; i<uintArrays[id]->sz; i++)
		{
			addUint( outand, uintArrays[id]->u[i] & prev );
			addUint( outor, uintArrays[id]->u[i] | prev );
			prev = uintArrays[id]->u[i];
		}

		outfile->writeUIArray( uintArrays[outand]->u, uintArrays[outand]->sz );
		outfile->writeUIArray( uintArrays[outor]->u, uintArrays[outor]->sz );

		deleteUintArray( outand );
		deleteUintArray( outor );
	}

	unsigned int DataBuffer::latticeReadUintArray( iobFile *infile )
	{
		unsigned int sz, inand, inor;

		infile->readUI( sz );
		inand = newUintArray( sz );
		infile->readUIArray( uintArrays[inand]->u, sz );

		infile->readUI( sz );
		inor = newUintArray( sz );
		infile->readUIArray( uintArrays[inor]->u, sz );

		unsigned int returnid = newUintArray( sz );
		unsigned int prev = 0;

		for (unsigned int i=0; i<sz; i++)
		{
			unsigned int iand = getUint( inand, i );
			unsigned int ior = getUint( inor, i );
			unsigned int b = (prev & iand) | (~prev & ior);

			addUint( returnid, b );
			prev = b;
		}

		deleteUintArray( inand );
		deleteUintArray( inor );

		return returnid;
	}

};
