#include "libssemath.h"


namespace ssemath
{

	// some internal constants... this way it is set only once at program startup.
	// ugly but a set1 can be more expensive than a mul on a core2, for example...
	const __m128 _mm_half = _mm_set1_ps( 0.5f );
	const __m128 _mm_three = _mm_set1_ps( 3.f );
	const __m128 _mmi_msign = _mm_set1i_ps( 0x7FFFFFFF );
	static const __m128 _mm_one = _mm_set1_ps( 1.0f );
	static const __m128 _mm_zero = _mm_set1_ps( 0.0f );
	static const __m128 _mmi_minusone = _mm_set1i_ps( -1 );

	// some more oddity ;)
	typedef union {
		__m128 f;
		__m128i i;
	} SSEDirtyTypeCast;


	/*****************************************************************************/
	// Atan

	static const __m128 sq2p1	 = _mm_set1_ps( 2.414213562373095048802e0f );
	static const __m128 sq2m1	 = _mm_set1_ps( .414213562373095048802e0f );
	static const __m128 pio2	 = _mm_set1_ps( 1.570796326794896619231e0f );
	static const __m128 pio4	 = _mm_set1_ps( .785398163397448309615e0f );
	static const __m128 p4	 = _mm_set1_ps( .161536412982230228262e2f );
	static const __m128 p3	 = _mm_set1_ps( .26842548195503973794141e3f );
	static const __m128 p2	 = _mm_set1_ps( .11530293515404850115428136e4f );
	static const __m128 p1	 = _mm_set1_ps( .178040631643319697105464587e4f );
	static const __m128 p0	 = _mm_set1_ps( .89678597403663861959987488e3f );
	static const __m128 q4	 = _mm_set1_ps( .5895697050844462222791e2f );
	static const __m128 q3	 = _mm_set1_ps( .536265374031215315104235e3f );
	static const __m128 q2	 = _mm_set1_ps( .16667838148816337184521798e4f );
	static const __m128 q1	 = _mm_set1_ps( .207933497444540981287275926e4f );
	static const __m128 q0	 = _mm_set1_ps( .89678597403663861962481162e3f );

