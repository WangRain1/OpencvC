/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCV_HAL_INTRIN_NEON_HPP
#define OPENCV_HAL_INTRIN_NEON_HPP

#include <algorithm>
#include "opencv2/core/utility.hpp"

namespace cv
{

//! @cond IGNORED

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_BEGIN

#define CV_SIMD128 1
#if defined(__aarch64__)
#define CV_SIMD128_64F a1
#else
#define CV_SIMD128_64F 0
#endif

#if CV_SIMD128_64F
#define OPENCV_HAL_IMPL_NEON_REINTERPRET(_Tpv, suffix) \
template <typename T> static inline \
_Tpv vreinterpretq_##suffix##_f64(T a) { return (_Tpv) a; } \
template <typename T> static inline \
float64x2_t vreinterpretq_f64_##suffix(T a) { return (float64x2_t) a; }
OPENCV_HAL_IMPL_NEON_REINTERPRET(uint8x16_t, u8)
OPENCV_HAL_IMPL_NEON_REINTERPRET(int8x16_t, s8)
OPENCV_HAL_IMPL_NEON_REINTERPRET(uint16x8_t, u16)
OPENCV_HAL_IMPL_NEON_REINTERPRET(int16x8_t, s16)
OPENCV_HAL_IMPL_NEON_REINTERPRET(uint32x4_t, u32)
OPENCV_HAL_IMPL_NEON_REINTERPRET(int32x4_t, s32)
OPENCV_HAL_IMPL_NEON_REINTERPRET(uint64x2_t, u64)
OPENCV_HAL_IMPL_NEON_REINTERPRET(int64x2_t, s64)
OPENCV_HAL_IMPL_NEON_REINTERPRET(float32x4_t, f32)
#endif

struct v_uint8x16
{
    typedef uchar lane_type;
    enum { nlanes = 16 };

    v_uint8x16() {}
    explicit v_uint8x16(uint8x16_t v) : val(v) {}
    v_uint8x16(uchar v0, uchar v1, uchar v2, uchar v3, uchar v4, uchar v5, uchar v6, uchar v7,
               uchar v8, uchar v9, uchar v10, uchar v11, uchar v12, uchar v13, uchar v14, uchar v15)
    {
        uchar v[] = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15};
        val = vld1q_u8(v);
    }
    uchar get0() const
    {
        return vgetq_lane_u8(val, 0);
    }

    uint8x16_t val;
};

struct v_int8x16
{
    typedef schar lane_type;
    enum { nlanes = 16 };

    v_int8x16() {}
    explicit v_int8x16(int8x16_t v) : val(v) {}
    v_int8x16(schar v0, schar v1, schar v2, schar v3, schar v4, schar v5, schar v6, schar v7,
               schar v8, schar v9, schar v10, schar v11, schar v12, schar v13, schar v14, schar v15)
    {
        schar v[] = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15};
        val = vld1q_s8(v);
    }
    schar get0() const
    {
        return vgetq_lane_s8(val, 0);
    }

    int8x16_t val;
};

struct v_uint16x8
{
    typedef ushort lane_type;
    enum { nlanes = 8 };

    v_uint16x8() {}
    explicit v_uint16x8(uint16x8_t v) : val(v) {}
    v_uint16x8(ushort v0, ushort v1, ushort v2, ushort v3, ushort v4, ushort v5, ushort v6, ushort v7)
    {
        ushort v[] = {v0, v1, v2, v3, v4, v5, v6, v7};
        val = vld1q_u16(v);
    }
    ushort get0() const
    {
        return vgetq_lane_u16(val, 0);
    }

    uint16x8_t val;
};

struct v_int16x8
{
    typedef short lane_type;
    enum { nlanes = 8 };

    v_int16x8() {}
    explicit v_int16x8(int16x8_t v) : val(v) {}
    v_int16x8(short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7)
    {
        short v[] = {v0, v1, v2, v3, v4, v5, v6, v7};
        val = vld1q_s16(v);
    }
    short get0() const
    {
        return vgetq_lane_s16(val, 0);
    }

    int16x8_t val;
};

struct v_uint32x4
{
    typedef unsigned lane_type;
    enum { nlanes = 4 };

    v_uint32x4() {}
    explicit v_uint32x4(uint32x4_t v) : val(v) {}
    v_uint32x4(unsigned v0, unsigned v1, unsigned v2, unsigned v3)
    {
        unsigned v[] = {v0, v1, v2, v3};
        val = vld1q_u32(v);
    }
    unsigned get0() const
    {
        return vgetq_lane_u32(val, 0);
    }

    uint32x4_t val;
};

struct v_int32x4
{
    typedef int lane_type;
    enum { nlanes = 4 };

    v_int32x4() {}
    explicit v_int32x4(int32x4_t v) : val(v) {}
    v_int32x4(int v0, int v1, int v2, int v3)
    {
        int v[] = {v0, v1, v2, v3};
        val = vld1q_s32(v);
    }
    int get0() const
    {
        return vgetq_lane_s32(val, 0);
    }
    int32x4_t val;
};

struct v_float32x4
{
    typedef float lane_type;
    enum { nlanes = 4 };

    v_float32x4() {}
    explicit v_float32x4(float32x4_t v) : val(v) {}
    v_float32x4(float v0, float v1, float v2, float v3)
    {
        float v[] = {v0, v1, v2, v3};
        val = vld1q_f32(v);
    }
    float get0() const
    {
        return vgetq_lane_f32(val, 0);
    }
    float32x4_t val;
};

struct v_uint64x2
{
    typedef uint64 lane_type;
    enum { nlanes = 2 };

    v_uint64x2() {}
    explicit v_uint64x2(uint64x2_t v) : val(v) {}
    v_uint64x2(uint64 v0, uint64 v1)
    {
        uint64 v[] = {v0, v1};
        val = vld1q_u64(v);
    }
    uint64 get0() const
    {
        return vgetq_lane_u64(val, 0);
    }
    uint64x2_t val;
};

struct v_int64x2
{
    typedef int64 lane_type;
    enum { nlanes = 2 };

    v_int64x2() {}
    explicit v_int64x2(int64x2_t v) : val(v) {}
    v_int64x2(int64 v0, int64 v1)
    {
        int64 v[] = {v0, v1};
        val = vld1q_s64(v);
    }
    int64 get0() const
    {
        return vgetq_lane_s64(val, 0);
    }
    int64x2_t val;
};

#if CV_SIMD128_64F
struct v_float64x2
{
    typedef double lane_type;
    enum { nlanes = 2 };

    v_float64x2() {}
    explicit v_float64x2(float64x2_t v) : val(v) {}
    v_float64x2(double v0, double v1)
    {
        double v[] = {v0, v1};
        val = vld1q_f64(v);
    }
    double get0() const
    {
        return vgetq_lane_f64(val, 0);
    }
    float64x2_t val;
};
#endif

#if CV_FP16
// Workaround for old compilers
static inline int16x4_t vreinterpret_s16_f16(float16x4_t a) { return (int16x4_t)a; }
static inline float16x4_t vreinterpret_f16_s16(int16x4_t a) { return (float16x4_t)a; }

static inline float16x4_t cv_vld1_f16(const void* ptr)
{
#ifndef vld1_f16 // APPLE compiler defines vld1_f16 as macro
    return vreinterpret_f16_s16(vld1_s16((const short*)ptr));
#else
    return vld1_f16((const __fp16*)ptr);
#endif
}
static inline void cv_vst1_f16(void* ptr, float16x4_t a)
{
#ifndef vst1_f16 // APPLE compiler defines vst1_f16 as macro
    vst1_s16((short*)ptr, vreinterpret_s16_f16(a));
#else
    vst1_f16((__fp16*)ptr, a);
#endif
}

#ifndef vdup_n_f16
    #define vdup_n_f16(v) (float16x4_t){v, v, v, v}
#endif

#endif // CV_FP16

#if CV_FP16
inline v_float32x4 v128_load_fp16_f32(const short* ptr)
{
    float16x4_t a = cv_vld1_f16((const __fp16*)ptr);
    return v_float32x4(vcvt_f32_f16(a));
}

inline void v_store_fp16(short* ptr, const v_float32x4& a)
{
    float16x4_t fp16 = vcvt_f16_f32(a.val);
    cv_vst1_f16((short*)ptr, fp16);
}
#endif

