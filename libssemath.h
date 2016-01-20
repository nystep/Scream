#ifndef __LIBSSEMATH_H__
#define __LIBSSEMATH_H__


#include <xmmintrin.h>
#include <emmintrin.h>


// keep in mind that libssemath requires SSE2.
namespace ssemath {

    union ssevector {
        __m128 m128;
        __m128i m128i;
        float f32[4];
        int i32[4];
    } __attribute__ ((aligned (16)));


    extern const __m128 _mm_half;
    extern const __m128 _mm_three;
    extern const __m128 _mmi_msign;


    // reciprocal with an additionnal newton raphson iteration.

    inline __m128 _mm_rcp_nr_ps(__m128 x) {
        const __m128 tmp0 = _mm_rcp_ps(x);
        const __m128 tmp1 = _mm_mul_ps(_mm_mul_ps(tmp0, tmp0), x);
        return _mm_sub_ps(_mm_add_ps(tmp0, tmp0), tmp1);
    }

    // 1/sqrt with an additionnal newton raphson iteration.

    inline __m128 _mm_rsqrt_nr_ps(__m128 x) {
        const __m128 tmp1 = _mm_rsqrt_ps(x);
        const __m128 tmp2 = _mm_sub_ps(_mm_mul_ps(_mm_mul_ps(x, tmp1), tmp1), _mm_three);
        return _mm_mul_ps(_mm_mul_ps(tmp2, tmp1), _mm_half);
    }

    inline __m128 _mm_select_ps(__m128 a, __m128 b, __m128 cond) {
        return _mm_or_ps(_mm_and_ps(a, cond), _mm_andnot_ps(cond, b));
    }

    inline __m128i _mm_select_epi(__m128i a, __m128i b, __m128i cond) {
        return _mm_or_si128(_mm_and_si128(a, cond), _mm_andnot_si128(cond, b));
    }

    inline __m128 _mm_set1i_ps(int i) {
        ssevector ret;
        ret.i32[0] = i;
        return _mm_shuffle_ps(ret.m128, ret.m128, 0);
    }

    inline __m128 _mm_abs_ps(__m128 x) {
        return _mm_and_ps(x, _mmi_msign);
    }

    inline __m128 _mm_madd_ps(__m128 x, __m128 y, __m128 z) {
        return _mm_add_ps(_mm_mul_ps(x, y), z);
    }

    // fast but inaccurate with big angles.
    __m128 _mm_fastsin_ps(__m128 x);
    __m128 _mm_fastcos_ps(__m128 x);
    __m128 _mm_fasttan_ps(__m128 x);

    __m128 _mm_sin_ps(__m128 x);
    __m128 _mm_cos_ps(__m128 x);
    __m128 _mm_tan_ps(__m128 x);

    __m128 _mm_atan_ps(__m128 x);
    __m128 _mm_acos_ps(__m128 x);
    __m128 _mm_asin_ps(__m128 x);

    // not enough precise.
    //__m128 _mm_ceil_ps( __m128 x );
    //__m128 _mm_floor_ps( __m128 x );
    __m128 _mm_ldexp_ps(__m128 x, __m128i pw2);
    __m128 _mm_frexp_ps(__m128 x, __m128i *pw2);
    __m128 _mm_modf_ps(__m128 x, __m128 *intpart);

    __m128 _mm_exp_ps(__m128 x);
    __m128 _mm_log_ps(__m128 x);

    __m128 _mm_pow_ps(__m128 x, __m128 y);

};


#endif
