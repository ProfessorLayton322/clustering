#include "point.h"
#include <cmath>
#include <cassert>

#ifdef __AVX512F__

#include <x86intrin.h>

// reads 0 <= d < 4 floats as __m128
static inline __m128 masked_read(int d, const float* x) {
    assert(0 <= d && d < 4);
    __attribute__((__aligned__(16))) float buf[4] = {0, 0, 0, 0};
    switch (d) {
        case 3:
            buf[2] = x[2];
        case 2:
            buf[1] = x[1];
        case 1:
            buf[0] = x[0];
    }
    return _mm_load_ps(buf);
    // cannot use AVX2 _mm_mask_set1_epi32
}

// reads 0 <= d < 8 floats as __m256
static inline __m256 masked_read_8(int d, const float* x) {
    assert(0 <= d && d < 8);
    if (d < 4) {
        __m256 res = _mm256_setzero_ps();
        res = _mm256_insertf128_ps(res, masked_read(d, x), 0);
        return res;
    } else {
        __m256 res = _mm256_setzero_ps();
        res = _mm256_insertf128_ps(res, _mm_loadu_ps(x), 0);
        res = _mm256_insertf128_ps(res, masked_read(d - 4, x + 4), 1);
        return res;
    }
}

float fvec_L2sqr_avx(const float* x, const float* y, size_t d) {
    __m256 msum1 = _mm256_setzero_ps();

    while (d >= 8) {
        __m256 mx = _mm256_loadu_ps(x);
        x += 8;
        __m256 my = _mm256_loadu_ps(y);
        y += 8;
        const __m256 a_m_b1 = mx - my;
        msum1 += a_m_b1 * a_m_b1;
        d -= 8;
    }

    __m128 msum2 = _mm256_extractf128_ps(msum1, 1);
    msum2 += _mm256_extractf128_ps(msum1, 0);

    if (d >= 4) {
        __m128 mx = _mm_loadu_ps(x);
        x += 4;
        __m128 my = _mm_loadu_ps(y);
        y += 4;
        const __m128 a_m_b1 = mx - my;
        msum2 += a_m_b1 * a_m_b1;
        d -= 4;
    }

    if (d > 0) {
        __m128 mx = masked_read(d, x);
        __m128 my = masked_read(d, y);
        __m128 a_m_b1 = mx - my;
        msum2 += a_m_b1 * a_m_b1;
    }

    msum2 = _mm_hadd_ps(msum2, msum2);
    msum2 = _mm_hadd_ps(msum2, msum2);
    return _mm_cvtss_f32(msum2);
}

// reads 0 <= d < 16 floats as __m512
static inline __m512 masked_read_16(int d, const float* x) {
    assert(0 <= d && d < 16);
    if (d < 8) {
        __m512 res = _mm512_setzero_ps();
        res = _mm512_insertf32x8(res, masked_read_8(d, x), 0);
        return res;
    } else {
        __m512 res = _mm512_setzero_ps();
        res = _mm512_insertf32x8(res, _mm256_loadu_ps(x), 0);
        res = _mm512_insertf32x8(res, masked_read_8(d - 8, x + 8), 1);
        return res;
    }
}

float fvec_L2sqr_avx512(const float* x, const float* y, size_t d) {
    __m512 msum1 = _mm512_setzero_ps();

    while (d >= 16) {
        __m512 mx = _mm512_loadu_ps(x);
        x += 16;
        __m512 my = _mm512_loadu_ps(y);
        y += 16;
        const __m512 a_m_b1 = mx - my;
        msum1 += a_m_b1 * a_m_b1;
        d -= 16;
    }

    __m256 msum2 = _mm512_extractf32x8_ps(msum1, 1);
    msum2 += _mm512_extractf32x8_ps(msum1, 0);

    while (d >= 8) {
        __m256 mx = _mm256_loadu_ps(x);
        x += 8;
        __m256 my = _mm256_loadu_ps(y);
        y += 8;
        const __m256 a_m_b1 = mx - my;
        msum2 += a_m_b1 * a_m_b1;
        d -= 8;
    }

    __m128 msum3 = _mm256_extractf128_ps(msum2, 1);
    msum3 += _mm256_extractf128_ps(msum2, 0);

    if (d >= 4) {
        __m128 mx = _mm_loadu_ps(x);
        x += 4;
        __m128 my = _mm_loadu_ps(y);
        y += 4;
        const __m128 a_m_b1 = mx - my;
        msum3 += a_m_b1 * a_m_b1;
        d -= 4;
    }

    if (d > 0) {
        __m128 mx = masked_read(d, x);
        __m128 my = masked_read(d, y);
        __m128 a_m_b1 = mx - my;
        msum3 += a_m_b1 * a_m_b1;
    }

    msum3 = _mm_hadd_ps(msum3, msum3);
    msum3 = _mm_hadd_ps(msum3, msum3);
    return _mm_cvtss_f32(msum3);
}

float fvec_cosine_avx512(const float* x, const float* y, size_t d) {
    __m512 msum1 = _mm512_setzero_ps();

    while (d >= 16) {
        __m512 mx = _mm512_loadu_ps(x);
        x += 16;
        __m512 my = _mm512_loadu_ps(y);
        y += 16;
        const __m512 a_m_b1 = mx * my;
        msum1 += a_m_b1;
        d -= 16;
    }

    __m256 msum2 = _mm512_extractf32x8_ps(msum1, 1);
    msum2 += _mm512_extractf32x8_ps(msum1, 0);

    while (d >= 8) {
        __m256 mx = _mm256_loadu_ps(x);
        x += 8;
        __m256 my = _mm256_loadu_ps(y);
        y += 8;
        const __m256 a_m_b1 = mx * my;
        msum2 += a_m_b1;
        d -= 8;
    }

    __m128 msum3 = _mm256_extractf128_ps(msum2, 1);
    msum3 += _mm256_extractf128_ps(msum2, 0);

    if (d >= 4) {
        __m128 mx = _mm_loadu_ps(x);
        x += 4;
        __m128 my = _mm_loadu_ps(y);
        y += 4;
        const __m128 a_m_b1 = mx * my;
        msum3 += a_m_b1;
        d -= 4;
    }

    if (d > 0) {
        __m128 mx = masked_read(d, x);
        __m128 my = masked_read(d, y);
        __m128 a_m_b1 = mx * my;
        msum3 += a_m_b1;
    }

    msum3 = _mm_hadd_ps(msum3, msum3);
    msum3 = _mm_hadd_ps(msum3, msum3);
    return _mm_cvtss_f32(msum3);
}
#endif

float ScalarDistance(const Point& a, const Point& b) {
    assert(a.size() == b.size());
#ifdef __AVX512F__
    return fvec_cosine_avx512(a.data(), b.data(), a.size());
#endif
    float cosine  = 0;
    for (size_t i = 0; i < a.size(); i++) {
        cosine += a[i] * b[i];
    }
    return cosine;
}

float DistanceSquared(const Point& a, const Point& b) {
    assert(a.size() == b.size());
#ifdef __AVX512F__
    return fvec_L2sqr_avx512(a.data(), b.data(), a.size());
#endif
    float squareDist = 0;
    for (size_t i = 0; i < a.size(); i++) {
        squareDist += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return (squareDist >= 0) ? squareDist : 0;
}

float Distance(const Point& a, const Point& b) { return std::sqrt(DistanceSquared(a, b)); }

std::ostream& operator<<(std::ostream& out, const Point& point) {
    for (float x : point) {
        out << x << " ";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<Point>& points) {
    for (const Point& point : points) {
        out << point << std::endl;
    }
    return out;
}