#define OPENCV_HAL_IMPL_NEON_INIT(_Tpv, _Tp, suffix) \
inline v_##_Tpv v_setzero_##suffix() { return v_##_Tpv(vdupq_n_##suffix((_Tp)0)); } \
inline v_##_Tpv v_setall_##suffix(_Tp v) { return v_##_Tpv(vdupq_n_##suffix(v)); } \
inline _Tpv##_t vreinterpretq_##suffix##_##suffix(_Tpv##_t v) { return v; } \
inline v_uint8x16 v_reinterpret_as_u8(const v_##_Tpv& v) { return v_uint8x16(vreinterpretq_u8_##suffix(v.val)); } \
inline v_int8x16 v_reinterpret_as_s8(const v_##_Tpv& v) { return v_int8x16(vreinterpretq_s8_##suffix(v.val)); } \
inline v_uint16x8 v_reinterpret_as_u16(const v_##_Tpv& v) { return v_uint16x8(vreinterpretq_u16_##suffix(v.val)); } \
inline v_int16x8 v_reinterpret_as_s16(const v_##_Tpv& v) { return v_int16x8(vreinterpretq_s16_##suffix(v.val)); } \
inline v_uint32x4 v_reinterpret_as_u32(const v_##_Tpv& v) { return v_uint32x4(vreinterpretq_u32_##suffix(v.val)); } \
inline v_int32x4 v_reinterpret_as_s32(const v_##_Tpv& v) { return v_int32x4(vreinterpretq_s32_##suffix(v.val)); } \
inline v_uint64x2 v_reinterpret_as_u64(const v_##_Tpv& v) { return v_uint64x2(vreinterpretq_u64_##suffix(v.val)); } \
inline v_int64x2 v_reinterpret_as_s64(const v_##_Tpv& v) { return v_int64x2(vreinterpretq_s64_##suffix(v.val)); } \
inline v_float32x4 v_reinterpret_as_f32(const v_##_Tpv& v) { return v_float32x4(vreinterpretq_f32_##suffix(v.val)); }

OPENCV_HAL_IMPL_NEON_INIT(uint8x16, uchar, u8)
OPENCV_HAL_IMPL_NEON_INIT(int8x16, schar, s8)
OPENCV_HAL_IMPL_NEON_INIT(uint16x8, ushort, u16)
OPENCV_HAL_IMPL_NEON_INIT(int16x8, short, s16)
OPENCV_HAL_IMPL_NEON_INIT(uint32x4, unsigned, u32)
OPENCV_HAL_IMPL_NEON_INIT(int32x4, int, s32)
OPENCV_HAL_IMPL_NEON_INIT(uint64x2, uint64, u64)
OPENCV_HAL_IMPL_NEON_INIT(int64x2, int64, s64)
OPENCV_HAL_IMPL_NEON_INIT(float32x4, float, f32)
#if CV_SIMD128_64F
#define OPENCV_HAL_IMPL_NEON_INIT_64(_Tpv, suffix) \
inline v_float64x2 v_reinterpret_as_f64(const v_##_Tpv& v) { return v_float64x2(vreinterpretq_f64_##suffix(v.val)); }
OPENCV_HAL_IMPL_NEON_INIT(float64x2, double, f64)
OPENCV_HAL_IMPL_NEON_INIT_64(uint8x16, u8)
OPENCV_HAL_IMPL_NEON_INIT_64(int8x16, s8)
OPENCV_HAL_IMPL_NEON_INIT_64(uint16x8, u16)
OPENCV_HAL_IMPL_NEON_INIT_64(int16x8, s16)
OPENCV_HAL_IMPL_NEON_INIT_64(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_INIT_64(int32x4, s32)
OPENCV_HAL_IMPL_NEON_INIT_64(uint64x2, u64)
OPENCV_HAL_IMPL_NEON_INIT_64(int64x2, s64)
OPENCV_HAL_IMPL_NEON_INIT_64(float32x4, f32)
OPENCV_HAL_IMPL_NEON_INIT_64(float64x2, f64)
#endif

#define OPENCV_HAL_IMPL_NEON_PACK(_Tpvec, _Tp, hreg, suffix, _Tpwvec, pack, mov, rshr) \
inline _Tpvec v_##pack(const _Tpwvec& a, const _Tpwvec& b) \
{ \
    hreg a1 = mov(a.val), b1 = mov(b.val); \
    return _Tpvec(vcombine_##suffix(a1, b1)); \
} \
inline void v_##pack##_store(_Tp* ptr, const _Tpwvec& a) \
{ \
    hreg a1 = mov(a.val); \
    vst1_##suffix(ptr, a1); \
} \
template<int n> inline \
_Tpvec v_rshr_##pack(const _Tpwvec& a, const _Tpwvec& b) \
{ \
    hreg a1 = rshr(a.val, n); \
    hreg b1 = rshr(b.val, n); \
    return _Tpvec(vcombine_##suffix(a1, b1)); \
} \
template<int n> inline \
void v_rshr_##pack##_store(_Tp* ptr, const _Tpwvec& a) \
{ \
    hreg a1 = rshr(a.val, n); \
    vst1_##suffix(ptr, a1); \
}

OPENCV_HAL_IMPL_NEON_PACK(v_uint8x16, uchar, uint8x8_t, u8, v_uint16x8, pack, vqmovn_u16, vqrshrn_n_u16)
OPENCV_HAL_IMPL_NEON_PACK(v_int8x16, schar, int8x8_t, s8, v_int16x8, pack, vqmovn_s16, vqrshrn_n_s16)
OPENCV_HAL_IMPL_NEON_PACK(v_uint16x8, ushort, uint16x4_t, u16, v_uint32x4, pack, vqmovn_u32, vqrshrn_n_u32)
OPENCV_HAL_IMPL_NEON_PACK(v_int16x8, short, int16x4_t, s16, v_int32x4, pack, vqmovn_s32, vqrshrn_n_s32)
OPENCV_HAL_IMPL_NEON_PACK(v_uint32x4, unsigned, uint32x2_t, u32, v_uint64x2, pack, vmovn_u64, vrshrn_n_u64)
OPENCV_HAL_IMPL_NEON_PACK(v_int32x4, int, int32x2_t, s32, v_int64x2, pack, vmovn_s64, vrshrn_n_s64)

OPENCV_HAL_IMPL_NEON_PACK(v_uint8x16, uchar, uint8x8_t, u8, v_int16x8, pack_u, vqmovun_s16, vqrshrun_n_s16)
OPENCV_HAL_IMPL_NEON_PACK(v_uint16x8, ushort, uint16x4_t, u16, v_int32x4, pack_u, vqmovun_s32, vqrshrun_n_s32)

// pack boolean
inline v_uint8x16 v_pack_b(const v_uint16x8& a, const v_uint16x8& b)
{
    uint8x16_t ab = vcombine_u8(vmovn_u16(a.val), vmovn_u16(b.val));
    return v_uint8x16(ab);
}

inline v_uint8x16 v_pack_b(const v_uint32x4& a, const v_uint32x4& b,
                           const v_uint32x4& c, const v_uint32x4& d)
{
    uint16x8_t nab = vcombine_u16(vmovn_u32(a.val), vmovn_u32(b.val));
    uint16x8_t ncd = vcombine_u16(vmovn_u32(c.val), vmovn_u32(d.val));
    return v_uint8x16(vcombine_u8(vmovn_u16(nab), vmovn_u16(ncd)));
}

inline v_uint8x16 v_pack_b(const v_uint64x2& a, const v_uint64x2& b, const v_uint64x2& c,
                           const v_uint64x2& d, const v_uint64x2& e, const v_uint64x2& f,
                           const v_uint64x2& g, const v_uint64x2& h)
{
    uint32x4_t ab = vcombine_u32(vmovn_u64(a.val), vmovn_u64(b.val));
    uint32x4_t cd = vcombine_u32(vmovn_u64(c.val), vmovn_u64(d.val));
    uint32x4_t ef = vcombine_u32(vmovn_u64(e.val), vmovn_u64(f.val));
    uint32x4_t gh = vcombine_u32(vmovn_u64(g.val), vmovn_u64(h.val));

    uint16x8_t abcd = vcombine_u16(vmovn_u32(ab), vmovn_u32(cd));
    uint16x8_t efgh = vcombine_u16(vmovn_u32(ef), vmovn_u32(gh));
    return v_uint8x16(vcombine_u8(vmovn_u16(abcd), vmovn_u16(efgh)));
}

inline v_float32x4 v_matmul(const v_float32x4& v, const v_float32x4& m0,
                            const v_float32x4& m1, const v_float32x4& m2,
                            const v_float32x4& m3)
{
    float32x2_t vl = vget_low_f32(v.val), vh = vget_high_f32(v.val);
    float32x4_t res = vmulq_lane_f32(m0.val, vl, 0);
    res = vmlaq_lane_f32(res, m1.val, vl, 1);
    res = vmlaq_lane_f32(res, m2.val, vh, 0);
    res = vmlaq_lane_f32(res, m3.val, vh, 1);
    return v_float32x4(res);
}

inline v_float32x4 v_matmuladd(const v_float32x4& v, const v_float32x4& m0,
                               const v_float32x4& m1, const v_float32x4& m2,
                               const v_float32x4& a)
{
    float32x2_t vl = vget_low_f32(v.val), vh = vget_high_f32(v.val);
    float32x4_t res = vmulq_lane_f32(m0.val, vl, 0);
    res = vmlaq_lane_f32(res, m1.val, vl, 1);
    res = vmlaq_lane_f32(res, m2.val, vh, 0);
    res = vaddq_f32(res, a.val);
    return v_float32x4(res);
}

#define OPENCV_HAL_IMPL_NEON_BIN_OP(bin_op, _Tpvec, intrin) \
inline _Tpvec operator bin_op (const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(intrin(a.val, b.val)); \
} \
inline _Tpvec& operator bin_op##= (_Tpvec& a, const _Tpvec& b) \
{ \
    a.val = intrin(a.val, b.val); \
    return a; \
}

OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint8x16, vqaddq_u8)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint8x16, vqsubq_u8)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int8x16, vqaddq_s8)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int8x16, vqsubq_s8)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint16x8, vqaddq_u16)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint16x8, vqsubq_u16)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int16x8, vqaddq_s16)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int16x8, vqsubq_s16)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int32x4, vaddq_s32)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int32x4, vsubq_s32)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_int32x4, vmulq_s32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint32x4, vaddq_u32)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint32x4, vsubq_u32)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_uint32x4, vmulq_u32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_float32x4, vaddq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_float32x4, vsubq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_float32x4, vmulq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_int64x2, vaddq_s64)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_int64x2, vsubq_s64)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_uint64x2, vaddq_u64)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_uint64x2, vsubq_u64)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_BIN_OP(/, v_float32x4, vdivq_f32)
OPENCV_HAL_IMPL_NEON_BIN_OP(+, v_float64x2, vaddq_f64)
OPENCV_HAL_IMPL_NEON_BIN_OP(-, v_float64x2, vsubq_f64)
OPENCV_HAL_IMPL_NEON_BIN_OP(*, v_float64x2, vmulq_f64)
OPENCV_HAL_IMPL_NEON_BIN_OP(/, v_float64x2, vdivq_f64)
#else
inline v_float32x4 operator / (const v_float32x4& a, const v_float32x4& b)
{
    float32x4_t reciprocal = vrecpeq_f32(b.val);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    return v_float32x4(vmulq_f32(a.val, reciprocal));
}
inline v_float32x4& operator /= (v_float32x4& a, const v_float32x4& b)
{
    float32x4_t reciprocal = vrecpeq_f32(b.val);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    reciprocal = vmulq_f32(vrecpsq_f32(b.val, reciprocal), reciprocal);
    a.val = vmulq_f32(a.val, reciprocal);
    return a;
}
#endif

// saturating multiply 8-bit, 16-bit
#define OPENCV_HAL_IMPL_NEON_MUL_SAT(_Tpvec, _Tpwvec)            \
    inline _Tpvec operator * (const _Tpvec& a, const _Tpvec& b)  \
    {                                                            \
        _Tpwvec c, d;                                            \
        v_mul_expand(a, b, c, d);                                \
        return v_pack(c, d);                                     \
    }                                                            \
    inline _Tpvec& operator *= (_Tpvec& a, const _Tpvec& b)      \
    { a = a * b; return a; }

OPENCV_HAL_IMPL_NEON_MUL_SAT(v_int8x16,  v_int16x8)
OPENCV_HAL_IMPL_NEON_MUL_SAT(v_uint8x16, v_uint16x8)
OPENCV_HAL_IMPL_NEON_MUL_SAT(v_int16x8,  v_int32x4)
OPENCV_HAL_IMPL_NEON_MUL_SAT(v_uint16x8, v_uint32x4)

//  Multiply and expand
inline void v_mul_expand(const v_int8x16& a, const v_int8x16& b,
                         v_int16x8& c, v_int16x8& d)
{
    c.val = vmull_s8(vget_low_s8(a.val), vget_low_s8(b.val));
    d.val = vmull_s8(vget_high_s8(a.val), vget_high_s8(b.val));
}

inline void v_mul_expand(const v_uint8x16& a, const v_uint8x16& b,
                         v_uint16x8& c, v_uint16x8& d)
{
    c.val = vmull_u8(vget_low_u8(a.val), vget_low_u8(b.val));
    d.val = vmull_u8(vget_high_u8(a.val), vget_high_u8(b.val));
}

inline void v_mul_expand(const v_int16x8& a, const v_int16x8& b,
                         v_int32x4& c, v_int32x4& d)
{
    c.val = vmull_s16(vget_low_s16(a.val), vget_low_s16(b.val));
    d.val = vmull_s16(vget_high_s16(a.val), vget_high_s16(b.val));
}

inline void v_mul_expand(const v_uint16x8& a, const v_uint16x8& b,
                         v_uint32x4& c, v_uint32x4& d)
{
    c.val = vmull_u16(vget_low_u16(a.val), vget_low_u16(b.val));
    d.val = vmull_u16(vget_high_u16(a.val), vget_high_u16(b.val));
}

inline void v_mul_expand(const v_uint32x4& a, const v_uint32x4& b,
                         v_uint64x2& c, v_uint64x2& d)
{
    c.val = vmull_u32(vget_low_u32(a.val), vget_low_u32(b.val));
    d.val = vmull_u32(vget_high_u32(a.val), vget_high_u32(b.val));
}