	__m128 _mm_atan_ps( __m128 arg )
	{
		const __m128 complement = _mm_cmplt_ps( arg, _mm_zero );

		arg = _mm_select_ps( _mm_sub_ps( _mm_zero, arg ), arg, complement );

		const __m128 fsector = _mm_cmpgt_ps( arg, sq2p1 );
		const __m128 ssector = _mm_andnot_ps( _mm_cmpge_ps( arg, sq2m1 ), fsector );
		
		arg = _mm_select_ps( _mm_div_ps( _mm_one, arg ), arg, fsector );
		arg = _mm_select_ps( _mm_div_ps( _mm_sub_ps( arg, _mm_one ), _mm_add_ps( arg, _mm_one ) ), arg, ssector );

		const __m128 argsq = _mm_mul_ps( arg, arg );
		__m128 numerator = _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( p4, argsq ), p3 ), argsq ), p2 ), argsq ), p1 ), argsq ), p0 );
		__m128 denominator = _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( argsq, q4 ), argsq ), q3 ), argsq ), q2 ), argsq ), q1 ), argsq ), q0 );
		__m128 arctangent = _mm_mul_ps( _mm_div_ps( numerator, denominator ), arg );

		arctangent = _mm_select_ps( _mm_sub_ps( pio2, arctangent ), arctangent, fsector );
		arctangent = _mm_select_ps( _mm_add_ps( pio4, arctangent ), arctangent, ssector );
		return _mm_select_ps( _mm_sub_ps( _mm_zero, arctangent ), arctangent, complement );
	}

	/*****************************************************************************/
	// Acos

	__m128 _mm_acos_ps( __m128 arg )
	{
		arg = _mm_div_ps( _mm_sqrt_ps( _mm_sub_ps( _mm_one, _mm_mul_ps( arg, arg ) ) ), _mm_add_ps( _mm_one, arg ) );
		const __m128 res = _mm_atan_ps( arg );
		return _mm_add_ps( res, res );
	}

	/*****************************************************************************/
	// Asin

	__m128 _mm_asin_ps( __m128 arg )
	{
		arg = _mm_div_ps( arg, 
			_mm_add_ps( _mm_sqrt_ps( _mm_sub_ps( _mm_one, _mm_mul_ps( arg, arg ) ) ), _mm_one ) );
		const __m128 res = _mm_atan_ps( arg );
		return _mm_add_ps( res, res );
	}

	/*****************************************************************************/
	// Fast trigonometric functions, but these are getting inaccurate with big angles.

	static const __m128 twopi = _mm_set1_ps( 6.283185307179586476925286766559f );
	static const __m128 itwopi = _mm_set1_ps( 0.15915494309189533576888376337251f );
	static const __m128 pi = _mm_set1_ps( 3.1415926535897932384626433832795f );
	static const __m128 pis2 = _mm_set1_ps( 1.5707963267948966192313216916398f );
	static const __m128 oo6 = _mm_set1_ps( 0.16666666666666666666666666666667f );
	static const __m128 oo120 = _mm_set1_ps( 0.0083333333333333333333333333333333f );
	static const __m128 oo5040 = _mm_set1_ps( 0.0001984126984126984126984126984127f );
	static const __m128 oo362880 = _mm_set1_ps( 2.7557319223985890652557319223986e-6f );
	static const __m128 oo39916800 = _mm_set1_ps( 2.5052108385441718775052108385442e-8f );
	static const __m128 oo6227020800 = _mm_set1_ps( 1.6059043836821614599392377170155e-10f );


	__m128 _mm_fastabssin_ps( __m128 x )
	{
		x = _mm_select_ps( _mm_sub_ps( _mm_zero, x ), x, _mm_cmplt_ps( x, _mm_zero ) );

		const __m128 div = _mm_cvtepi32_ps( _mm_cvttps_epi32( _mm_mul_ps( x, itwopi ) ) );
		x = _mm_sub_ps( x, _mm_mul_ps( div, twopi ) );

		x = _mm_select_ps( _mm_sub_ps( x, pi ), x, _mm_cmpgt_ps( x, pi ) );
		x = _mm_select_ps( _mm_sub_ps( x, pi ), x, _mm_cmpgt_ps( x, pis2 ) );

		const __m128 x2 = _mm_mul_ps( x, x );

		__m128 numerat = x;
		__m128 sine = numerat;

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_sub_ps( sine, _mm_mul_ps( numerat, oo6 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_add_ps( sine, _mm_mul_ps( numerat, oo120 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_sub_ps( sine, _mm_mul_ps( numerat, oo5040 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_add_ps( sine, _mm_mul_ps( numerat, oo362880 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_sub_ps( sine, _mm_mul_ps( numerat, oo39916800 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_add_ps( sine, _mm_mul_ps( numerat, oo6227020800 ) );

		return sine;
	}


	__m128 _mm_fastsin_ps( __m128 x )
	{
		// we abuse symetry.
		const __m128 complement = _mm_cmplt_ps( x, _mm_zero );
		x = _mm_select_ps( _mm_sub_ps( _mm_zero, x ), x, complement );

		// modulo 2 pi.
		const __m128 div = _mm_cvtepi32_ps( _mm_cvttps_epi32( _mm_mul_ps( x, itwopi ) ) );
		x = _mm_sub_ps( x, _mm_mul_ps( div, twopi ) );

		// come to [0..pi]
		const __m128 red2pi = _mm_cmpgt_ps( x, pi );
		x = _mm_select_ps( _mm_sub_ps( x, pi ), x, red2pi );

		// come to [-pi/2..pi/2]
		const __m128 complementpi2 = _mm_cmpgt_ps( x, pis2 );
		x = _mm_select_ps( _mm_sub_ps( x, pi ), x, complementpi2 );

		const __m128 x2 = _mm_mul_ps( x, x );

		__m128 numerat = x;
		__m128 sine = numerat;

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_sub_ps( sine, _mm_mul_ps( numerat, oo6 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_add_ps( sine, _mm_mul_ps( numerat, oo120 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_sub_ps( sine, _mm_mul_ps( numerat, oo5040 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_add_ps( sine, _mm_mul_ps( numerat, oo362880 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_sub_ps( sine, _mm_mul_ps( numerat, oo39916800 ) );

		numerat = _mm_mul_ps( numerat, x2 );
		sine = _mm_add_ps( sine, _mm_mul_ps( numerat, oo6227020800 ) );

		// adjust sign..
		sine = _mm_select_ps( _mm_sub_ps( _mm_zero, sine ), sine, complementpi2 );
		sine = _mm_select_ps( _mm_sub_ps( _mm_zero, sine ), sine, red2pi );
		sine = _mm_select_ps( _mm_sub_ps( _mm_zero, sine ), sine, complement );

		return sine;
	}

	// Cos...
	__m128 _mm_fastcos_ps( const __m128 x )
	{
		return _mm_fastsin_ps( _mm_add_ps( x, pis2 ) );
	}


	void _mm_fastabssincos_ps( const __m128 x, __m128* y, __m128* z )
	{
		__m128 abssine = _mm_fastabssin_ps( x );
		abssine = _mm_select_ps( _mm_sub_ps( _mm_zero, abssine ), abssine, _mm_cmplt_ps( abssine, _mm_zero ) );
		*y = abssine;
		*z = _mm_sqrt_ps( _mm_sub_ps( _mm_one, _mm_mul_ps( abssine, abssine ) ) );
	}

	static const __m128 ipis2 = _mm_set1_ps( 0.63661977236758134307553505349006f );
	static const __m128i intone = _mm_set1_epi32( 1 );

	// Tan
	__m128 _mm_fasttan_ps( const __m128 x )
	{
		SSEDirtyTypeCast mask;
		__m128 sine, cosine;

		_mm_fastabssincos_ps( x, &sine, &cosine );

		const __m128i quart = _mm_cvttps_epi32( _mm_mul_ps( x, ipis2 ) );
		mask.i = _mm_cmpeq_epi32( _mm_and_si128( quart, intone ), intone );

		const __m128 tangent = _mm_div_ps( sine, cosine );

		return _mm_select_ps( _mm_sub_ps( _mm_zero, tangent ), tangent, mask.f );
	}

	/*****************************************************************************/
	// modf

	static const __m128i exponentmask = _mm_set1_epi32( 0xFF );
	static const __m128i exponentbias = _mm_set1_epi32( 127 );
	static const __m128i modfzero = _mm_set1_epi32( 0 );
	static const __m128i modfone = _mm_set1_epi32( 1 );
	static const __m128i modf23 = _mm_set1_epi32( 23 );

	__m128 _mm_modf_ps( __m128 x, __m128 *intpart )
	{
		SSEDirtyTypeCast v;
		SSEDirtyTypeCast retmask;
		SSEDirtyTypeCast maskmb;
		__m128 retval;

		v.f = x;

		__m128i exponent = _mm_sub_epi32( _mm_and_si128( _mm_srli_epi32( v.i, 23 ), exponentmask ), exponentbias );
		retmask.i = _mm_cmplt_epi32( exponent, modfzero );
		//retval = x;
		//*intpart = _mm_zero;

		__m128i maskbits = _mm_sub_epi32( modf23, exponent );
		maskmb.i = _mm_cmpgt_epi32( maskbits, modfzero );
		
		retval = _mm_select_ps( _mm_zero, x, maskmb.f );
		*intpart = _mm_select_ps( x, _mm_zero, maskmb.f );
		
		retmask.i = _mm_or_si128( maskmb.i, retmask.i );

		__m128i t;
		t.m128i_i32[0] = 1 << (maskbits.m128i_i32[0]);
		t.m128i_i32[1] = 1 << (maskbits.m128i_i32[1]);
		t.m128i_i32[2] = 1 << (maskbits.m128i_i32[2]);
		t.m128i_i32[3] = 1 << (maskbits.m128i_i32[3]);

		v.i = _mm_andnot_si128( v.i, _mm_sub_epi32( t, modfone ) );

		*intpart = _mm_select_ps( *intpart, v.f, retmask.f );
		return _mm_select_ps( retval, _mm_sub_ps( x, v.f ), retmask.f );
	}

	/*****************************************************************************/
	// round

	__m128 _mm_round_ps( __m128 x )
	{
		return _mm_zero;
	}

	/*****************************************************************************/
	// fract

	__m128 _mm_fract_ps( __m128 x )
	{
		return _mm_zero;
	}

	/*****************************************************************************/
	// Sine

	__m128 _mm_sin_ps( __m128 x )
	{
		return _mm_zero;
	}

	/*****************************************************************************/
	// cos

	__m128 _mm_cos_ps( __m128 x )
	{
		return _mm_zero;
	}

	/*****************************************************************************/
	// tan

	__m128 _mm_tan_ps( __m128 x )
	{
		return _mm_zero;
	}

	/*****************************************************************************/
	// Ceil

	__m128 _mm_ceil_ps( const __m128 a )
	{
		const __m128 mask = _mm_cmpge_ps( a, _mm_zero );
		const __m128 b = _mm_cvtepi32_ps( _mm_cvttps_epi32( a ) );
		return _mm_select_ps( _mm_add_ps( b, _mm_one ), b, mask );
	}

	/*****************************************************************************/
	// Floor

	__m128 _mm_floor_ps( const __m128 a )
	{
		const __m128 mask = _mm_cmplt_ps( a, _mm_zero );
		const __m128 b = _mm_cvtepi32_ps( _mm_cvttps_epi32( a ) );
		return _mm_select_ps( _mm_sub_ps( b, _mm_one ), b, mask );
	}


	/*****************************************************************************/
	// Ldexp


	static const __m128i msk_exponent = _mm_set1_epi32( 0xFF );
	static const __m128i ldexpzero = _mm_set1_epi32( 0 );
	static const __m128i ldexpone = _mm_set1_epi32( 1 );
	static const __m128i ldexpm24 = _mm_set1_epi32( -24 );
	static const __m128i ldexpmexp = _mm_set1_epi32( 255 );
	static const __m128 ldexpmaxfloat = _mm_set1_ps( 1.7014117331926442990585209174225846272e38f );
	static const __m128i ldexpmaskExp = _mm_set1_epi32( 0x807fffff );


	__m128 _mm_ldexp_ps( const __m128 x, const __m128i pow2 )
	{
		SSEDirtyTypeCast y;
		__m128i e;
		__m128i pw2 = pow2;

		__m128 retval = _mm_zero;
		SSEDirtyTypeCast retmask;

		y.f = x;
		retmask.i = ldexpzero;

		if ( _mm_movemask_epi8( _mm_cmpeq_epi32( e = _mm_and_si128( _mm_srli_epi32( y.i, 23 ), msk_exponent ), ldexpzero ) ) != 0 )
		{
			SSEDirtyTypeCast maskPw2Pos, maskPw2Neg, maskPw2Zero;

			retmask.f = _mm_or_ps( _mm_cmpeq_ps( y.f, _mm_zero ), retmask.f );

			maskPw2Pos.i = _mm_cmpgt_epi32( pw2, ldexpzero );
			_mm_select_epi( _mm_sub_epi32( pw2, ldexpone ), pw2, maskPw2Pos.i );
			_mm_select_ps( _mm_add_ps( y.f, y.f ), y.f, maskPw2Pos.f );

			retmask.i = _mm_or_si128( _mm_cmplt_epi32( pw2, ldexpm24 ), retmask.i );

			maskPw2Neg.i = _mm_cmplt_epi32( pw2, ldexpzero );
			_mm_select_epi( _mm_add_epi32( pw2, ldexpone ), pw2, maskPw2Neg.i );
			_mm_select_ps( _mm_mul_ps( y.f, _mm_half ), y.f, maskPw2Neg.f );

			maskPw2Zero.i = _mm_cmpeq_epi32( pw2, ldexpzero );
			retval = _mm_select_ps( y.f, retval, maskPw2Zero.f );
			retmask.f = _mm_or_ps( retmask.f, maskPw2Zero.f );
		}

		e = _mm_add_epi32( pw2, e );

		SSEDirtyTypeCast maskEMexp, maskELOne;

		maskEMexp.i = _mm_cmpgt_epi32( e, ldexpmexp );
		retval = _mm_select_ps( ldexpmaxfloat, retval, maskEMexp.f );
		retmask.f = _mm_or_ps( maskEMexp.f, retmask.f );

		maskELOne.i = _mm_cmplt_epi32( e, ldexpone );
		retval = _mm_select_ps( _mm_zero, retval, maskELOne.f );
		retmask.f = _mm_or_ps( maskELOne.f, retmask.f );

		y.i = _mm_and_si128( y.i, ldexpmaskExp );
		y.i = _mm_or_si128( _mm_slli_epi32( _mm_and_si128( e, msk_exponent ), 23 ), y.i );

		return _mm_select_ps( retval, y.f, retmask.f );
	}

	/*****************************************************************************/
	// Exp

	static const __m128 expMaxlog = _mm_set1_ps( 88.02969187150841f );
	static const __m128 expMinlog = _mm_set1_ps( -103.278929903431851103f );
	static const __m128 expmaxfloat = _mm_set1_ps( 1.7014117331926442990585209174225846272e38f );
	static const __m128 expLog2ef = _mm_set1_ps( 1.44269504088896341f );
	static const __m128 expC1 = _mm_set1_ps( 0.693359375f );
	static const __m128 expC2 = _mm_set1_ps( -2.12194440e-4f );
	static const __m128 expA1 = _mm_set1_ps( 5.0000001201E-1f );
	static const __m128 expA2 = _mm_set1_ps( 1.6666665459E-1f );
	static const __m128 expA3 = _mm_set1_ps( 4.1665795894E-2f );
	static const __m128 expA4 = _mm_set1_ps( 8.3334519073E-3f );
	static const __m128 expA5 = _mm_set1_ps( 1.3981999507E-3f );
	static const __m128 expA6 = _mm_set1_ps( 1.9875691500E-4f );


	__m128 _mm_exp_ps( const __m128 xx )
	{
		__m128 x, z;
		__m128i n;
		__m128 retval;

		SSEDirtyTypeCast retmask;

		retmask.f = _mm_cmpgt_ps( xx, expMaxlog );
		retval = _mm_select_ps( expmaxfloat, _mm_zero, retmask.f );
		retmask.f = _mm_or_ps( _mm_cmplt_ps( xx, expMinlog ), retmask.f );

		//if ( _mm_movemask_ps( retmask.f ) == 0xF ) return retval;

		x = xx;
		z = _mm_floor_ps( _mm_add_ps( _mm_mul_ps( expLog2ef, xx ), _mm_half ) );
		x = _mm_sub_ps( xx, _mm_mul_ps( z, expC1 ) );
		x = _mm_sub_ps( x, _mm_mul_ps( z, expC2 ) );
		n = _mm_cvttps_epi32( z );

		z = _mm_mul_ps( x, x );
		z = _mm_add_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( expA6, x ), expA5 ), x ), expA4 ), x ), expA3 ), x ), expA2 ), x ), expA1 ), z ), x ), _mm_one );

		x = _mm_ldexp_ps( z, n );

		return _mm_select_ps( retval, x, retmask.f );
	}


	/*****************************************************************************/
	// Frexp


	static const __m128i frexpsub = _mm_set1_epi32( 0x7e );
	static const __m128i ldexpmaskExponent = _mm_set1_epi32( 0x3f000000 );


	__m128 _mm_frexp_ps( const __m128 x, __m128i *pw2 )
	{
		SSEDirtyTypeCast y, retmask;

		y.f = x;

		__m128i i = _mm_and_si128( _mm_srli_epi32( y.i, 23 ), msk_exponent );
		retmask.i = _mm_cmpeq_epi32( i, ldexpzero );

		i = _mm_sub_epi32( i, frexpsub );

		y.i = _mm_and_si128( y.i, ldexpmaskExp );
		y.i = _mm_or_si128( y.i, ldexpmaskExponent );

		*pw2 = _mm_select_epi( ldexpzero, i, retmask.i );

		return _mm_select_ps( _mm_zero, y.f, retmask.f );
	}


	/*****************************************************************************/
	// Log


	static const __m128 logsqrthf = _mm_set1_ps( 0.707106781186547524f );
	static const __m128 logminfinity = _mm_set1_ps( -1.7014117331926442990585209174225846272e38f );
	static const __m128i logone = _mm_set1_epi32( 1 );
	static const __m128i logzero = _mm_set1_epi32( 0 );
	static const __m128 logC1 = _mm_set1_ps( 7.0376836292E-2f );
	static const __m128 logC2 = _mm_set1_ps( -1.1514610310E-1f );
	static const __m128 logC3 = _mm_set1_ps( 1.1676998740E-1f );
	static const __m128 logC4 = _mm_set1_ps( -1.2420140846E-1f );
	static const __m128 logC5 = _mm_set1_ps( 1.4249322787E-1f );
	static const __m128 logC6 = _mm_set1_ps( -1.6668057665E-1f );
	static const __m128 logC7 = _mm_set1_ps( 2.0000714765E-1f );
	static const __m128 logC8 = _mm_set1_ps( -2.4999993993E-1f );
	static const __m128 logC9 = _mm_set1_ps( 3.3333331174E-1f );
	static const __m128 logA1 = _mm_set1_ps( -2.12194440e-4f );
	static const __m128 logA2 = _mm_set1_ps( 0.693359375f );
	static const __m128 logmhalf = _mm_set1_ps( -0.5f );


	__m128 _mm_log_ps( const __m128 xx )
	{
		__m128 x, y, z, fe, retmask;
		__m128i e;

		x = xx;
		fe = _mm_zero;

		retmask = _mm_cmple_ps( x, _mm_zero );
//		if ( _mm_movemask_ps( retmask ) == 0xF )
//			return logminfinity;

		x = _mm_frexp_ps( x, &e );

		SSEDirtyTypeCast masksqrt;
		masksqrt.f = _mm_cmplt_ps( x, logsqrthf );
		e = _mm_select_epi( _mm_sub_epi32( e, logone ), e, masksqrt.i );
		x = _mm_select_ps( _mm_add_ps( x, x ), x, masksqrt.f );
		x = _mm_sub_ps( x, _mm_one );

		z = _mm_mul_ps( x, x );

		y = _mm_mul_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( logC1, x ), logC2 ), x ), logC3 ), x ), logC4 ), x ), logC5 ), x ), logC6 ), x ), logC7 ), x ), logC8 ), x ), logC9 ), x ), z );

		SSEDirtyTypeCast maske;

		maske.i = _mm_cmpeq_epi32( e, logzero );

		fe = _mm_cvtepi32_ps( e );
		y = _mm_select_ps( y, _mm_add_ps( y, _mm_mul_ps( logA1, fe ) ), maske.f );

		y = _mm_add_ps( y, _mm_mul_ps( logmhalf, z ) );
		z = _mm_add_ps( x, y );

		z = _mm_select_ps( z, _mm_add_ps( z, _mm_mul_ps( logA2, fe ) ), maske.f );

		return _mm_select_ps( logminfinity, z, retmask );
	}


	/*****************************************************************************/
	// Pow

	static const __m128 powA[] = {
		_mm_set1_ps( 1.00000000000000000000E0f ),
		_mm_set1_ps( 9.57603275775909423828125E-1f ),
		_mm_set1_ps( 9.17004048824310302734375E-1f ),
		_mm_set1_ps( 8.78126084804534912109375E-1f ),
		_mm_set1_ps( 8.40896427631378173828125E-1f ),
		_mm_set1_ps( 8.05245161056518554687500E-1f ),
		_mm_set1_ps( 7.71105408668518066406250E-1f ),
		_mm_set1_ps( 7.38413095474243164062500E-1f ),
		_mm_set1_ps( 7.07106769084930419921875E-1f ),
		_mm_set1_ps( 6.77127778530120849609375E-1f ),
		_mm_set1_ps( 6.48419797420501708984375E-1f ),
		_mm_set1_ps( 6.20928883552551269531250E-1f ),
		_mm_set1_ps( 5.94603538513183593750000E-1f ),
		_mm_set1_ps( 5.69394290447235107421875E-1f ),
		_mm_set1_ps( 5.45253872871398925781250E-1f ),
		_mm_set1_ps( 5.22136867046356201171875E-1f ),
		_mm_set1_ps( 5.00000000000000000000E-1f )
	};

	static const __m128 powB[] = {
		_mm_set1_ps( 0.00000000000000000000E0f ),
		_mm_set1_ps( -5.61963907099083340520586E-9f ),
		_mm_set1_ps( -1.23776636307969995237668E-8f ),
		_mm_set1_ps( 4.03545234539989593104537E-9f ),
		_mm_set1_ps( 1.21016171044789693621048E-8f ),
		_mm_set1_ps( -2.00949968760174979411038E-8f ),
		_mm_set1_ps( 1.89881769396087499852802E-8f ),
		_mm_set1_ps( -6.53877009617774467211965E-9f ),
		_mm_set1_ps( 0.00000000000000000000E0f )
	};

	static const __m128 powAinv[] = {
		_mm_set1_ps( 1.00000000000000000000000E0f ),
		_mm_set1_ps( 1.04427378242741384032197E0f ),
		_mm_set1_ps( 1.09050773266525765920701E0f ),
		_mm_set1_ps( 1.13878863475669165370383E0f ),
		_mm_set1_ps( 1.18920711500272106671750E0f ),
		_mm_set1_ps( 1.24185781207348404859368E0f ),
		_mm_set1_ps( 1.29683955465100966593375E0f ),
		_mm_set1_ps( 1.35425554693689272829801E0f ),
		_mm_set1_ps( 1.41421356237309504880169E0f ),
		_mm_set1_ps( 1.47682614593949931138691E0f ),
		_mm_set1_ps( 1.54221082540794082361229E0f ),
		_mm_set1_ps( 1.61049033194925430817952E0f ),
		_mm_set1_ps( 1.68179283050742908606225E0f ),
		_mm_set1_ps( 1.75625216037329948311216E0f ),
		_mm_set1_ps( 1.83400808640934246348708E0f ),
		_mm_set1_ps( 1.91520656139714729387261E0f ),
		_mm_set1_ps( 2.00000000000000000000000E0f )
	};


	static const __m128 powLOG2EA = _mm_set1_ps( 0.44269504088896340736f );
	static const __m128 powi16 = _mm_set1_ps( 0.0625f );
	static const __m128 pow16 = _mm_set1_ps( 16.0f );
	static const __m128i powizero = _mm_set1_epi32( 0 );
	static const __m128i powione = _mm_set1_epi32( 1 );
	static const __m128i powinine = _mm_set1_epi32( 9 );
	static const __m128i powifour = _mm_set1_epi32( 4 );
	static const __m128i powitwo = _mm_set1_epi32( 2 );
	static const __m128i powiminusone = _mm_set1_epi32( -1 );
	static const __m128 powC1 = _mm_set1_ps( -0.1663883081054895f );
	static const __m128 powC2 = _mm_set1_ps( 0.2003770364206271f );
	static const __m128 powC3 = _mm_set1_ps( -0.2500006373383951f );
	static const __m128 powC4 = _mm_set1_ps( 0.3333331095506474f );
	static const __m128 powD1 = _mm_set1_ps( 9.416993633606397E-003f );
	static const __m128 powD2 = _mm_set1_ps( 5.549356188719141E-002f );
	static const __m128 powD3 = _mm_set1_ps( 2.402262883964191E-001f );
	static const __m128 powD4 = _mm_set1_ps( 6.931471791490764E-001f );
	static const __m128 powMEXP = _mm_set1_ps( 2043.0f );
	static const __m128 powMNEXP = _mm_set1_ps( -2043.0f );


	#define reduc(x)  _mm_mul_ps( powi16, _mm_cvtepi32_ps( _mm_cvttps_epi32( _mm_mul_ps( pow16, x ) ) ) )
	//#define reduc(x)  _mm_mul_ps( powi16, _mm_floor_ps( _mm_mul_ps( pow16, x ) ) )


	__m128 _mm_pow_ps( __m128 x, __m128 y )
	{
		__m128 w, z, ya, yb, retval, retmask;
		__m128 F, Fa, Fb, G, Ga, Gb, H, Ha, Hb, nflg, A, B, Ainv;
		__m128i e, i;

		//w = _mm_floor_ps( y );
		w = _mm_cvtepi32_ps( _mm_cvttps_epi32( y ) );
		z = _mm_abs_ps( w );

		retmask = _mm_cmpeq_ps( x, _mm_zero );
		retval = _mm_select_ps( _mm_one, _mm_zero, _mm_and_ps( _mm_cmpeq_ps( y, _mm_zero ), retmask ) );
		nflg = _mm_cmplt_ps( x, _mm_zero );
		retmask = _mm_or_ps( _mm_and_ps( _mm_xor_ps( _mm_cmpeq_ps( w, y ), _mmi_minusone ), nflg ), retmask );

		x = _mm_abs_ps( x );
		x = _mm_frexp_ps( x, &e );

		SSEDirtyTypeCast mask;
		mask.f = _mm_cmple_ps( x, powA[9] );
		i = _mm_select_epi( powinine, powione, mask.i );

		// gather
		A.m128_f32[0] = powA[i.m128i_i32[0]+4].m128_f32[0];
		A.m128_f32[1] = powA[i.m128i_i32[1]+4].m128_f32[0];
		A.m128_f32[2] = powA[i.m128i_i32[2]+4].m128_f32[0];
		A.m128_f32[3] = powA[i.m128i_i32[3]+4].m128_f32[0];

		mask.f = _mm_cmple_ps( x, A );
		i = _mm_select_epi( _mm_add_epi32( i, powifour ), i, mask.i );

		A.m128_f32[0] = powA[i.m128i_i32[0]+2].m128_f32[0];
		A.m128_f32[1] = powA[i.m128i_i32[1]+2].m128_f32[0];
		A.m128_f32[2] = powA[i.m128i_i32[2]+2].m128_f32[0];
		A.m128_f32[3] = powA[i.m128i_i32[3]+2].m128_f32[0];

		mask.f = _mm_cmple_ps( x, A );
		i = _mm_select_epi( _mm_add_epi32( i, powitwo ), i, mask.i );
		mask.f = _mm_cmpge_ps( x, powA[1] );
		i = _mm_select_epi( powiminusone, i, mask.i );
		i = _mm_add_epi32( i, powione );

		A.m128_f32[0] = powA[i.m128i_i32[0]].m128_f32[0];
		A.m128_f32[1] = powA[i.m128i_i32[1]].m128_f32[0];
		A.m128_f32[2] = powA[i.m128i_i32[2]].m128_f32[0];
		A.m128_f32[3] = powA[i.m128i_i32[3]].m128_f32[0];

		x = _mm_sub_ps( x, A );

		B.m128_f32[0] = powB[i.m128i_i32[0]>>1].m128_f32[0];
		B.m128_f32[1] = powB[i.m128i_i32[1]>>1].m128_f32[0];
		B.m128_f32[2] = powB[i.m128i_i32[2]>>1].m128_f32[0];
		B.m128_f32[3] = powB[i.m128i_i32[3]>>1].m128_f32[0];

		x = _mm_sub_ps( x, B );

		Ainv.m128_f32[0] = powAinv[i.m128i_i32[0]].m128_f32[0];
		Ainv.m128_f32[1] = powAinv[i.m128i_i32[1]].m128_f32[0];
		Ainv.m128_f32[2] = powAinv[i.m128i_i32[2]].m128_f32[0];
		Ainv.m128_f32[3] = powAinv[i.m128i_i32[3]].m128_f32[0];

		x = _mm_mul_ps( x, Ainv );


		z = _mm_mul_ps( x, x );
		w = _mm_mul_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( powC1, x ), powC2 ), x ), powC3 ), x ), powC4 ), x ), z );
		w = _mm_sub_ps( w, _mm_mul_ps( _mm_half, z ) );
		w = _mm_add_ps( w, _mm_mul_ps( powLOG2EA, w ) );

		z = _mm_add_ps( w, _mm_mul_ps( powLOG2EA, x ) );
		z = _mm_add_ps( z, x );

		w = _mm_sub_ps( _mm_zero, _mm_cvtepi32_ps( i ) );
		w = _mm_mul_ps( powi16, w );
		w = _mm_add_ps( w, _mm_cvtepi32_ps( e ) );

		ya = reduc( y );
		yb = _mm_sub_ps( y, ya );

		F = _mm_add_ps( _mm_mul_ps( z, y ), _mm_mul_ps( w, yb ) );
		Fa = reduc( F );
		Fb = _mm_sub_ps( F, Fa );

		G = _mm_add_ps( Fa, _mm_mul_ps( w, ya ) );
		Ga = reduc( G );
		Gb = _mm_sub_ps( G, Ga );

		H = _mm_add_ps( Fb, Gb );
		Ha = reduc( H );
		w = _mm_mul_ps( pow16, _mm_add_ps( Ga, Ha ) );

		mask.f = _mm_cmpgt_ps( w, powMEXP );
		retval = _mm_select_ps( expmaxfloat, retval, _mm_andnot_ps( mask.f, retmask ) );
		retmask = _mm_or_ps( mask.f, retmask );
		w = _mm_select_ps( powMEXP, w, mask.f );

		mask.f = _mm_cmplt_ps( w, powMNEXP );
		retval = _mm_select_ps( _mm_zero, retval, _mm_andnot_ps( mask.f, retmask ) );
		retmask = _mm_or_ps( mask.f, retmask );
		w = _mm_select_ps( powMNEXP, w, mask.f );

		e = _mm_cvttps_epi32( w );
		Hb = _mm_sub_ps( H, Ha );

		mask.f = _mm_cmpgt_ps( Hb, _mm_zero );
		e = _mm_select_epi( _mm_add_epi32( e, powione ), e, mask.i );
		Hb = _mm_select_ps( _mm_sub_ps( Hb, powi16 ), Hb, mask.f );


		z = _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( _mm_add_ps( _mm_mul_ps( powD1, Hb ), powD2 ), Hb ), powD3 ), Hb ), powD4 ), Hb );


		i = _mm_select_epi( _mm_sub_epi32( powizero, _mm_srli_epi32( _mm_sub_epi32( powizero, e ), 4 ) ), 
							_mm_add_epi32( _mm_srli_epi32( e, 4 ), powione ), _mm_cmplt_epi32( e, powizero ) );
		e = _mm_sub_epi32( _mm_slli_epi32( i, 4 ), e );

		w.m128_f32[0] = powA[e.m128i_i32[0]].m128_f32[0];
		w.m128_f32[1] = powA[e.m128i_i32[1]].m128_f32[0];
		w.m128_f32[2] = powA[e.m128i_i32[2]].m128_f32[0];
		w.m128_f32[3] = powA[e.m128i_i32[3]].m128_f32[0];
		
		z = _mm_add_ps( w, _mm_mul_ps( w, z ) );
		z = _mm_ldexp_ps( z, i );

		w = _mm_cvtepi32_ps( _mm_cvttps_epi32( _mm_mul_ps( _mm_half, w ) ) );
		w = _mm_add_ps( w, w );
		z = _mm_select_ps( _mm_sub_ps( _mm_zero, z ), z, _mm_andnot_ps( nflg, _mm_cmpeq_ps( w, y ) ) );

		return _mm_select_ps( retval, z, retmask );
	}


};