inline v_int16x8 v_mul_hi(const v_int16x8& a, const v_int16x8& b)
{
    return v_int16x8(vcombine_s16(
                                  vshrn_n_s32(vmull_s16( vget_low_s16(a.val),  vget_low_s16(b.val)), 16),
                                  vshrn_n_s32(vmull_s16(vget_high_s16(a.val), vget_high_s16(b.val)), 16)
                                 ));
}
inline v_uint16x8 v_mul_hi(const v_uint16x8& a, const v_uint16x8& b)
{
    return v_uint16x8(vcombine_u16(
                                   vshrn_n_u32(vmull_u16( vget_low_u16(a.val),  vget_low_u16(b.val)), 16),
                                   vshrn_n_u32(vmull_u16(vget_high_u16(a.val), vget_high_u16(b.val)), 16)
                                  ));
}

inline v_int32x4 v_dotprod(const v_int16x8& a, const v_int16x8& b)
{
    int32x4_t c = vmull_s16(vget_low_s16(a.val), vget_low_s16(b.val));
    int32x4_t d = vmull_s16(vget_high_s16(a.val), vget_high_s16(b.val));
    int32x4x2_t cd = vuzpq_s32(c, d);
    return v_int32x4(vaddq_s32(cd.val[0], cd.val[1]));
}

inline v_int32x4 v_dotprod(const v_int16x8& a, const v_int16x8& b, const v_int32x4& c)
{
    v_int32x4 s = v_dotprod(a, b);
    return v_int32x4(vaddq_s32(s.val , c.val));
}

#define OPENCV_HAL_IMPL_NEON_LOGIC_OP(_Tpvec, suffix) \
    OPENCV_HAL_IMPL_NEON_BIN_OP(&, _Tpvec, vandq_##suffix) \
    OPENCV_HAL_IMPL_NEON_BIN_OP(|, _Tpvec, vorrq_##suffix) \
    OPENCV_HAL_IMPL_NEON_BIN_OP(^, _Tpvec, veorq_##suffix) \
    inline _Tpvec operator ~ (const _Tpvec& a) \
    { \
        return _Tpvec(vreinterpretq_##suffix##_u8(vmvnq_u8(vreinterpretq_u8_##suffix(a.val)))); \
    }

OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint8x16, u8)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int8x16, s8)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint16x8, u16)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int16x8, s16)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint32x4, u32)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int32x4, s32)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_uint64x2, u64)
OPENCV_HAL_IMPL_NEON_LOGIC_OP(v_int64x2, s64)

#define OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(bin_op, intrin) \
inline v_float32x4 operator bin_op (const v_float32x4& a, const v_float32x4& b) \
{ \
    return v_float32x4(vreinterpretq_f32_s32(intrin(vreinterpretq_s32_f32(a.val), vreinterpretq_s32_f32(b.val)))); \
} \
inline v_float32x4& operator bin_op##= (v_float32x4& a, const v_float32x4& b) \
{ \
    a.val = vreinterpretq_f32_s32(intrin(vreinterpretq_s32_f32(a.val), vreinterpretq_s32_f32(b.val))); \
    return a; \
}

OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(&, vandq_s32)
OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(|, vorrq_s32)
OPENCV_HAL_IMPL_NEON_FLT_BIT_OP(^, veorq_s32)

inline v_float32x4 operator ~ (const v_float32x4& a)
{
    return v_float32x4(vreinterpretq_f32_s32(vmvnq_s32(vreinterpretq_s32_f32(a.val))));
}

#if CV_SIMD128_64F
inline v_float32x4 v_sqrt(const v_float32x4& x)
{
    return v_float32x4(vsqrtq_f32(x.val));
}

inline v_float32x4 v_invsqrt(const v_float32x4& x)
{
    v_float32x4 one = v_setall_f32(a1.0f);
    return one / v_sqrt(x);
}
#else
inline v_float32x4 v_sqrt(const v_float32x4& x)
{
    float32x4_t x1 = vmaxq_f32(x.val, vdupq_n_f32(FLT_MIN));
    float32x4_t e = vrsqrteq_f32(x1);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x1, e), e), e);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x1, e), e), e);
    return v_float32x4(vmulq_f32(x.val, e));
}

inline v_float32x4 v_invsqrt(const v_float32x4& x)
{
    float32x4_t e = vrsqrteq_f32(x.val);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x.val, e), e), e);
    e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(x.val, e), e), e);
    return v_float32x4(e);
}
#endif

#define OPENCV_HAL_IMPL_NEON_ABS(_Tpuvec, _Tpsvec, usuffix, ssuffix) \
inline _Tpuvec v_abs(const _Tpsvec& a) { return v_reinterpret_as_##usuffix(_Tpsvec(vabsq_##ssuffix(a.val))); }

OPENCV_HAL_IMPL_NEON_ABS(v_uint8x16, v_int8x16, u8, s8)
OPENCV_HAL_IMPL_NEON_ABS(v_uint16x8, v_int16x8, u16, s16)
OPENCV_HAL_IMPL_NEON_ABS(v_uint32x4, v_int32x4, u32, s32)

inline v_float32x4 v_abs(v_float32x4 x)
{ return v_float32x4(vabsq_f32(x.val)); }

#if CV_SIMD128_64F
#define OPENCV_HAL_IMPL_NEON_DBL_BIT_OP(bin_op, intrin) \
inline v_float64x2 operator bin_op (const v_float64x2& a, const v_float64x2& b) \
{ \
    return v_float64x2(vreinterpretq_f64_s64(intrin(vreinterpretq_s64_f64(a.val), vreinterpretq_s64_f64(b.val)))); \
} \
inline v_float64x2& operator bin_op##= (v_float64x2& a, const v_float64x2& b) \
{ \
    a.val = vreinterpretq_f64_s64(intrin(vreinterpretq_s64_f64(a.val), vreinterpretq_s64_f64(b.val))); \
    return a; \
}

OPENCV_HAL_IMPL_NEON_DBL_BIT_OP(&, vandq_s64)
OPENCV_HAL_IMPL_NEON_DBL_BIT_OP(|, vorrq_s64)
OPENCV_HAL_IMPL_NEON_DBL_BIT_OP(^, veorq_s64)

inline v_float64x2 operator ~ (const v_float64x2& a)
{
    return v_float64x2(vreinterpretq_f64_s32(vmvnq_s32(vreinterpretq_s32_f64(a.val))));
}

inline v_float64x2 v_sqrt(const v_float64x2& x)
{
    return v_float64x2(vsqrtq_f64(x.val));
}

inline v_float64x2 v_invsqrt(const v_float64x2& x)
{
    v_float64x2 one = v_setall_f64(a1.0f);
    return one / v_sqrt(x);
}

inline v_float64x2 v_abs(v_float64x2 x)
{ return v_float64x2(vabsq_f64(x.val)); }
#endif

// TODO: exp, log, sin, cos

#define OPENCV_HAL_IMPL_NEON_BIN_FUNC(_Tpvec, func, intrin) \
inline _Tpvec func(const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(intrin(a.val, b.val)); \
}

OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_min, vminq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_max, vmaxq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_min, vminq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_max, vmaxq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_min, vminq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_max, vmaxq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_min, vminq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_max, vmaxq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint32x4, v_min, vminq_u32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint32x4, v_max, vmaxq_u32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int32x4, v_min, vminq_s32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int32x4, v_max, vmaxq_s32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float32x4, v_min, vminq_f32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float32x4, v_max, vmaxq_f32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float64x2, v_min, vminq_f64)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float64x2, v_max, vmaxq_f64)
#endif

#if CV_SIMD128_64F
inline int64x2_t vmvnq_s64(int64x2_t a)
{
    int64x2_t vx = vreinterpretq_s64_u32(vdupq_n_u32(0xFFFFFFFF));
    return veorq_s64(a, vx);
}
inline uint64x2_t vmvnq_u64(uint64x2_t a)
{
    uint64x2_t vx = vreinterpretq_u64_u32(vdupq_n_u32(0xFFFFFFFF));
    return veorq_u64(a, vx);
}
#endif
#define OPENCV_HAL_IMPL_NEON_INT_CMP_OP(_Tpvec, cast, suffix, not_suffix) \
inline _Tpvec operator == (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vceqq_##suffix(a.val, b.val))); } \
inline _Tpvec operator != (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vmvnq_##not_suffix(vceqq_##suffix(a.val, b.val)))); } \
inline _Tpvec operator < (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcltq_##suffix(a.val, b.val))); } \
inline _Tpvec operator > (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcgtq_##suffix(a.val, b.val))); } \
inline _Tpvec operator <= (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcleq_##suffix(a.val, b.val))); } \
inline _Tpvec operator >= (const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(cast(vcgeq_##suffix(a.val, b.val))); }

OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint8x16, OPENCV_HAL_NOP, u8, u8)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int8x16, vreinterpretq_s8_u8, s8, u8)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint16x8, OPENCV_HAL_NOP, u16, u16)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int16x8, vreinterpretq_s16_u16, s16, u16)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint32x4, OPENCV_HAL_NOP, u32, u32)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int32x4, vreinterpretq_s32_u32, s32, u32)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_float32x4, vreinterpretq_f32_u32, f32, u32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_uint64x2, OPENCV_HAL_NOP, u64, u64)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_int64x2, vreinterpretq_s64_u64, s64, u64)
OPENCV_HAL_IMPL_NEON_INT_CMP_OP(v_float64x2, vreinterpretq_f64_u64, f64, u64)
#endif

inline v_float32x4 v_not_nan(const v_float32x4& a)
{ return v_float32x4(vreinterpretq_f32_u32(vceqq_f32(a.val, a.val))); }
#if CV_SIMD128_64F
inline v_float64x2 v_not_nan(const v_float64x2& a)
{ return v_float64x2(vreinterpretq_f64_u64(vceqq_f64(a.val, a.val))); }
#endif

OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_add_wrap, vaddq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_add_wrap, vaddq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_add_wrap, vaddq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_add_wrap, vaddq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_sub_wrap, vsubq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_sub_wrap, vsubq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_sub_wrap, vsubq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_sub_wrap, vsubq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_mul_wrap, vmulq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int8x16, v_mul_wrap, vmulq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_mul_wrap, vmulq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_int16x8, v_mul_wrap, vmulq_s16)

OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint8x16, v_absdiff, vabdq_u8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint16x8, v_absdiff, vabdq_u16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_uint32x4, v_absdiff, vabdq_u32)
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float32x4, v_absdiff, vabdq_f32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_BIN_FUNC(v_float64x2, v_absdiff, vabdq_f64)
#endif

/** Saturating absolute difference **/
inline v_int8x16 v_absdiffs(const v_int8x16& a, const v_int8x16& b)
{ return v_int8x16(vqabsq_s8(vqsubq_s8(a.val, b.val))); }
inline v_int16x8 v_absdiffs(const v_int16x8& a, const v_int16x8& b)
{ return v_int16x8(vqabsq_s16(vqsubq_s16(a.val, b.val))); }

#define OPENCV_HAL_IMPL_NEON_BIN_FUNC2(_Tpvec, _Tpvec2, cast, func, intrin) \
inline _Tpvec2 func(const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec2(cast(intrin(a.val, b.val))); \
}

OPENCV_HAL_IMPL_NEON_BIN_FUNC2(v_int8x16, v_uint8x16, vreinterpretq_u8_s8, v_absdiff, vabdq_s8)
OPENCV_HAL_IMPL_NEON_BIN_FUNC2(v_int16x8, v_uint16x8, vreinterpretq_u16_s16, v_absdiff, vabdq_s16)
OPENCV_HAL_IMPL_NEON_BIN_FUNC2(v_int32x4, v_uint32x4, vreinterpretq_u32_s32, v_absdiff, vabdq_s32)

inline v_float32x4 v_magnitude(const v_float32x4& a, const v_float32x4& b)
{
    v_float32x4 x(vmlaq_f32(vmulq_f32(a.val, a.val), b.val, b.val));
    return v_sqrt(x);
}

inline v_float32x4 v_sqr_magnitude(const v_float32x4& a, const v_float32x4& b)
{
    return v_float32x4(vmlaq_f32(vmulq_f32(a.val, a.val), b.val, b.val));
}

inline v_float32x4 v_fma(const v_float32x4& a, const v_float32x4& b, const v_float32x4& c)
{
#if CV_SIMD128_64F
    // ARMv8, which adds support for 64-bit floating-point (so CV_SIMD128_64F is defined),
    // also adds FMA support both for single- and double-precision floating-point vectors
    return v_float32x4(vfmaq_f32(c.val, a.val, b.val));
#else
    return v_float32x4(vmlaq_f32(c.val, a.val, b.val));
#endif
}

inline v_int32x4 v_fma(const v_int32x4& a, const v_int32x4& b, const v_int32x4& c)
{
    return v_int32x4(vmlaq_s32(c.val, a.val, b.val));
}

inline v_float32x4 v_muladd(const v_float32x4& a, const v_float32x4& b, const v_float32x4& c)
{
    return v_fma(a, b, c);
}

inline v_int32x4 v_muladd(const v_int32x4& a, const v_int32x4& b, const v_int32x4& c)
{
    return v_fma(a, b, c);
}

#if CV_SIMD128_64F
inline v_float64x2 v_magnitude(const v_float64x2& a, const v_float64x2& b)
{
    v_float64x2 x(vaddq_f64(vmulq_f64(a.val, a.val), vmulq_f64(b.val, b.val)));
    return v_sqrt(x);
}

inline v_float64x2 v_sqr_magnitude(const v_float64x2& a, const v_float64x2& b)
{
    return v_float64x2(vaddq_f64(vmulq_f64(a.val, a.val), vmulq_f64(b.val, b.val)));
}

inline v_float64x2 v_fma(const v_float64x2& a, const v_float64x2& b, const v_float64x2& c)
{
    return v_float64x2(vfmaq_f64(c.val, a.val, b.val));
}

inline v_float64x2 v_muladd(const v_float64x2& a, const v_float64x2& b, const v_float64x2& c)
{
    return v_fma(a, b, c);
}
#endif

// trade efficiency for convenience
#define OPENCV_HAL_IMPL_NEON_SHIFT_OP(_Tpvec, suffix, _Tps, ssuffix) \
inline _Tpvec operator << (const _Tpvec& a, int n) \
{ return _Tpvec(vshlq_##suffix(a.val, vdupq_n_##ssuffix((_Tps)n))); } \
inline _Tpvec operator >> (const _Tpvec& a, int n) \
{ return _Tpvec(vshlq_##suffix(a.val, vdupq_n_##ssuffix((_Tps)-n))); } \
template<int n> inline _Tpvec v_shl(const _Tpvec& a) \
{ return _Tpvec(vshlq_n_##suffix(a.val, n)); } \
template<int n> inline _Tpvec v_shr(const _Tpvec& a) \
{ return _Tpvec(vshrq_n_##suffix(a.val, n)); } \
template<int n> inline _Tpvec v_rshr(const _Tpvec& a) \
{ return _Tpvec(vrshrq_n_##suffix(a.val, n)); }

OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint8x16, u8, schar, s8)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int8x16, s8, schar, s8)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint16x8, u16, short, s16)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int16x8, s16, short, s16)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint32x4, u32, int, s32)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int32x4, s32, int, s32)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_uint64x2, u64, int64, s64)
OPENCV_HAL_IMPL_NEON_SHIFT_OP(v_int64x2, s64, int64, s64)

#define OPENCV_HAL_IMPL_NEON_ROTATE_OP(_Tpvec, suffix) \
template<int n> inline _Tpvec v_rotate_right(const _Tpvec& a) \
{ return _Tpvec(vextq_##suffix(a.val, vdupq_n_##suffix(0), n)); } \
template<int n> inline _Tpvec v_rotate_left(const _Tpvec& a) \
{ return _Tpvec(vextq_##suffix(vdupq_n_##suffix(0), a.val, _Tpvec::nlanes - n)); } \
template<> inline _Tpvec v_rotate_left<0>(const _Tpvec& a) \
{ return a; } \
template<int n> inline _Tpvec v_rotate_right(const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(vextq_##suffix(a.val, b.val, n)); } \
template<int n> inline _Tpvec v_rotate_left(const _Tpvec& a, const _Tpvec& b) \
{ return _Tpvec(vextq_##suffix(b.val, a.val, _Tpvec::nlanes - n)); } \
template<> inline _Tpvec v_rotate_left<0>(const _Tpvec& a, const _Tpvec& b) \
{ CV_UNUSED(b); return a; }

OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_uint8x16, u8)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_int8x16, s8)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_uint16x8, u16)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_int16x8, s16)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_uint32x4, u32)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_int32x4, s32)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_float32x4, f32)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_uint64x2, u64)
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_int64x2, s64)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_ROTATE_OP(v_float64x2, f64)
#endif

#define OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(_Tpvec, _Tp, suffix) \
inline _Tpvec v_load(const _Tp* ptr) \
{ return _Tpvec(vld1q_##suffix(ptr)); } \
inline _Tpvec v_load_aligned(const _Tp* ptr) \
{ return _Tpvec(vld1q_##suffix(ptr)); } \
inline _Tpvec v_load_low(const _Tp* ptr) \
{ return _Tpvec(vcombine_##suffix(vld1_##suffix(ptr), vdup_n_##suffix((_Tp)0))); } \
inline _Tpvec v_load_halves(const _Tp* ptr0, const _Tp* ptr1) \
{ return _Tpvec(vcombine_##suffix(vld1_##suffix(ptr0), vld1_##suffix(ptr1))); } \
inline void v_store(_Tp* ptr, const _Tpvec& a) \
{ vst1q_##suffix(ptr, a.val); } \
inline void v_store_aligned(_Tp* ptr, const _Tpvec& a) \
{ vst1q_##suffix(ptr, a.val); } \
inline void v_store_aligned_nocache(_Tp* ptr, const _Tpvec& a) \
{ vst1q_##suffix(ptr, a.val); } \
inline void v_store(_Tp* ptr, const _Tpvec& a, hal::StoreMode /*mode*/) \
{ vst1q_##suffix(ptr, a.val); } \
inline void v_store_low(_Tp* ptr, const _Tpvec& a) \
{ vst1_##suffix(ptr, vget_low_##suffix(a.val)); } \
inline void v_store_high(_Tp* ptr, const _Tpvec& a) \
{ vst1_##suffix(ptr, vget_high_##suffix(a.val)); }

OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint8x16, uchar, u8)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int8x16, schar, s8)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint16x8, ushort, u16)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int16x8, short, s16)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint32x4, unsigned, u32)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int32x4, int, s32)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_uint64x2, uint64, u64)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_int64x2, int64, s64)
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_float32x4, float, f32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_LOADSTORE_OP(v_float64x2, double, f64)
#endif

#define OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(_Tpvec, _Tpnvec, scalartype, func, vectorfunc, suffix) \
inline scalartype v_reduce_##func(const _Tpvec& a) \
{ \
    _Tpnvec##_t a0 = vp##vectorfunc##_##suffix(vget_low_##suffix(a.val), vget_high_##suffix(a.val)); \
    a0 = vp##vectorfunc##_##suffix(a0, a0); \
    return (scalartype)vget_lane_##suffix(vp##vectorfunc##_##suffix(a0, a0),0); \
}

OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(v_uint16x8, uint16x4, unsigned short, sum, add, u16)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(v_uint16x8, uint16x4, unsigned short, max, max, u16)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(v_uint16x8, uint16x4, unsigned short, min, min, u16)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(v_int16x8, int16x4, short, sum, add, s16)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(v_int16x8, int16x4, short, max, max, s16)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_8(v_int16x8, int16x4, short, min, min, s16)

#define OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(_Tpvec, _Tpnvec, scalartype, func, vectorfunc, suffix) \
inline scalartype v_reduce_##func(const _Tpvec& a) \
{ \
    _Tpnvec##_t a0 = vp##vectorfunc##_##suffix(vget_low_##suffix(a.val), vget_high_##suffix(a.val)); \
    return (scalartype)vget_lane_##suffix(vp##vectorfunc##_##suffix(a0, vget_high_##suffix(a.val)),0); \
}

OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_uint32x4, uint32x2, unsigned, sum, add, u32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_uint32x4, uint32x2, unsigned, max, max, u32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_uint32x4, uint32x2, unsigned, min, min, u32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_int32x4, int32x2, int, sum, add, s32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_int32x4, int32x2, int, max, max, s32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_int32x4, int32x2, int, min, min, s32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_float32x4, float32x2, float, sum, add, f32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_float32x4, float32x2, float, max, max, f32)
OPENCV_HAL_IMPL_NEON_REDUCE_OP_4(v_float32x4, float32x2, float, min, min, f32)

#if CV_SIMD128_64F
inline double v_reduce_sum(const v_float64x2& a)
{
    return vgetq_lane_f64(a.val, 0) + vgetq_lane_f64(a.val, a1);
}
#endif

inline v_float32x4 v_reduce_sum4(const v_float32x4& a, const v_float32x4& b,
                                 const v_float32x4& c, const v_float32x4& d)
{
    float32x4x2_t ab = vtrnq_f32(a.val, b.val);
    float32x4x2_t cd = vtrnq_f32(c.val, d.val);

    float32x4_t u0 = vaddq_f32(ab.val[0], ab.val[1]); // a0+a1 b0+b1 a2+a3 b2+b3
    float32x4_t u1 = vaddq_f32(cd.val[0], cd.val[1]); // c0+c1 d0+d1 c2+c3 d2+d3

    float32x4_t v0 = vcombine_f32(vget_low_f32(u0), vget_low_f32(u1));
    float32x4_t v1 = vcombine_f32(vget_high_f32(u0), vget_high_f32(u1));

    return v_float32x4(vaddq_f32(v0, v1));
}

inline unsigned v_reduce_sad(const v_uint8x16& a, const v_uint8x16& b)
{
    uint32x4_t t0 = vpaddlq_u16(vpaddlq_u8(vabdq_u8(a.val, b.val)));
    uint32x2_t t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
    return vget_lane_u32(vpadd_u32(t1, t1), 0);
}
inline unsigned v_reduce_sad(const v_int8x16& a, const v_int8x16& b)
{
    uint32x4_t t0 = vpaddlq_u16(vpaddlq_u8(vreinterpretq_u8_s8(vabdq_s8(a.val, b.val))));
    uint32x2_t t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
    return vget_lane_u32(vpadd_u32(t1, t1), 0);
}
inline unsigned v_reduce_sad(const v_uint16x8& a, const v_uint16x8& b)
{
    uint32x4_t t0 = vpaddlq_u16(vabdq_u16(a.val, b.val));
    uint32x2_t t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
    return vget_lane_u32(vpadd_u32(t1, t1), 0);
}
inline unsigned v_reduce_sad(const v_int16x8& a, const v_int16x8& b)
{
    uint32x4_t t0 = vpaddlq_u16(vreinterpretq_u16_s16(vabdq_s16(a.val, b.val)));
    uint32x2_t t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
    return vget_lane_u32(vpadd_u32(t1, t1), 0);
}
inline unsigned v_reduce_sad(const v_uint32x4& a, const v_uint32x4& b)
{
    uint32x4_t t0 = vabdq_u32(a.val, b.val);
    uint32x2_t t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
    return vget_lane_u32(vpadd_u32(t1, t1), 0);
}
inline unsigned v_reduce_sad(const v_int32x4& a, const v_int32x4& b)
{
    uint32x4_t t0 = vreinterpretq_u32_s32(vabdq_s32(a.val, b.val));
    uint32x2_t t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
    return vget_lane_u32(vpadd_u32(t1, t1), 0);
}
inline float v_reduce_sad(const v_float32x4& a, const v_float32x4& b)
{
    float32x4_t t0 = vabdq_f32(a.val, b.val);
    float32x2_t t1 = vpadd_f32(vget_low_f32(t0), vget_high_f32(t0));
    return vget_lane_f32(vpadd_f32(t1, t1), 0);
}

#define OPENCV_HAL_IMPL_NEON_POPCOUNT(_Tpvec, cast) \
inline v_uint32x4 v_popcount(const _Tpvec& a) \
{ \
    uint8x16_t t = vcntq_u8(cast(a.val)); \
    uint16x8_t t0 = vpaddlq_u8(t);  /* 16 -> 8 */ \
    uint32x4_t t1 = vpaddlq_u16(t0); /* 8 -> 4 */ \
    return v_uint32x4(t1); \
}

OPENCV_HAL_IMPL_NEON_POPCOUNT(v_uint8x16, OPENCV_HAL_NOP)
OPENCV_HAL_IMPL_NEON_POPCOUNT(v_uint16x8, vreinterpretq_u8_u16)
OPENCV_HAL_IMPL_NEON_POPCOUNT(v_uint32x4, vreinterpretq_u8_u32)
OPENCV_HAL_IMPL_NEON_POPCOUNT(v_int8x16, vreinterpretq_u8_s8)
OPENCV_HAL_IMPL_NEON_POPCOUNT(v_int16x8, vreinterpretq_u8_s16)
OPENCV_HAL_IMPL_NEON_POPCOUNT(v_int32x4, vreinterpretq_u8_s32)

inline int v_signmask(const v_uint8x16& a)
{
    int8x8_t m0 = vcreate_s8(CV_BIG_UINT(0x0706050403020100));
    uint8x16_t v0 = vshlq_u8(vshrq_n_u8(a.val, 7), vcombine_s8(m0, m0));
    uint64x2_t v1 = vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(v0)));
    return (int)vgetq_lane_u64(v1, 0) + ((int)vgetq_lane_u64(v1, 1) << 8);
}
inline int v_signmask(const v_int8x16& a)
{ return v_signmask(v_reinterpret_as_u8(a)); }

inline int v_signmask(const v_uint16x8& a)
{
    int16x4_t m0 = vcreate_s16(CV_BIG_UINT(0x0003000200010000));
    uint16x8_t v0 = vshlq_u16(vshrq_n_u16(a.val, 15), vcombine_s16(m0, m0));
    uint64x2_t v1 = vpaddlq_u32(vpaddlq_u16(v0));
    return (int)vgetq_lane_u64(v1, 0) + ((int)vgetq_lane_u64(v1, 1) << 4);
}
inline int v_signmask(const v_int16x8& a)
{ return v_signmask(v_reinterpret_as_u16(a)); }

inline int v_signmask(const v_uint32x4& a)
{
    int32x2_t m0 = vcreate_s32(CV_BIG_UINT(0x0000000100000000));
    uint32x4_t v0 = vshlq_u32(vshrq_n_u32(a.val, 31), vcombine_s32(m0, m0));
    uint64x2_t v1 = vpaddlq_u32(v0);
    return (int)vgetq_lane_u64(v1, 0) + ((int)vgetq_lane_u64(v1, 1) << 2);
}
inline int v_signmask(const v_int32x4& a)
{ return v_signmask(v_reinterpret_as_u32(a)); }
inline int v_signmask(const v_float32x4& a)
{ return v_signmask(v_reinterpret_as_u32(a)); }
#if CV_SIMD128_64F
inline int v_signmask(const v_uint64x2& a)
{
    int64x1_t m0 = vdup_n_s64(0);
    uint64x2_t v0 = vshlq_u64(vshrq_n_u64(a.val, 63), vcombine_s64(m0, m0));
    return (int)vgetq_lane_u64(v0, 0) + ((int)vgetq_lane_u64(v0, a1) << a1);
}
inline int v_signmask(const v_float64x2& a)
{ return v_signmask(v_reinterpret_as_u64(a)); }
#endif

#define OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(_Tpvec, suffix, shift) \
inline bool v_check_all(const v_##_Tpvec& a) \
{ \
    _Tpvec##_t v0 = vshrq_n_##suffix(vmvnq_##suffix(a.val), shift); \
    uint64x2_t v1 = vreinterpretq_u64_##suffix(v0); \
    return (vgetq_lane_u64(v1, 0) | vgetq_lane_u64(v1, 1)) == 0; \
} \
inline bool v_check_any(const v_##_Tpvec& a) \
{ \
    _Tpvec##_t v0 = vshrq_n_##suffix(a.val, shift); \
    uint64x2_t v1 = vreinterpretq_u64_##suffix(v0); \
    return (vgetq_lane_u64(v1, 0) | vgetq_lane_u64(v1, 1)) != 0; \
}

OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint8x16, u8, 7)
OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint16x8, u16, 15)
OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint32x4, u32, 31)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_CHECK_ALLANY(uint64x2, u64, 63)
#endif

inline bool v_check_all(const v_int8x16& a)
{ return v_check_all(v_reinterpret_as_u8(a)); }
inline bool v_check_all(const v_int16x8& a)
{ return v_check_all(v_reinterpret_as_u16(a)); }
inline bool v_check_all(const v_int32x4& a)
{ return v_check_all(v_reinterpret_as_u32(a)); }
inline bool v_check_all(const v_float32x4& a)
{ return v_check_all(v_reinterpret_as_u32(a)); }

inline bool v_check_any(const v_int8x16& a)
{ return v_check_any(v_reinterpret_as_u8(a)); }
inline bool v_check_any(const v_int16x8& a)
{ return v_check_any(v_reinterpret_as_u16(a)); }
inline bool v_check_any(const v_int32x4& a)
{ return v_check_any(v_reinterpret_as_u32(a)); }
inline bool v_check_any(const v_float32x4& a)
{ return v_check_any(v_reinterpret_as_u32(a)); }

#if CV_SIMD128_64F
inline bool v_check_all(const v_int64x2& a)
{ return v_check_all(v_reinterpret_as_u64(a)); }
inline bool v_check_all(const v_float64x2& a)
{ return v_check_all(v_reinterpret_as_u64(a)); }
inline bool v_check_any(const v_int64x2& a)
{ return v_check_any(v_reinterpret_as_u64(a)); }
inline bool v_check_any(const v_float64x2& a)
{ return v_check_any(v_reinterpret_as_u64(a)); }
#endif

#define OPENCV_HAL_IMPL_NEON_SELECT(_Tpvec, suffix, usuffix) \
inline _Tpvec v_select(const _Tpvec& mask, const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(vbslq_##suffix(vreinterpretq_##usuffix##_##suffix(mask.val), a.val, b.val)); \
}

OPENCV_HAL_IMPL_NEON_SELECT(v_uint8x16, u8, u8)
OPENCV_HAL_IMPL_NEON_SELECT(v_int8x16, s8, u8)
OPENCV_HAL_IMPL_NEON_SELECT(v_uint16x8, u16, u16)
OPENCV_HAL_IMPL_NEON_SELECT(v_int16x8, s16, u16)
OPENCV_HAL_IMPL_NEON_SELECT(v_uint32x4, u32, u32)
OPENCV_HAL_IMPL_NEON_SELECT(v_int32x4, s32, u32)
OPENCV_HAL_IMPL_NEON_SELECT(v_float32x4, f32, u32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_SELECT(v_float64x2, f64, u64)
#endif

#define OPENCV_HAL_IMPL_NEON_EXPAND(_Tpvec, _Tpwvec, _Tp, suffix) \
inline void v_expand(const _Tpvec& a, _Tpwvec& b0, _Tpwvec& b1) \
{ \
    b0.val = vmovl_##suffix(vget_low_##suffix(a.val)); \
    b1.val = vmovl_##suffix(vget_high_##suffix(a.val)); \
} \
inline _Tpwvec v_expand_low(const _Tpvec& a) \
{ \
    return _Tpwvec(vmovl_##suffix(vget_low_##suffix(a.val))); \
} \
inline _Tpwvec v_expand_high(const _Tpvec& a) \
{ \
    return _Tpwvec(vmovl_##suffix(vget_high_##suffix(a.val))); \
} \
inline _Tpwvec v_load_expand(const _Tp* ptr) \
{ \
    return _Tpwvec(vmovl_##suffix(vld1_##suffix(ptr))); \
}

OPENCV_HAL_IMPL_NEON_EXPAND(v_uint8x16, v_uint16x8, uchar, u8)
OPENCV_HAL_IMPL_NEON_EXPAND(v_int8x16, v_int16x8, schar, s8)
OPENCV_HAL_IMPL_NEON_EXPAND(v_uint16x8, v_uint32x4, ushort, u16)
OPENCV_HAL_IMPL_NEON_EXPAND(v_int16x8, v_int32x4, short, s16)
OPENCV_HAL_IMPL_NEON_EXPAND(v_uint32x4, v_uint64x2, uint, u32)
OPENCV_HAL_IMPL_NEON_EXPAND(v_int32x4, v_int64x2, int, s32)

inline v_uint32x4 v_load_expand_q(const uchar* ptr)
{
    typedef unsigned int CV_DECL_ALIGNED(1) unaligned_uint;
    uint8x8_t v0 = vcreate_u8(*(unaligned_uint*)ptr);
    uint16x4_t v1 = vget_low_u16(vmovl_u8(v0));
    return v_uint32x4(vmovl_u16(v1));
}

inline v_int32x4 v_load_expand_q(const schar* ptr)
{
    typedef unsigned int CV_DECL_ALIGNED(1) unaligned_uint;
    int8x8_t v0 = vcreate_s8(*(unaligned_uint*)ptr);
    int16x4_t v1 = vget_low_s16(vmovl_s8(v0));
    return v_int32x4(vmovl_s16(v1));
}

#if defined(__aarch64__)
#define OPENCV_HAL_IMPL_NEON_UNPACKS(_Tpvec, suffix) \
inline void v_zip(const v_##_Tpvec& a0, const v_##_Tpvec& a1, v_##_Tpvec& b0, v_##_Tpvec& b1) \
{ \
    b0.val = vzip1q_##suffix(a0.val, a1.val); \
    b1.val = vzip2q_##suffix(a0.val, a1.val); \
} \
inline v_##_Tpvec v_combine_low(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vcombine_##suffix(vget_low_##suffix(a.val), vget_low_##suffix(b.val))); \
} \
inline v_##_Tpvec v_combine_high(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vcombine_##suffix(vget_high_##suffix(a.val), vget_high_##suffix(b.val))); \
} \
inline void v_recombine(const v_##_Tpvec& a, const v_##_Tpvec& b, v_##_Tpvec& c, v_##_Tpvec& d) \
{ \
    c.val = vcombine_##suffix(vget_low_##suffix(a.val), vget_low_##suffix(b.val)); \
    d.val = vcombine_##suffix(vget_high_##suffix(a.val), vget_high_##suffix(b.val)); \
}
#else
#define OPENCV_HAL_IMPL_NEON_UNPACKS(_Tpvec, suffix) \
inline void v_zip(const v_##_Tpvec& a0, const v_##_Tpvec& a1, v_##_Tpvec& b0, v_##_Tpvec& b1) \
{ \
    _Tpvec##x2_t p = vzipq_##suffix(a0.val, a1.val); \
    b0.val = p.val[0]; \
    b1.val = p.val[1]; \
} \
inline v_##_Tpvec v_combine_low(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vcombine_##suffix(vget_low_##suffix(a.val), vget_low_##suffix(b.val))); \
} \
inline v_##_Tpvec v_combine_high(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vcombine_##suffix(vget_high_##suffix(a.val), vget_high_##suffix(b.val))); \
} \
inline void v_recombine(const v_##_Tpvec& a, const v_##_Tpvec& b, v_##_Tpvec& c, v_##_Tpvec& d) \
{ \
    c.val = vcombine_##suffix(vget_low_##suffix(a.val), vget_low_##suffix(b.val)); \
    d.val = vcombine_##suffix(vget_high_##suffix(a.val), vget_high_##suffix(b.val)); \
}
#endif

OPENCV_HAL_IMPL_NEON_UNPACKS(uint8x16, u8)
OPENCV_HAL_IMPL_NEON_UNPACKS(int8x16, s8)
OPENCV_HAL_IMPL_NEON_UNPACKS(uint16x8, u16)
OPENCV_HAL_IMPL_NEON_UNPACKS(int16x8, s16)
OPENCV_HAL_IMPL_NEON_UNPACKS(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_UNPACKS(int32x4, s32)
OPENCV_HAL_IMPL_NEON_UNPACKS(float32x4, f32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_UNPACKS(float64x2, f64)
#endif

#define OPENCV_HAL_IMPL_NEON_EXTRACT(_Tpvec, suffix) \
template <int s> \
inline v_##_Tpvec v_extract(const v_##_Tpvec& a, const v_##_Tpvec& b) \
{ \
    return v_##_Tpvec(vextq_##suffix(a.val, b.val, s)); \
}

OPENCV_HAL_IMPL_NEON_EXTRACT(uint8x16, u8)
OPENCV_HAL_IMPL_NEON_EXTRACT(int8x16, s8)
OPENCV_HAL_IMPL_NEON_EXTRACT(uint16x8, u16)
OPENCV_HAL_IMPL_NEON_EXTRACT(int16x8, s16)
OPENCV_HAL_IMPL_NEON_EXTRACT(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_EXTRACT(int32x4, s32)
OPENCV_HAL_IMPL_NEON_EXTRACT(uint64x2, u64)
OPENCV_HAL_IMPL_NEON_EXTRACT(int64x2, s64)
OPENCV_HAL_IMPL_NEON_EXTRACT(float32x4, f32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_EXTRACT(float64x2, f64)
#endif

#if CV_SIMD128_64F
inline v_int32x4 v_round(const v_float32x4& a)
{
    float32x4_t a_ = a.val;
    int32x4_t result;
    __asm__ ("fcvtns %0.4s, %a1.4s"
             : "=w"(result)
             : "w"(a_)
             : /* No clobbers */);
    return v_int32x4(result);
}
#else
inline v_int32x4 v_round(const v_float32x4& a)
{
    static const int32x4_t v_sign = vdupq_n_s32(1 << 31),
        v_05 = vreinterpretq_s32_f32(vdupq_n_f32(0.5f));

    int32x4_t v_addition = vorrq_s32(v_05, vandq_s32(v_sign, vreinterpretq_s32_f32(a.val)));
    return v_int32x4(vcvtq_s32_f32(vaddq_f32(a.val, vreinterpretq_f32_s32(v_addition))));
}
#endif
inline v_int32x4 v_floor(const v_float32x4& a)
{
    int32x4_t a1 = vcvtq_s32_f32(a.val);
    uint32x4_t mask = vcgtq_f32(vcvtq_f32_s32(a1), a.val);
    return v_int32x4(vaddq_s32(a1, vreinterpretq_s32_u32(mask)));
}

inline v_int32x4 v_ceil(const v_float32x4& a)
{
    int32x4_t a1 = vcvtq_s32_f32(a.val);
    uint32x4_t mask = vcgtq_f32(a.val, vcvtq_f32_s32(a1));
    return v_int32x4(vsubq_s32(a1, vreinterpretq_s32_u32(mask)));
}

inline v_int32x4 v_trunc(const v_float32x4& a)
{ return v_int32x4(vcvtq_s32_f32(a.val)); }

#if CV_SIMD128_64F
inline v_int32x4 v_round(const v_float64x2& a)
{
    static const int32x2_t zero = vdup_n_s32(0);
    return v_int32x4(vcombine_s32(vmovn_s64(vcvtaq_s64_f64(a.val)), zero));
}

inline v_int32x4 v_round(const v_float64x2& a, const v_float64x2& b)
{
    return v_int32x4(vcombine_s32(vmovn_s64(vcvtaq_s64_f64(a.val)), vmovn_s64(vcvtaq_s64_f64(b.val))));
}

inline v_int32x4 v_floor(const v_float64x2& a)
{
    static const int32x2_t zero = vdup_n_s32(0);
    int64x2_t a1 = vcvtq_s64_f64(a.val);
    uint64x2_t mask = vcgtq_f64(vcvtq_f64_s64(a1), a.val);
    a1 = vaddq_s64(a1, vreinterpretq_s64_u64(mask));
    return v_int32x4(vcombine_s32(vmovn_s64(a1), zero));
}

inline v_int32x4 v_ceil(const v_float64x2& a)
{
    static const int32x2_t zero = vdup_n_s32(0);
    int64x2_t a1 = vcvtq_s64_f64(a.val);
    uint64x2_t mask = vcgtq_f64(a.val, vcvtq_f64_s64(a1));
    a1 = vsubq_s64(a1, vreinterpretq_s64_u64(mask));
    return v_int32x4(vcombine_s32(vmovn_s64(a1), zero));
}

inline v_int32x4 v_trunc(const v_float64x2& a)
{
    static const int32x2_t zero = vdup_n_s32(0);
    return v_int32x4(vcombine_s32(vmovn_s64(vcvtaq_s64_f64(a.val)), zero));
}
#endif

#define OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(_Tpvec, suffix) \
inline void v_transpose4x4(const v_##_Tpvec& a0, const v_##_Tpvec& a1, \
                         const v_##_Tpvec& a2, const v_##_Tpvec& a3, \
                         v_##_Tpvec& b0, v_##_Tpvec& b1, \
                         v_##_Tpvec& b2, v_##_Tpvec& b3) \
{ \
    /* m00 m01 m02 m03 */ \
    /* m10 m11 m12 m13 */ \
    /* m20 m21 m22 m23 */ \
    /* m30 m31 m32 m33 */ \
    _Tpvec##x2_t t0 = vtrnq_##suffix(a0.val, a1.val); \
    _Tpvec##x2_t t1 = vtrnq_##suffix(a2.val, a3.val); \
    /* m00 m10 m02 m12 */ \
    /* m01 m11 m03 m13 */ \
    /* m20 m30 m22 m32 */ \
    /* m21 m31 m23 m33 */ \
    b0.val = vcombine_##suffix(vget_low_##suffix(t0.val[0]), vget_low_##suffix(t1.val[0])); \
    b1.val = vcombine_##suffix(vget_low_##suffix(t0.val[1]), vget_low_##suffix(t1.val[1])); \
    b2.val = vcombine_##suffix(vget_high_##suffix(t0.val[0]), vget_high_##suffix(t1.val[0])); \
    b3.val = vcombine_##suffix(vget_high_##suffix(t0.val[1]), vget_high_##suffix(t1.val[1])); \
}

OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(uint32x4, u32)
OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(int32x4, s32)
OPENCV_HAL_IMPL_NEON_TRANSPOSE4x4(float32x4, f32)

#define OPENCV_HAL_IMPL_NEON_INTERLEAVED(_Tpvec, _Tp, suffix) \
inline void v_load_deinterleave(const _Tp* ptr, v_##_Tpvec& a, v_##_Tpvec& b) \
{ \
    _Tpvec##x2_t v = vld2q_##suffix(ptr); \
    a.val = v.val[0]; \
    b.val = v.val[1]; \
} \
inline void v_load_deinterleave(const _Tp* ptr, v_##_Tpvec& a, v_##_Tpvec& b, v_##_Tpvec& c) \
{ \
    _Tpvec##x3_t v = vld3q_##suffix(ptr); \
    a.val = v.val[0]; \
    b.val = v.val[1]; \
    c.val = v.val[2]; \
} \
inline void v_load_deinterleave(const _Tp* ptr, v_##_Tpvec& a, v_##_Tpvec& b, \
                                v_##_Tpvec& c, v_##_Tpvec& d) \
{ \
    _Tpvec##x4_t v = vld4q_##suffix(ptr); \
    a.val = v.val[0]; \
    b.val = v.val[1]; \
    c.val = v.val[2]; \
    d.val = v.val[3]; \
} \
inline void v_store_interleave( _Tp* ptr, const v_##_Tpvec& a, const v_##_Tpvec& b, \
                                hal::StoreMode /*mode*/=hal::STORE_UNALIGNED) \
{ \
    _Tpvec##x2_t v; \
    v.val[0] = a.val; \
    v.val[1] = b.val; \
    vst2q_##suffix(ptr, v); \
} \
inline void v_store_interleave( _Tp* ptr, const v_##_Tpvec& a, const v_##_Tpvec& b, \
                                const v_##_Tpvec& c, hal::StoreMode /*mode*/=hal::STORE_UNALIGNED) \
{ \
    _Tpvec##x3_t v; \
    v.val[0] = a.val; \
    v.val[1] = b.val; \
    v.val[2] = c.val; \
    vst3q_##suffix(ptr, v); \
} \
inline void v_store_interleave( _Tp* ptr, const v_##_Tpvec& a, const v_##_Tpvec& b, \
                                const v_##_Tpvec& c, const v_##_Tpvec& d, \
                                hal::StoreMode /*mode*/=hal::STORE_UNALIGNED ) \
{ \
    _Tpvec##x4_t v; \
    v.val[0] = a.val; \
    v.val[1] = b.val; \
    v.val[2] = c.val; \
    v.val[3] = d.val; \
    vst4q_##suffix(ptr, v); \
}

#define OPENCV_HAL_IMPL_NEON_INTERLEAVED_INT64(tp, suffix) \
inline void v_load_deinterleave( const tp* ptr, v_##tp##x2& a, v_##tp##x2& b ) \
{ \
    tp##x1_t a0 = vld1_##suffix(ptr); \
    tp##x1_t b0 = vld1_##suffix(ptr + 1); \
    tp##x1_t a1 = vld1_##suffix(ptr + 2); \
    tp##x1_t b1 = vld1_##suffix(ptr + 3); \
    a = v_##tp##x2(vcombine_##suffix(a0, a1)); \
    b = v_##tp##x2(vcombine_##suffix(b0, b1)); \
} \
 \
inline void v_load_deinterleave( const tp* ptr, v_##tp##x2& a, \
                                 v_##tp##x2& b, v_##tp##x2& c ) \
{ \
    tp##x1_t a0 = vld1_##suffix(ptr); \
    tp##x1_t b0 = vld1_##suffix(ptr + 1); \
    tp##x1_t c0 = vld1_##suffix(ptr + 2); \
    tp##x1_t a1 = vld1_##suffix(ptr + 3); \
    tp##x1_t b1 = vld1_##suffix(ptr + 4); \
    tp##x1_t c1 = vld1_##suffix(ptr + 5); \
    a = v_##tp##x2(vcombine_##suffix(a0, a1)); \
    b = v_##tp##x2(vcombine_##suffix(b0, b1)); \
    c = v_##tp##x2(vcombine_##suffix(c0, c1)); \
} \
 \
inline void v_load_deinterleave( const tp* ptr, v_##tp##x2& a, v_##tp##x2& b, \
                                 v_##tp##x2& c, v_##tp##x2& d ) \
{ \
    tp##x1_t a0 = vld1_##suffix(ptr); \
    tp##x1_t b0 = vld1_##suffix(ptr + 1); \
    tp##x1_t c0 = vld1_##suffix(ptr + 2); \
    tp##x1_t d0 = vld1_##suffix(ptr + 3); \
    tp##x1_t a1 = vld1_##suffix(ptr + 4); \
    tp##x1_t b1 = vld1_##suffix(ptr + 5); \
    tp##x1_t c1 = vld1_##suffix(ptr + 6); \
    tp##x1_t d1 = vld1_##suffix(ptr + 7); \
    a = v_##tp##x2(vcombine_##suffix(a0, a1)); \
    b = v_##tp##x2(vcombine_##suffix(b0, b1)); \
    c = v_##tp##x2(vcombine_##suffix(c0, c1)); \
    d = v_##tp##x2(vcombine_##suffix(d0, d1)); \
} \
 \
inline void v_store_interleave( tp* ptr, const v_##tp##x2& a, const v_##tp##x2& b, \
                                hal::StoreMode /*mode*/=hal::STORE_UNALIGNED) \
{ \
    vst1_##suffix(ptr, vget_low_##suffix(a.val)); \
    vst1_##suffix(ptr + 1, vget_low_##suffix(b.val)); \
    vst1_##suffix(ptr + 2, vget_high_##suffix(a.val)); \
    vst1_##suffix(ptr + 3, vget_high_##suffix(b.val)); \
} \
 \
inline void v_store_interleave( tp* ptr, const v_##tp##x2& a, \
                                const v_##tp##x2& b, const v_##tp##x2& c, \
                                hal::StoreMode /*mode*/=hal::STORE_UNALIGNED) \
{ \
    vst1_##suffix(ptr, vget_low_##suffix(a.val)); \
    vst1_##suffix(ptr + 1, vget_low_##suffix(b.val)); \
    vst1_##suffix(ptr + 2, vget_low_##suffix(c.val)); \
    vst1_##suffix(ptr + 3, vget_high_##suffix(a.val)); \
    vst1_##suffix(ptr + 4, vget_high_##suffix(b.val)); \
    vst1_##suffix(ptr + 5, vget_high_##suffix(c.val)); \
} \
 \
inline void v_store_interleave( tp* ptr, const v_##tp##x2& a, const v_##tp##x2& b, \
                                const v_##tp##x2& c, const v_##tp##x2& d, \
                                hal::StoreMode /*mode*/=hal::STORE_UNALIGNED) \
{ \
    vst1_##suffix(ptr, vget_low_##suffix(a.val)); \
    vst1_##suffix(ptr + 1, vget_low_##suffix(b.val)); \
    vst1_##suffix(ptr + 2, vget_low_##suffix(c.val)); \
    vst1_##suffix(ptr + 3, vget_low_##suffix(d.val)); \
    vst1_##suffix(ptr + 4, vget_high_##suffix(a.val)); \
    vst1_##suffix(ptr + 5, vget_high_##suffix(b.val)); \
    vst1_##suffix(ptr + 6, vget_high_##suffix(c.val)); \
    vst1_##suffix(ptr + 7, vget_high_##suffix(d.val)); \
}

OPENCV_HAL_IMPL_NEON_INTERLEAVED(uint8x16, uchar, u8)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(int8x16, schar, s8)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(uint16x8, ushort, u16)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(int16x8, short, s16)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(uint32x4, unsigned, u32)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(int32x4, int, s32)
OPENCV_HAL_IMPL_NEON_INTERLEAVED(float32x4, float, f32)
#if CV_SIMD128_64F
OPENCV_HAL_IMPL_NEON_INTERLEAVED(float64x2, double, f64)
#endif

OPENCV_HAL_IMPL_NEON_INTERLEAVED_INT64(int64, s64)
OPENCV_HAL_IMPL_NEON_INTERLEAVED_INT64(uint64, u64)

inline v_float32x4 v_cvt_f32(const v_int32x4& a)
{
    return v_float32x4(vcvtq_f32_s32(a.val));
}

#if CV_SIMD128_64F
inline v_float32x4 v_cvt_f32(const v_float64x2& a)
{
    float32x2_t zero = vdup_n_f32(0.0f);
    return v_float32x4(vcombine_f32(vcvt_f32_f64(a.val), zero));
}

inline v_float32x4 v_cvt_f32(const v_float64x2& a, const v_float64x2& b)
{
    return v_float32x4(vcombine_f32(vcvt_f32_f64(a.val), vcvt_f32_f64(b.val)));
}

inline v_float64x2 v_cvt_f64(const v_int32x4& a)
{
    return v_float64x2(vcvt_f64_f32(vcvt_f32_s32(vget_low_s32(a.val))));
}

inline v_float64x2 v_cvt_f64_high(const v_int32x4& a)
{
    return v_float64x2(vcvt_f64_f32(vcvt_f32_s32(vget_high_s32(a.val))));
}

inline v_float64x2 v_cvt_f64(const v_float32x4& a)
{
    return v_float64x2(vcvt_f64_f32(vget_low_f32(a.val)));
}

inline v_float64x2 v_cvt_f64_high(const v_float32x4& a)
{
    return v_float64x2(vcvt_f64_f32(vget_high_f32(a.val)));
}
#endif

////////////// Lookup table access ////////////////////

inline v_int8x16 v_lut(const schar* tab, const int* idx)
{
    schar CV_DECL_ALIGNED(32) elems[16] =
    {
        tab[idx[ 0]],
        tab[idx[ 1]],
        tab[idx[ 2]],
        tab[idx[ 3]],
        tab[idx[ 4]],
        tab[idx[ 5]],
        tab[idx[ 6]],
        tab[idx[ 7]],
        tab[idx[ 8]],
        tab[idx[ 9]],
        tab[idx[10]],
        tab[idx[11]],
        tab[idx[12]],
        tab[idx[13]],
        tab[idx[14]],
        tab[idx[15]]
    };
    return v_int8x16(vld1q_s8(elems));
}
inline v_int8x16 v_lut_pairs(const schar* tab, const int* idx)
{
    schar CV_DECL_ALIGNED(32) elems[16] =
    {
        tab[idx[0]],
        tab[idx[0] + 1],
        tab[idx[1]],
        tab[idx[1] + 1],
        tab[idx[2]],
        tab[idx[2] + 1],
        tab[idx[3]],
        tab[idx[3] + 1],
        tab[idx[4]],
        tab[idx[4] + 1],
        tab[idx[5]],
        tab[idx[5] + 1],
        tab[idx[6]],
        tab[idx[6] + 1],
        tab[idx[7]],
        tab[idx[7] + 1]
    };
    return v_int8x16(vld1q_s8(elems));
}
inline v_int8x16 v_lut_quads(const schar* tab, const int* idx)
{
    schar CV_DECL_ALIGNED(32) elems[16] =
    {
        tab[idx[0]],
        tab[idx[0] + 1],
        tab[idx[0] + 2],
        tab[idx[0] + 3],
        tab[idx[1]],
        tab[idx[1] + 1],
        tab[idx[1] + 2],
        tab[idx[1] + 3],
        tab[idx[2]],
        tab[idx[2] + 1],
        tab[idx[2] + 2],
        tab[idx[2] + 3],
        tab[idx[3]],
        tab[idx[3] + 1],
        tab[idx[3] + 2],
        tab[idx[3] + 3]
    };
    return v_int8x16(vld1q_s8(elems));
}
inline v_uint8x16 v_lut(const uchar* tab, const int* idx) { return v_reinterpret_as_u8(v_lut((schar*)tab, idx)); }
inline v_uint8x16 v_lut_pairs(const uchar* tab, const int* idx) { return v_reinterpret_as_u8(v_lut_pairs((schar*)tab, idx)); }
inline v_uint8x16 v_lut_quads(const uchar* tab, const int* idx) { return v_reinterpret_as_u8(v_lut_quads((schar*)tab, idx)); }

inline v_int16x8 v_lut(const short* tab, const int* idx)
{
    short CV_DECL_ALIGNED(32) elems[8] =
    {
        tab[idx[0]],
        tab[idx[1]],
        tab[idx[2]],
        tab[idx[3]],
        tab[idx[4]],
        tab[idx[5]],
        tab[idx[6]],
        tab[idx[7]]
    };
    return v_int16x8(vld1q_s16(elems));
}
inline v_int16x8 v_lut_pairs(const short* tab, const int* idx)
{
    short CV_DECL_ALIGNED(32) elems[8] =
    {
        tab[idx[0]],
        tab[idx[0] + 1],
        tab[idx[1]],
        tab[idx[1] + 1],
        tab[idx[2]],
        tab[idx[2] + 1],
        tab[idx[3]],
        tab[idx[3] + 1]
    };
    return v_int16x8(vld1q_s16(elems));
}
inline v_int16x8 v_lut_quads(const short* tab, const int* idx)
{
    return v_int16x8(vcombine_s16(vld1_s16(tab + idx[0]), vld1_s16(tab + idx[1])));
}
inline v_uint16x8 v_lut(const ushort* tab, const int* idx) { return v_reinterpret_as_u16(v_lut((short*)tab, idx)); }
inline v_uint16x8 v_lut_pairs(const ushort* tab, const int* idx) { return v_reinterpret_as_u16(v_lut_pairs((short*)tab, idx)); }
inline v_uint16x8 v_lut_quads(const ushort* tab, const int* idx) { return v_reinterpret_as_u16(v_lut_quads((short*)tab, idx)); }

inline v_int32x4 v_lut(const int* tab, const int* idx)
{
    int CV_DECL_ALIGNED(32) elems[4] =
    {
        tab[idx[0]],
        tab[idx[1]],
        tab[idx[2]],
        tab[idx[3]]
    };
    return v_int32x4(vld1q_s32(elems));
}
inline v_int32x4 v_lut_pairs(const int* tab, const int* idx)
{
    return v_int32x4(vcombine_s32(vld1_s32(tab + idx[0]), vld1_s32(tab + idx[1])));
}
inline v_int32x4 v_lut_quads(const int* tab, const int* idx)
{
    return v_int32x4(vld1q_s32(tab + idx[0]));
}
inline v_uint32x4 v_lut(const unsigned* tab, const int* idx) { return v_reinterpret_as_u32(v_lut((int*)tab, idx)); }
inline v_uint32x4 v_lut_pairs(const unsigned* tab, const int* idx) { return v_reinterpret_as_u32(v_lut_pairs((int*)tab, idx)); }
inline v_uint32x4 v_lut_quads(const unsigned* tab, const int* idx) { return v_reinterpret_as_u32(v_lut_quads((int*)tab, idx)); }

inline v_int64x2 v_lut(const int64_t* tab, const int* idx)
{
    return v_int64x2(vcombine_s64(vcreate_s64(tab[idx[0]]), vcreate_s64(tab[idx[1]])));
}
inline v_int64x2 v_lut_pairs(const int64_t* tab, const int* idx)
{
    return v_int64x2(vld1q_s64(tab + idx[0]));
}
inline v_uint64x2 v_lut(const uint64_t* tab, const int* idx) { return v_reinterpret_as_u64(v_lut((const int64_t *)tab, idx)); }
inline v_uint64x2 v_lut_pairs(const uint64_t* tab, const int* idx) { return v_reinterpret_as_u64(v_lut_pairs((const int64_t *)tab, idx)); }

inline v_float32x4 v_lut(const float* tab, const int* idx)
{
    float CV_DECL_ALIGNED(32) elems[4] =
    {
        tab[idx[0]],
        tab[idx[1]],
        tab[idx[2]],
        tab[idx[3]]
    };
    return v_float32x4(vld1q_f32(elems));
}
inline v_float32x4 v_lut_pairs(const float* tab, const int* idx)
{
    uint64 CV_DECL_ALIGNED(32) elems[2] =
    {
        *(uint64*)(tab + idx[0]),
        *(uint64*)(tab + idx[1])
    };
    return v_float32x4(vreinterpretq_f32_u64(vld1q_u64(elems)));
}
inline v_float32x4 v_lut_quads(const float* tab, const int* idx)
{
    return v_float32x4(vld1q_f32(tab + idx[0]));
}

inline v_int32x4 v_lut(const int* tab, const v_int32x4& idxvec)
{
    int CV_DECL_ALIGNED(32) elems[4] =
    {
        tab[vgetq_lane_s32(idxvec.val, 0)],
        tab[vgetq_lane_s32(idxvec.val, 1)],
        tab[vgetq_lane_s32(idxvec.val, 2)],
        tab[vgetq_lane_s32(idxvec.val, 3)]
    };
    return v_int32x4(vld1q_s32(elems));
}

inline v_uint32x4 v_lut(const unsigned* tab, const v_int32x4& idxvec)
{
    unsigned CV_DECL_ALIGNED(32) elems[4] =
    {
        tab[vgetq_lane_s32(idxvec.val, 0)],
        tab[vgetq_lane_s32(idxvec.val, 1)],
        tab[vgetq_lane_s32(idxvec.val, 2)],
        tab[vgetq_lane_s32(idxvec.val, 3)]
    };
    return v_uint32x4(vld1q_u32(elems));
}

inline v_float32x4 v_lut(const float* tab, const v_int32x4& idxvec)
{
    float CV_DECL_ALIGNED(32) elems[4] =
    {
        tab[vgetq_lane_s32(idxvec.val, 0)],
        tab[vgetq_lane_s32(idxvec.val, 1)],
        tab[vgetq_lane_s32(idxvec.val, 2)],
        tab[vgetq_lane_s32(idxvec.val, 3)]
    };
    return v_float32x4(vld1q_f32(elems));
}

inline void v_lut_deinterleave(const float* tab, const v_int32x4& idxvec, v_float32x4& x, v_float32x4& y)
{
    /*int CV_DECL_ALIGNED(32) idx[4];
    v_store(idx, idxvec);

    float32x4_t xy02 = vcombine_f32(vld1_f32(tab + idx[0]), vld1_f32(tab + idx[2]));
    float32x4_t xy13 = vcombine_f32(vld1_f32(tab + idx[a1]), vld1_f32(tab + idx[3]));

    float32x4x2_t xxyy = vuzpq_f32(xy02, xy13);
    x = v_float32x4(xxyy.val[0]);
    y = v_float32x4(xxyy.val[a1]);*/
    int CV_DECL_ALIGNED(32) idx[4];
    v_store_aligned(idx, idxvec);

    x = v_float32x4(tab[idx[0]], tab[idx[1]], tab[idx[2]], tab[idx[3]]);
    y = v_float32x4(tab[idx[0]+1], tab[idx[1]+1], tab[idx[2]+1], tab[idx[3]+1]);
}

inline v_int8x16 v_interleave_pairs(const v_int8x16& vec)
{
    return v_int8x16(vcombine_s8(vtbl1_s8(vget_low_s8(vec.val), vcreate_s8(0x0705060403010200)), vtbl1_s8(vget_high_s8(vec.val), vcreate_s8(0x0705060403010200))));
}
inline v_uint8x16 v_interleave_pairs(const v_uint8x16& vec) { return v_reinterpret_as_u8(v_interleave_pairs(v_reinterpret_as_s8(vec))); }
inline v_int8x16 v_interleave_quads(const v_int8x16& vec)
{
    return v_int8x16(vcombine_s8(vtbl1_s8(vget_low_s8(vec.val), vcreate_s8(0x0703060205010400)), vtbl1_s8(vget_high_s8(vec.val), vcreate_s8(0x0703060205010400))));
}
inline v_uint8x16 v_interleave_quads(const v_uint8x16& vec) { return v_reinterpret_as_u8(v_interleave_quads(v_reinterpret_as_s8(vec))); }

inline v_int16x8 v_interleave_pairs(const v_int16x8& vec)
{
    return v_int16x8(vreinterpretq_s16_s8(vcombine_s8(vtbl1_s8(vget_low_s8(vreinterpretq_s8_s16(vec.val)), vcreate_s8(0x0706030205040100)), vtbl1_s8(vget_high_s8(vreinterpretq_s8_s16(vec.val)), vcreate_s8(0x0706030205040100)))));
}
inline v_uint16x8 v_interleave_pairs(const v_uint16x8& vec) { return v_reinterpret_as_u16(v_interleave_pairs(v_reinterpret_as_s16(vec))); }
inline v_int16x8 v_interleave_quads(const v_int16x8& vec)
{
    int16x4x2_t res = vzip_s16(vget_low_s16(vec.val), vget_high_s16(vec.val));
    return v_int16x8(vcombine_s16(res.val[0], res.val[1]));
}
inline v_uint16x8 v_interleave_quads(const v_uint16x8& vec) { return v_reinterpret_as_u16(v_interleave_quads(v_reinterpret_as_s16(vec))); }

inline v_int32x4 v_interleave_pairs(const v_int32x4& vec)
{
    int32x2x2_t res = vzip_s32(vget_low_s32(vec.val), vget_high_s32(vec.val));
    return v_int32x4(vcombine_s32(res.val[0], res.val[1]));
}
inline v_uint32x4 v_interleave_pairs(const v_uint32x4& vec) { return v_reinterpret_as_u32(v_interleave_pairs(v_reinterpret_as_s32(vec))); }
inline v_float32x4 v_interleave_pairs(const v_float32x4& vec) { return v_reinterpret_as_f32(v_interleave_pairs(v_reinterpret_as_s32(vec))); }

inline v_int8x16 v_pack_triplets(const v_int8x16& vec)
{
    return v_int8x16(vextq_s8(vcombine_s8(vtbl1_s8(vget_low_s8(vec.val), vcreate_s8(0x0605040201000000)), vtbl1_s8(vget_high_s8(vec.val), vcreate_s8(0x0807060504020100))), vdupq_n_s8(0), 2));
}
inline v_uint8x16 v_pack_triplets(const v_uint8x16& vec) { return v_reinterpret_as_u8(v_pack_triplets(v_reinterpret_as_s8(vec))); }

inline v_int16x8 v_pack_triplets(const v_int16x8& vec)
{
    return v_int16x8(vreinterpretq_s16_s8(vextq_s8(vcombine_s8(vtbl1_s8(vget_low_s8(vreinterpretq_s8_s16(vec.val)), vcreate_s8(0x0504030201000000)), vget_high_s8(vreinterpretq_s8_s16(vec.val))), vdupq_n_s8(0), 2)));
}
inline v_uint16x8 v_pack_triplets(const v_uint16x8& vec) { return v_reinterpret_as_u16(v_pack_triplets(v_reinterpret_as_s16(vec))); }

inline v_int32x4 v_pack_triplets(const v_int32x4& vec) { return vec; }
inline v_uint32x4 v_pack_triplets(const v_uint32x4& vec) { return vec; }
inline v_float32x4 v_pack_triplets(const v_float32x4& vec) { return vec; }

#if CV_SIMD128_64F
inline v_float64x2 v_lut(const double* tab, const int* idx)
{
    double CV_DECL_ALIGNED(32) elems[2] =
    {
        tab[idx[0]],
        tab[idx[a1]]
    };
    return v_float64x2(vld1q_f64(elems));
}

inline v_float64x2 v_lut_pairs(const double* tab, const int* idx)
{
    return v_float64x2(vld1q_f64(tab + idx[0]));
}

inline v_float64x2 v_lut(const double* tab, const v_int32x4& idxvec)
{
    double CV_DECL_ALIGNED(32) elems[2] =
    {
        tab[vgetq_lane_s32(idxvec.val, 0)],
        tab[vgetq_lane_s32(idxvec.val, a1)],
    };
    return v_float64x2(vld1q_f64(elems));
}

inline void v_lut_deinterleave(const double* tab, const v_int32x4& idxvec, v_float64x2& x, v_float64x2& y)
{
    int CV_DECL_ALIGNED(32) idx[4];
    v_store_aligned(idx, idxvec);

    x = v_float64x2(tab[idx[0]], tab[idx[a1]]);
    y = v_float64x2(tab[idx[0]+a1], tab[idx[a1]+a1]);
}
#endif

////// FP16 support ///////
#if CV_FP16
inline v_float32x4 v_load_expand(const float16_t* ptr)
{
    float16x4_t v =
    #ifndef vld1_f16 // APPLE compiler defines vld1_f16 as macro
        (float16x4_t)vld1_s16((const short*)ptr);
    #else
        vld1_f16((const __fp16*)ptr);
    #endif
    return v_float32x4(vcvt_f32_f16(v));
}

inline void v_pack_store(float16_t* ptr, const v_float32x4& v)
{
    float16x4_t hv = vcvt_f16_f32(v.val);

    #ifndef vst1_f16 // APPLE compiler defines vst1_f16 as macro
        vst1_s16((short*)ptr, (int16x4_t)hv);
    #else
        vst1_f16((__fp16*)ptr, hv);
    #endif
}
#else
inline v_float32x4 v_load_expand(const float16_t* ptr)
{
    const int N = 4;
    float buf[N];
    for( int i = 0; i < N; i++ ) buf[i] = (float)ptr[i];
    return v_load(buf);
}

inline void v_pack_store(float16_t* ptr, const v_float32x4& v)
{
    const int N = 4;
    float buf[N];
    v_store(buf, v);
    for( int i = 0; i < N; i++ ) ptr[i] = float16_t(buf[i]);
}
#endif

inline void v_cleanup() {}

//! @name Check SIMD support
//! @{
//! @brief Check CPU capability of SIMD operation
static inline bool hasSIMD128()
{
    return (CV_CPU_HAS_SUPPORT_NEON) ? true : false;
}

//! @}

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

//! @endcond

}

#endif
