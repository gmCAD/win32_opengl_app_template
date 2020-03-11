#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#if BUILD_WIN32
#include <windows.h>
#include <gl/gl.h>
#include "ext/wglext.h"
#else
#error "OpenGL includes for platform not supported."
#endif
#include "ext/glext.h"

#define global         static
#define internal       static
#define local_persist  static

#define MemoryCopy memcpy
#define MemoryMove memmove
#define MemorySet  memset
#define CalculateCStringLength (u32)strlen
#define FMod fmodf
#define AbsoluteValue fabsf
#define SquareRoot sqrtf
#define Sin sinf
#define Cos cosf
#define Tan tanf

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define Bytes(n)      (n)
#define Kilobytes(n)  (Bytes(n)*1024)
#define Megabytes(n)  (Kilobytes(n)*1024)
#define Gigabytes(n)  (Megabytes(n)*1024)

#define PI (3.1415926535897f)

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef i8       s8;
typedef i16      s16;
typedef i32      s32;
typedef i64      s64;
typedef i8       b8;
typedef i16      b16;
typedef i32      b32;
typedef i64      b64;
typedef float    f32;
typedef double   f64;

typedef union v2 v2;
union v2
{
    struct
    {
        f32 x;
        f32 y;
    };
    
    struct
    {
        f32 width;
        f32 height;
    };
    
    float elements[2];
};

typedef union v3 v3;
union v3
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
    };
    
    struct
    {
        f32 r;
        f32 g;
        f32 b;
    };
    
    f32 elements[3];
};

typedef union v4 v4;
union v4
{
    struct
    {
        f32 x;
        f32 y;
        union
        {
            struct
            {
                f32 z;
                
                union
                {
                    f32 w;
                    f32 radius;
                };
            };
            struct
            {
                f32 width;
                f32 height;
            };
        };
    };
    
    struct
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    
    f32 elements[4];
};

internal v2
V2Init(f32 x, f32 y)
{
    v2 v = { x, y };
    return v;
}
#define v2(x, y) V2Init(x, y)

internal v3
V3Init(f32 x, f32 y, f32 z)
{
    v3 v = { x, y, z };
    return v;
}
#define v3(x, y, z) V3Init(x, y, z)

internal v3
V3AddV3(v3 a, v3 b)
{
    v3 c = { a.x + b.x, a.y + b.y, a.z + b.z };
    return c;
}

internal v3
V3MinusV3(v3 a, v3 b)
{
    v3 c = { a.x - b.x, a.y - b.y, a.z - b.z };
    return c;
}

internal v3
V3MultiplyF32(v3 v, f32 f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;
    return v;
}

internal f32
V3LengthSquared(v3 a)
{
    return a.x*a.x + a.y*a.y + a.z*a.z;
}

internal f32
V3Length(v3 a)
{
    return SquareRoot(V3LengthSquared(a));
}

internal v3
V3Normalize(v3 v)
{
    f32 length = V3Length(v);
    v3 result =
    {
        v.x / length,
        v.y / length,
        v.z / length,
    };
    return result;
}

internal f32
V3Dot(v3 a, v3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

internal v3
V3Cross(v3 a, v3 b)
{
    v3 result =
    {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x,
    };
    return result;
}

internal float
MinimumInV3(v3 v)
{
    float minimum = v.x;
    if(v.y < minimum) { minimum = v.y; }
    if(v.z < minimum) { minimum = v.z; }
    return minimum;
}

internal float
MaximumInV3(v3 v)
{
    float maximum = v.x;
    if(v.y > maximum) { maximum = v.y; }
    if(v.z > maximum) { maximum = v.z; }
    return maximum;
}

internal v4
V4Init(f32 x, f32 y, f32 z, f32 w)
{
    v4 v = { x, y, z, w };
    return v;
}
#define v4(x, y, z, w) V4Init(x, y, z, w)

internal b32
V4RectHasPoint(v4 v, v2 p)
{
    return(p.x >= v.x && p.x <= v.x + v.width &&
           p.y >= v.y && p.y <= v.y + v.height);
}

typedef union iv2 iv2;
union iv2
{
    struct
    {
        i32 x;
        i32 y;
    };
    
    struct
    {
        i32 width;
        i32 height;
    };
    
    i32 elements[2];
};

typedef union iv3 iv3;
union iv3
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
    };
    
    struct
    {
        i32 r;
        i32 g;
        i32 b;
    };
    
    i32 elements[3];
};

typedef union iv4 iv4;
union iv4
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
        i32 w;
    };
    
    struct
    {
        i32 r;
        i32 g;
        i32 b;
        i32 a;
    };
    
    i32 elements[4];
};

internal iv2
IV2Init(i32 x, i32 y)
{
    iv2 v = { x, y };
    return v;
}
#define iv2(x, y) IV2Init(x, y)

internal iv3
IV3Init(i32 x, i32 y, i32 z)
{
    iv3 v = { x, y, z };
    return v;
}
#define iv3(x, y, z) IV3Init(x, y, z)

internal iv4
IV4Init(i32 x, i32 y, i32 z, i32 w)
{
    iv4 v = { x, y, z, w };
    return v;
}
#define iv4(x, y, z, w) IV4Init(x, y, z, w)

typedef struct m4 m4;
struct m4
{
    f32 elements[4][4];
};

internal m4
M4InitD(f32 diagonal)
{
    m4 m =
    {
        {
            { diagonal                },
            { 0.f, diagonal           },
            { 0.f, 0.f, diagonal      },
            { 0.f, 0.f, 0.f, diagonal },
        }
    };
    return m;
}

internal m4
M4MultiplyM4(m4 a, m4 b)
{
    m4 c = {0};
    
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
        {
            c.elements[i][j] = (a.elements[0][j]*b.elements[i][0] +
                                a.elements[1][j]*b.elements[i][1] +
                                a.elements[2][j]*b.elements[i][2] +
                                a.elements[3][j]*b.elements[i][3]);
        }
    }
    
    return c;
}

internal m4
M4MultiplyF32(m4 a, f32 b)
{
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
        {
            a.elements[i][j] *= b;
        }
    }
    
    return a;
}

internal f32
V4Dot(v4 a, v4 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

internal v4
V4AddV4(v4 a, v4 b)
{
    v4 c = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    return c;
}

internal v4
V4MinusV4(v4 a, v4 b)
{
    v4 c = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    return c;
}

internal v4
V4MultiplyV4(v4 a, v4 b)
{
    v4 c = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
    return c;
}

internal v4
V4MultiplyM4(v4 v, m4 m)
{
    v4 result = {0};
    
    for(int i = 0; i < 4; ++i)
    {
        result.elements[i] = (v.elements[0]*m.elements[0][i] +
                              v.elements[1]*m.elements[1][i] +
                              v.elements[2]*m.elements[2][i] +
                              v.elements[3]*m.elements[3][i]);
    }
    
    return result;
}

internal m4
M4TranslateV3(v3 translation)
{
    m4 result = M4InitD(1.f);
    result.elements[3][0] = translation.x;
    result.elements[3][1] = translation.y;
    result.elements[3][2] = translation.z;
    return result;
}

internal m4
M4ScaleV3(v3 scale)
{
    m4 result = M4InitD(1.f);
    result.elements[0][0] = scale.x;
    result.elements[1][1] = scale.y;
    result.elements[2][2] = scale.z;
    return result;
}

internal m4
M4Perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z)
{
    m4 result = {0};
    f32 tan_theta_over_2 = Tan(fov * (PI / 360.f));
    result.elements[0][0] = 1.f / tan_theta_over_2;
    result.elements[1][1] = aspect_ratio / tan_theta_over_2;
    result.elements[2][3] = -1.f;
    result.elements[2][2] = (near_z + far_z) / (near_z - far_z);
    result.elements[3][2] = (2.f * near_z * far_z) / (near_z - far_z);
    result.elements[3][3] = 0.f;
    return result;
}

internal m4
M4LookAt(v3 eye, v3 center, v3 up)
{
    m4 result;
    
    v3 f = V3Normalize(V3MinusV3(center, eye));
    v3 s = V3Normalize(V3Cross(f, up));
    v3 u = V3Cross(s, f);
    
    result.elements[0][0] = s.x;
    result.elements[0][1] = u.x;
    result.elements[0][2] = -f.x;
    result.elements[0][3] = 0.0f;
    
    result.elements[1][0] = s.y;
    result.elements[1][1] = u.y;
    result.elements[1][2] = -f.y;
    result.elements[1][3] = 0.0f;
    
    result.elements[2][0] = s.z;
    result.elements[2][1] = u.z;
    result.elements[2][2] = -f.z;
    result.elements[2][3] = 0.0f;
    
    result.elements[3][0] = -V3Dot(s, eye);
    result.elements[3][1] = -V3Dot(u, eye);
    result.elements[3][2] = V3Dot(f, eye);
    result.elements[3][3] = 1.0f;
    
    return result;
}

internal m4
M4Inverse(m4 m)
{
    f32 coef00 = m.elements[2][2] * m.elements[3][3] - m.elements[3][2] * m.elements[2][3];
    f32 coef02 = m.elements[1][2] * m.elements[3][3] - m.elements[3][2] * m.elements[1][3];
    f32 coef03 = m.elements[1][2] * m.elements[2][3] - m.elements[2][2] * m.elements[1][3];
    f32 coef04 = m.elements[2][1] * m.elements[3][3] - m.elements[3][1] * m.elements[2][3];
    f32 coef06 = m.elements[1][1] * m.elements[3][3] - m.elements[3][1] * m.elements[1][3];
    f32 coef07 = m.elements[1][1] * m.elements[2][3] - m.elements[2][1] * m.elements[1][3];
    f32 coef08 = m.elements[2][1] * m.elements[3][2] - m.elements[3][1] * m.elements[2][2];
    f32 coef10 = m.elements[1][1] * m.elements[3][2] - m.elements[3][1] * m.elements[1][2];
    f32 coef11 = m.elements[1][1] * m.elements[2][2] - m.elements[2][1] * m.elements[1][2];
    f32 coef12 = m.elements[2][0] * m.elements[3][3] - m.elements[3][0] * m.elements[2][3];
    f32 coef14 = m.elements[1][0] * m.elements[3][3] - m.elements[3][0] * m.elements[1][3];
    f32 coef15 = m.elements[1][0] * m.elements[2][3] - m.elements[2][0] * m.elements[1][3];
    f32 coef16 = m.elements[2][0] * m.elements[3][2] - m.elements[3][0] * m.elements[2][2];
    f32 coef18 = m.elements[1][0] * m.elements[3][2] - m.elements[3][0] * m.elements[1][2];
    f32 coef19 = m.elements[1][0] * m.elements[2][2] - m.elements[2][0] * m.elements[1][2];
    f32 coef20 = m.elements[2][0] * m.elements[3][1] - m.elements[3][0] * m.elements[2][1];
    f32 coef22 = m.elements[1][0] * m.elements[3][1] - m.elements[3][0] * m.elements[1][1];
    f32 coef23 = m.elements[1][0] * m.elements[2][1] - m.elements[2][0] * m.elements[1][1];
    
    v4 fac0 = { coef00, coef00, coef02, coef03 };
    v4 fac1 = { coef04, coef04, coef06, coef07 };
    v4 fac2 = { coef08, coef08, coef10, coef11 };
    v4 fac3 = { coef12, coef12, coef14, coef15 };
    v4 fac4 = { coef16, coef16, coef18, coef19 };
    v4 fac5 = { coef20, coef20, coef22, coef23 };
    
    v4 vec0 = { m.elements[1][0], m.elements[0][0], m.elements[0][0], m.elements[0][0] };
    v4 vec1 = { m.elements[1][1], m.elements[0][1], m.elements[0][1], m.elements[0][1] };
    v4 vec2 = { m.elements[1][2], m.elements[0][2], m.elements[0][2], m.elements[0][2] };
    v4 vec3 = { m.elements[1][3], m.elements[0][3], m.elements[0][3], m.elements[0][3] };
    
    v4 inv0 = V4AddV4(V4MinusV4(V4MultiplyV4(vec1, fac0), V4MultiplyV4(vec2, fac1)), V4MultiplyV4(vec3, fac2));
    v4 inv1 = V4AddV4(V4MinusV4(V4MultiplyV4(vec0, fac0), V4MultiplyV4(vec2, fac3)), V4MultiplyV4(vec3, fac4));
    v4 inv2 = V4AddV4(V4MinusV4(V4MultiplyV4(vec0, fac1), V4MultiplyV4(vec1, fac3)), V4MultiplyV4(vec3, fac5));
    v4 inv3 = V4AddV4(V4MinusV4(V4MultiplyV4(vec0, fac2), V4MultiplyV4(vec1, fac4)), V4MultiplyV4(vec2, fac5));
    
    v4 sign_a = { +1, -1, +1, -1 };
    v4 sign_b = { -1, +1, -1, +1 };
    
    m4 inverse;
    for(u32 i = 0; i < 4; ++i)
    {
        inverse.elements[0][i] = inv0.elements[i] * sign_a.elements[i];
        inverse.elements[1][i] = inv1.elements[i] * sign_b.elements[i];
        inverse.elements[2][i] = inv2.elements[i] * sign_a.elements[i];
        inverse.elements[3][i] = inv3.elements[i] * sign_b.elements[i];
    }
    
    v4 row0 = { inverse.elements[0][0], inverse.elements[1][0], inverse.elements[2][0], inverse.elements[3][0] };
    v4 m0 = { m.elements[0][0], m.elements[0][1], m.elements[0][2], m.elements[0][3] };
    v4 dot0 = V4MultiplyV4(m0, row0);
    f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
    
    f32 one_over_det = 1 / dot1;
    
    return M4MultiplyF32(inverse, one_over_det);
}

internal m4
M4RemoveRotation(m4 mat)
{
    v3 scale =
    {
        V3Length(v3(mat.elements[0][0], mat.elements[0][1], mat.elements[0][2])),
        V3Length(v3(mat.elements[1][0], mat.elements[1][1], mat.elements[1][2])),
        V3Length(v3(mat.elements[2][0], mat.elements[2][1], mat.elements[2][2])),
    };
    
    mat.elements[0][0] = scale.x;
    mat.elements[1][0] = 0.f;
    mat.elements[2][0] = 0.f;
    
    mat.elements[0][1] = 0.f;
    mat.elements[1][1] = scale.y;
    mat.elements[2][1] = 0.f;
    
    mat.elements[0][2] = 0.f;
    mat.elements[1][2] = 0.f;
    mat.elements[2][2] = scale.z;
    
    return mat;
}

internal v3
RGBToHSV(v3 rgb)
{
    f32 c_max = MaximumInV3(rgb);
    f32 c_min = MinimumInV3(rgb);
    f32 delta = c_max - c_min;
    b32 c_max_is_r = rgb.r > rgb.g && rgb.r > rgb.b;
    b32 c_max_is_g = rgb.g > rgb.r && rgb.g > rgb.b;
    b32 c_max_is_b = rgb.b > rgb.r && rgb.b > rgb.g;
    f32 h = (c_max_is_r ? (rgb.g - rgb.b) / delta + 0 : c_max_is_g ? (rgb.b - rgb.r) / delta + 2 : c_max_is_b ? (rgb.r - rgb.g) / delta + 4 : 0);
    f32 s = c_max == 0 ? 0 : (delta / c_max);
    f32 v = c_max;
    v3 hsv = {h / 6.f, s, v};
    return hsv;
}

internal v3
HSVToRGB(v3 hsv)
{
    f32 h = FMod(hsv.x * 360.f, 360.f);
    f32 s = hsv.y;
    f32 v = hsv.z;
    
    f32 c = v * s;
    f32 x = c * (1 - AbsoluteValueF(FMod((h / 60.f), 2) - 1));
    f32 m = v - c;
    
    f32 r;
    f32 g;
    f32 b;
    
    if((h >= 0.f && h < 60.f) || (h >= 360.f && h < 420.f))
    {
        r = c;
        g = x;
        b = 0;
    }
    else if(h >= 60.f && h < 120.f)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if(h >= 120.f && h < 180.f)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if(h >= 180.f && h < 240.f)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if(h >= 240.f && h < 300.f)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if((h >= 300.f && h <= 360.f) ||
            (h >= -60.f && h <= 0.f))
    {
        r = c;
        g = 0;
        b = x;
    }
    
    v3 rgb = {r + m, g + m, b + m};
    return rgb;
}

internal f32
ClampF32(f32 v, f32 l, f32 h)
{
    return v < l ? l : v > h ? h : v;
}

internal void
SeedRandomNumberGeneratorWithTime(void)
{
    // TODO(rjf): Get rid of rand()
    srand((unsigned int)time(0));
}

internal void
SeedRandomNumberGenerator(unsigned int seed)
{
    // TODO(rjf): Get rid of rand()
    srand(seed);
}

internal f32
RandomF32(f32 low, f32 high)
{
    // TODO(rjf): Get rid of rand()
    return low + (high - low) * ((rand() % 10000) / 10000.f);
}

global int global_perlin_noise_seed = 0;
global int global_perlin_noise_hash[] =
{
    208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
    185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
    9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
    70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
    203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
    164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
    228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
    232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
    193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
    101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
    135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
    114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219
};

internal int
PerlinNoise2(int x, int y)
{
    int tmp = global_perlin_noise_hash[(y + global_perlin_noise_seed) % 256];
    return global_perlin_noise_hash[(tmp + x) % 256];
}

internal f32
PerlinLinearlyInterpolate(f32 x, f32 y, f32 s)
{
    return x + s * (y-x);
}

internal f32
PerlinSmoothlyInterpolate(f32 x, f32 y, f32 s)
{
    return PerlinLinearlyInterpolate(x, y, s * s * (3-2*s));
}

internal f32
PerlinNoise2D(f32 x, f32 y)
{
    int x_int = (int)x;
    int y_int = (int)y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = PerlinNoise2(x_int, y_int);
    int t = PerlinNoise2(x_int+1, y_int);
    int u = PerlinNoise2(x_int, y_int+1);
    int v = PerlinNoise2(x_int+1, y_int+1);
    float low =  PerlinSmoothlyInterpolate((f32)s, (f32)t, x_frac);
    float high = PerlinSmoothlyInterpolate((f32)u, (f32)v, x_frac);
    return PerlinSmoothlyInterpolate(low, high, y_frac);
}

internal f32
Perlin2D(f32 x, f32 y, f32 freq, int depth)
{
    f32 xa = x*freq;
    f32 ya = y*freq;
    f32 amp = 1.0;
    f32 fin = 0;
    f32 div = 0.0;
    
    for(int i = 0; i < depth; i++)
    {
        div += 256 * amp;
        fin += PerlinNoise2D(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }
    
    return fin/div;
}

typedef struct MemoryArena MemoryArena;
struct MemoryArena
{
    void *memory;
    u32 memory_size;
    u32 memory_alloc_pos;
    u32 memory_left;
    u32 minimum_alignment;
};

internal MemoryArena
MemoryArenaInitAligned(void *memory, u32 memory_size, u32 alignment)
{
    MemoryArena arena = {0};
    arena.memory = memory;
    arena.memory_size = memory_size;
    arena.memory_left = memory_size;
    arena.minimum_alignment = alignment;
    return arena;
}

internal MemoryArena
MemoryArenaInit(void *memory, u32 memory_size)
{
    MemoryArena arena = MemoryArenaInitAligned(memory, memory_size, 8);
    return arena;
}

internal void *
MemoryArenaAllocateAligned(MemoryArena *arena, u32 size, u32 alignment)
{
    void *memory = 0;
    
    if(alignment < arena->minimum_alignment)
    {
        alignment = arena->minimum_alignment;
    }
    
    if(arena->memory_alloc_pos + size <= arena->memory_size)
    {
        memory = (void *)((char *)arena->memory + arena->memory_alloc_pos);
        u32 bytes_off_alignment = (uintptr_t)memory % (uintptr_t)alignment;
        if(bytes_off_alignment != 0)
        {
            u32 padding = alignment - bytes_off_alignment;
            memory = (char *)memory + padding;
            size += padding;
        }
        arena->memory_alloc_pos += size;
        arena->memory_left -= size;
    }
    
    return memory;
}

internal void *
MemoryArenaAllocateAndZeroAligned(MemoryArena *arena, u32 size, u32 alignment)
{
    void *memory = MemoryArenaAllocateAligned(arena, size, alignment);
    if(memory)
    {
        MemorySet(memory, 0, size);
    }
    return memory;
}

internal void *
MemoryArenaAllocate(MemoryArena *arena, u32 size)
{
    return MemoryArenaAllocateAligned(arena, size, 1);
}

internal void *
MemoryArenaAllocateAndZero(MemoryArena *arena, u32 size)
{
    return MemoryArenaAllocateAndZeroAligned(arena, size, 1);
}

internal void
MemoryArenaFreeBytes(MemoryArena *arena, u32 size)
{
    if(size > arena->memory_alloc_pos)
    {
        arena->memory_alloc_pos = 0;
        arena->memory_left = arena->memory_size;
    }
    else
    {
        arena->memory_alloc_pos -= size;
        arena->memory_left += size;
    }
}

internal void
MemoryArenaClear(MemoryArena *arena)
{
    arena->memory_alloc_pos = 0;
    arena->memory_left = arena->memory_size;
}

internal void
MemoryArenaZero(MemoryArena *arena)
{
    MemorySet(arena->memory, 0, arena->memory_size);
}

internal char *
MemoryArenaAllocateCStringCopy(MemoryArena *arena, char *str)
{
    u32 str_length = CalculateCStringLength(str);
    char *str_copy = (char *)MemoryArenaAllocate(arena, str_length+1);
    if(str_copy)
    {
        MemoryCopy(str_copy, str, str_length);
        str_copy[str_length] = 0;
    }
    return str_copy;
}

internal b32
MemoryArenaWrite(MemoryArena *arena, void *source, u32 size)
{
    b32 success = 0;
    if(arena->memory_left >= size)
    {
        void *destination = MemoryArenaAllocate(arena, size);
        MemoryCopy(destination, source, size);
        success = 1;
    }
    return success;
}

internal b32
MemoryArenaRead(MemoryArena *arena, void *destination, u32 size)
{
    b32 success = 0;
    if(arena->memory_left >= size)
    {
        void *source = MemoryArenaAllocate(arena, size);
        MemoryCopy(destination, source, size);
        success = 1;
    }
    return success;
}

typedef struct MemoryPoolChunk MemoryPoolChunk;
struct MemoryPoolChunk
{
    MemoryPoolChunk *next;
};

typedef struct MemoryPool MemoryPool;
struct MemoryPool
{
    void *memory;
    u32 memory_size;
    u32 chunk_size;
    u32 free_list_length;
    MemoryPoolChunk *free_list_head;
};

internal MemoryPool
MemoryPoolInit(void *memory, u32 memory_size, u32 chunk_size)
{
    MemoryPool pool = {0};
    pool.memory = memory;
    pool.memory_size = memory_size;
    if(chunk_size < sizeof(MemoryPoolChunk))
    {
        chunk_size = sizeof(MemoryPoolChunk);
    }
    pool.chunk_size = chunk_size;
    u32 number_of_chunks = memory_size / chunk_size;
    for(u32 i = 0; i < number_of_chunks; ++i)
    {
        MemoryPoolChunk *chunk = (void *)((char *)memory + chunk_size*i);
        
        if(i < number_of_chunks - 1)
        {
            chunk->next = (void *)((char *)memory + chunk_size*(i+1));
        }
        else
        {
            chunk->next = 0;
        }
    }
    pool.free_list_head = memory;
    return pool;
}

internal void *
MemoryPoolAllocate(MemoryPool *pool)
{
    void *memory = 0;
    if(pool->free_list_head)
    {
        memory = pool->free_list_head;
        pool->free_list_head = pool->free_list_head->next;
    }
    return memory;
}

internal void
MemoryPoolFree(MemoryPool *pool, void *memory)
{
    if((u64)memory >= (u64)pool->memory && (u64)memory < (u64)pool->memory + pool->memory_size)
    {
        MemoryPoolChunk *old_free_list_head = pool->free_list_head;
        pool->free_list_head = memory;
        pool->free_list_head->next = old_free_list_head;
    }
}

internal void
MemoryPoolClear(MemoryPool *pool)
{
    *pool = MemoryPoolInit(pool->memory, pool->memory_size, pool->chunk_size);
}

#define StringCopy                 strcpy
#define StringCopyN                strncpy
#define CStringToI32(s)            ((i32)atoi(s))
#define CStringToI16(s)            ((i16)atoi(s))
#define CStringToF32(s)            ((f32)atof(s))
#define CharIsSpace(c) ((c) <= 32)
#define CharIsDigit CharIsNumeric

internal b32
CharIsAlpha(char c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}

internal b32
CharIsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

internal b32
CharIsSymbol(char c)
{
    return (c == '~' ||
            c == '!' ||
            c == '%' ||
            c == '^' ||
            c == '&' ||
            c == '*' ||
            c == '(' ||
            c == ')' ||
            c == '[' ||
            c == ']' ||
            c == '{' ||
            c == '}' ||
            c == '-' ||
            c == '+' ||
            c == '=' ||
            c == ';' ||
            c == ':' ||
            c == '<' ||
            c == '>' ||
            c == '/' ||
            c == '?' ||
            c == '.' ||
            c == ',');
}

internal char
CharToLower(char c)
{
    if(c >= 'A' && c <= 'Z')
    {
        return c + 32;
    }
    return c;
}

internal char
CharToUpper(char c)
{
    if(c >= 'a' && c <= 'z')
    {
        return c - 32;
    }
    return c;
}

internal b32
CStringMatchCaseSensitive(char *a, char *b)
{
    b32 result = 1;
    if(a && b)
    {
        for(u32 i = 0;; ++i)
        {
            if(a[i] != b[i])
            {
                result = 0;
                break;
            }
            else if(!a[i])
            {
                break;
            }
        }
    }
    else if(a != b)
    {
        result = 0;
    }
    return result;
}

internal b32
CStringMatchCaseSensitiveN(char *a, char *b, u32 n)
{
    b32 result = 1;
    if(a && b)
    {
        for(u32 i = 0; i < n; ++i)
        {
            if(a[i] != b[i])
            {
                result = 0;
                break;
            }
            else if(!a[i])
            {
                break;
            }
        }
    }
    else if(a != b)
    {
        result = 0;
    }
    return result;
}

internal b32
CStringMatchCaseInsensitive(char *a, char *b)
{
    b32 result = 1;
    if(a && b)
    {
        for(u32 i = 0;; ++i)
        {
            if(CharToLower(a[i]) != CharToLower(b[i]))
            {
                result = 0;
                break;
            }
            else if(!a[i])
            {
                break;
            }
        }
    }
    else if(a != b)
    {
        result = 0;
    }
    return result;
}

internal b32
CStringMatchCaseInsensitiveN(char *a, char *b, u32 n)
{
    b32 result = 1;
    if(a && b)
    {
        for(u32 i = 0; i < n; ++i)
        {
            if(CharToLower(a[i]) != CharToLower(b[i]))
            {
                result = 0;
                break;
            }
            else if(!a[i])
            {
                break;
            }
        }
    }
    else if(a != b)
    {
        result = 0;
    }
    return result;
}

#define CStringPrefixedWith CStringBeginsWith
internal b32
CStringBeginsWith(char *str, char *prefix)
{
    return CStringMatchCaseInsensitiveN(str, prefix, CalculateCStringLength(prefix));
}

internal b32
CStringEndsWithSuffix(char *str, char *suffix)
{
    return CStringMatchCaseInsensitive(str + CalculateCStringLength(str) - CalculateCStringLength(suffix), suffix);
}

internal b32
CStringHasSubstring(char *str, char *substr)
{
    b32 result = 0;
    for(u32 i = 0; str[i]; ++i)
    {
        if(CharToLower(str[i]) == CharToLower(substr[0]))
        {
            if(CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
            {
                result = 1;
                break;
            }
        }
    }
    return result;
}

internal i32
GetFirstI32FromCString(char *str)
{
    i32 result = 0;
    b32 found_first_digit = 0;
    u32 integer_write_pos = 0;
    char integer[64] = {0};
    u32 read_pos = 0;
    for(;; ++read_pos)
    {
        if(str[read_pos] == 0)
        {
            break;
        }
        if(found_first_digit)
        {
            if(integer_write_pos == sizeof(integer))
            {
                integer[sizeof(integer) - 1] = 0;
                break;
            }
            if(CharIsDigit(str[read_pos]) || str[read_pos] == '-')
            {
                integer[integer_write_pos++] = str[read_pos];
            }
            else
            {
                integer[integer_write_pos++] = 0;
                break;
            }
        }
        else
        {
            if(CharIsDigit(str[read_pos]) || str[read_pos] == '-')
            {
                integer[integer_write_pos++] = str[read_pos];
                found_first_digit = 1;
            }
        }
    }
    result = CStringToI32(integer);
    return result;
}

internal f32
GetFirstF32FromCString(char *str)
{
    f32 result = 0;
    b32 found_first_digit = 0;
    u32 float_write_pos = 0;
    char float_str[64] = {0};
    u32 read_pos = 0;
    for(;; ++read_pos)
    {
        if(str[read_pos] == 0)
        {
            break;
        }
        if(found_first_digit)
        {
            if(float_write_pos == sizeof(float_str))
            {
                float_str[sizeof(float_str) - 1] = 0;
                break;
            }
            if(CharIsDigit(str[read_pos]) || str[read_pos] == '.' || str[read_pos] == '-')
            {
                float_str[float_write_pos++] = str[read_pos];
            }
            else
            {
                float_str[float_write_pos++] = 0;
                break;
            }
        }
        else
        {
            if(CharIsDigit(str[read_pos]) || str[read_pos] == '.' || str[read_pos] == '-')
            {
                float_str[float_write_pos++] = str[read_pos];
                found_first_digit = 1;
            }
        }
    }
    result = CStringToF32(float_str);
    return result;
}

internal void
CopySubstringToStringUntilCharN(char *str1, u32 str1_max,
                                const char *str2, char str2_term)
{
    u32 write_pos = 0;
    while(1)
    {
        if(str2[write_pos] == str2_term || write_pos == str1_max - 1)
        {
            str1[write_pos++] = 0;
            break;
        }
        else
        {
            str1[write_pos] = str2[write_pos];
            ++write_pos;
        }
    }
}

internal void
CopyStringToFixedSizeBuffer(char *dest, u32 dest_max,
                            const char *src)
{
    u32 read_pos = 0;
    u32 write_pos = 0;
    for(;;)
    {
        if(src[read_pos] == 0 || write_pos >= dest_max)
        {
            break;
        }
        dest[write_pos++] = src[read_pos++];
    }
    if(write_pos >= dest_max)
    {
        dest[dest_max - 1] = 0;
    }
    else
    {
        dest[write_pos++] = 0;
    }
}

internal u32
CStringIndexAfterSubstring(char *str, char *substr)
{
    u32 result = 0;
    for(u32 i = 0; str[i]; ++i)
    {
        if(str[i] == substr[0])
        {
            if(CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
            {
                result = i + CalculateCStringLength(substr);
            }
        }
    }
    return result;
}

internal u32
CStringFirstIndexAfterSubstring(char *str, char *substr)
{
    u32 result = 0;
    for(u32 i = 0; str[i]; ++i)
    {
        if(str[i] == substr[0])
        {
            if(CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
            {
                result = i + CalculateCStringLength(substr);
                break;
            }
        }
    }
    return result;
}

internal void
CopyCStringToFixedSizeBuffer(char *destination, u32 destination_max, char *source)
{
    for(u32 i = 0; i < destination_max; ++i)
    {
        destination[i] = source[i];
        if(source[i] == 0)
        {
            break;
        }
    }
    destination[destination_max-1] = 0;
}

internal void
CopyCStringToFixedSizeBufferN(char *destination, u32 destination_max, char *source, u32 source_max)
{
    for(u32 i = 0; i < destination_max && i < source_max; ++i)
    {
        destination[i] = source[i];
        if(source[i] == 0)
        {
            break;
        }
    }
    destination[destination_max-1] = 0;
}

internal char *
ConvertCStringToLowercase(char *str)
{
    for(int i = 0; str[i]; ++i)
    {
        str[i] = CharToLower(str[i]);
    }
    return str;
}

internal char *
ConvertCStringToUppercase(char *str)
{
    for(int i = 0; str[i]; ++i)
    {
        str[i] = CharToUpper(str[i]);
    }
    return str;
}

internal char *
ConvertCStringToLowercaseWithUnderscores(char *str)
{
    for(int i = 0; str[i]; ++i)
    {
        if(str[i] == ' ')
        {
            str[i] = '_';
        }
        else
        {
            str[i] = CharToLower(str[i]);
        }
    }
    return str;
}

typedef struct String
{
    char *data;
    u32 length;
    u32 size;
    u32 max_size;
    b32 is_mutable;
}
String;

internal char *
MakeCStringOnMemoryArena(MemoryArena *arena, char *format, ...)
{
    char *result = 0;
    va_list args;
    va_start(args, format);
    u32 needed_bytes = vsnprintf(0, 0, format, args)+1;
    va_end(args);
    result = MemoryArenaAllocate(arena, needed_bytes);
    if(result)
    {
        va_start(args, format);
        vsnprintf(result, needed_bytes, format, args);
        va_end(args);
        result[needed_bytes-1] = 0;
    }
    return result;
}

internal String
MakeStringOnMemoryArena(MemoryArena *arena, char *format, ...)
{
    String string = {0};
    
    va_list args;
    va_start(args, format);
    u32 needed_bytes = vsnprintf(0, 0, format, args)+1;
    va_end(args);
    
    string.data = MemoryArenaAllocate(arena, needed_bytes);
    if(string.data)
    {
        string.length = needed_bytes-1;
        string.size = needed_bytes;
        string.max_size = string.size;
        string.is_mutable = 0;
        
        va_start(args, format);
        vsnprintf(string.data, needed_bytes, format, args);
        va_end(args);
        
        string.data[needed_bytes-1] = 0;
    }
    
    return string;
}

static unsigned int global_crc32_table[] =
{
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
    0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
    0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
    0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
    0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
    0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
    0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
    0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
    0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
    0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
    0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
    0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
    0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
    0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
    0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
    0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
    0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
    0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
    0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
    0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
    0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
    0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4,
};

internal u32
CStringCRC32N(char *name, u32 n)
{
    u32 crc = 0;
    for(u32 i = 0; name[i] && i < n; ++i)
    {
        crc = (crc << 8) ^ global_crc32_table[((crc >> 24) ^ name[i]) & 255];
    }
    return crc;
}

internal u32
CStringCRC32(char *name)
{
    return CStringCRC32N(name, (u32)(u32)(-1));
}

internal void
AppendToFixedSizeCString(char *destination, u32 destination_max, char *str)
{
    u32 i = 0;
    for(; i < destination_max && destination[i]; ++i);
    if(destination_max > i)
    {
        CopyCStringToFixedSizeBuffer(destination + i, destination_max - i, str);
    }
}

// NOTE(rjf): Keys

enum
{
#define Key(name, str) KEY_##name,
#include "platform_key_list.inc"
#undef Key
    KEY_MAX
};

internal char *
KeyName(i32 key)
{
    local_persist char *key_names[KEY_MAX] = {
#define Key(name, str) str,
#include "platform_key_list.inc"
#undef Key
    };
    char *key_name = "(Invalid Key)";
    if(key >= 0 && key < KEY_MAX)
    {
        key_name = key_names[key];
    }
    return key_name;
}

// NOTE(rjf): Gamepad Buttons

enum
{
#define GamepadButton(name, str) GAMEPAD_BUTTON_##name,
#include "gamepad_button_list.inc"
#undef GamepadButton
    GAMEPAD_BUTTON_MAX
};

internal char *
GamepadButtonName(i32 GamepadButton)
{
    local_persist char *GamepadButton_names[GAMEPAD_BUTTON_MAX] = {
#define GamepadButton(name, str) str,
#include "gamepad_button_list.inc"
#undef GamepadButton
    };
    return GamepadButton_names[GamepadButton];
}

#define PLATFORM_DIRECTORY_LIST_DIRECTORIES (1<<0)
#define PLATFORM_DIRECTORY_LIST_EXTENSIONS  (1<<1)
#define PLATFORM_DIRECTORY_LIST_RECURSIVE   (1<<2)

#define PLATFORM_DIRECTORY_ITEM_DIRECTORY   (1<<0)

typedef struct PlatformDirectoryList PlatformDirectoryList;
struct PlatformDirectoryList
{
    i32 flags;
    u32 item_count;
    char **items;
    i32 *item_flags;
};

typedef struct GamepadInput GamepadInput;
struct GamepadInput
{
    b32 connected;
    v2 joystick_1;
    v2 joystick_2;
    f32 trigger_left;
    f32 trigger_right;
    i32 button_states[GAMEPAD_BUTTON_MAX];
};

typedef struct Platform Platform;
struct Platform
{
    // NOTE(rjf): Application Metadata
    char *executable_folder_absolute_path;
    char *executable_absolute_path;
    char *working_directory_path;
    
    // NOTE(rjf): Application Memory
    MemoryArena permanent_arena;
    MemoryArena scratch_arena;
    
    // NOTE(rjf): Options
    volatile b32 quit;
    b32 vsync;
    b32 fullscreen;
    int window_width;
    int window_height;
    f32 current_time;
    f32 target_frames_per_second;
    b32 wait_for_events_to_update;
    b32 pump_events;
    
    // NOTE(rjf): Mouse Input Data
    f32   mouse_x;
    f32   mouse_y;
    f32   last_mouse_x;
    f32   last_mouse_y;
    f32   mouse_scroll_x;
    f32   mouse_scroll_y;
    b32   left_mouse_down;
    b32   left_mouse_pressed;
    b32   right_mouse_down;
    b32   right_mouse_pressed;
    b32   mouse_position_captured;
    b32   mouse_buttons_captured;
    b32   pre_frame_left_mouse_down;
    b32   pre_frame_right_mouse_down;
    
    // NOTE(rjf): Keyboard Input Data
    b8    key_down            [KEY_MAX];
    b8    key_pressed         [KEY_MAX];
    b8    key_released        [KEY_MAX];
    b8    pre_frame_key_down  [KEY_MAX];
    i32   last_key;
    char *target_text;
    u32   target_text_max_characters;
    u32   target_text_edit_pos;
    b32   keyboard_captured;
    
    // NOTE(rjf): Gamepad Input Data
#ifndef MAX_GAMEPAD_COUNT
#define MAX_GAMEPAD_COUNT 1
#endif
    GamepadInput *gamepads;
    GamepadInput *last_frame_gamepads;
    GamepadInput gamepad_states_1[MAX_GAMEPAD_COUNT];
    GamepadInput gamepad_states_2[MAX_GAMEPAD_COUNT];
    
    // NOTE(rjf): Audio Output Data
    f32 *sample_out;
    u32 sample_count_to_output;
    u32 samples_per_second;
    
    // NOTE(rjf): Functions
    void (*OutputError)(char *error_type, char *error_format, ...);
    void (*SaveToFile)(char *path, void *data, u32 data_len);
    void (*AppendToFile)(char *path, void *data, u32 data_len);
    void (*LoadEntireFile)(char *path, void **data, u32 *data_len, b32 error_on_non_existence);
    char *(*LoadEntireFileAndNullTerminate)(char *path);
    void (*FreeFileMemory)(void *data);
    void (*DeleteFile)(char *path);
    b32 (*MakeDirectory)(char *path);
    b32 (*DoesFileExist)(char *path);
    b32 (*DoesDirectoryExist)(char *path);
    b32 (*CopyFile)(char *dest, char *source);
    b32 (*CopyDirectoryRecursively)(char *dest, char *source);
    PlatformDirectoryList (*PlatformDirectoryListLoad)(char *path, i32 flags);
    void (*PlatformDirectoryListCleanUp)(PlatformDirectoryList *file_list);
    void *(*HeapAlloc)(u32 size);
    void (*HeapFree)(void *memory);
    f32 (*GetTime)(void);
    u64 (*GetCycles)(void);
    void (*ResetCursor)(void);
    void (*SetCursorToHorizontalResize)(void);
    void (*SetCursorToVerticalResize)(void);
    void (*RefreshScreen)(void);
    void *(*LoadOpenGLProcedure)(char *name);
};

global Platform *platform = 0;

inline internal void
PlatformCaptureMousePosition(void)
{
    platform->mouse_x = -1000;
    platform->mouse_y = -1000;
    platform->mouse_scroll_x = 0;
    platform->mouse_scroll_y = 0;
    platform->mouse_position_captured = 1;
}

inline internal void
PlatformCaptureMouseButtons(void)
{
    platform->left_mouse_pressed = 0;
    platform->left_mouse_down = 0;
    platform->right_mouse_pressed = 0;
    platform->right_mouse_down = 0;
    platform->mouse_buttons_captured = 1;
}

inline internal void
PlatformCaptureKeyboard(void)
{
    MemorySet(platform->key_down, 0, sizeof(platform->key_down));
    MemorySet(platform->key_pressed, 0, sizeof(platform->key_pressed));
    MemorySet(platform->key_released, 0, sizeof(platform->key_released));
    platform->last_key = 0;
    platform->keyboard_captured = 1;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
PlatformBeginFrame(void)
{
    platform->pre_frame_left_mouse_down = platform->left_mouse_down;
    platform->pre_frame_right_mouse_down = platform->right_mouse_down;
    MemoryCopy(platform->pre_frame_key_down, platform->key_down, sizeof(platform->key_down[0]) * KEY_MAX);
    platform->target_text = 0;
    platform->pump_events = 0;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
PlatformEndFrame(void)
{
    platform->last_mouse_x = platform->mouse_x;
    platform->last_mouse_y = platform->mouse_y;
    
    // NOTE(rjf): Prepare input data for next frame
    {
        platform->last_key = 0;
        for(u32 i = 0; i < KEY_MAX; ++i)
        {
            platform->key_pressed[i] = 0;
            platform->key_released[i] = 0;
        }
        platform->keyboard_captured = 0;
        
        platform->mouse_position_captured = 0;
        platform->mouse_buttons_captured = 0;
        platform->mouse_scroll_x = 0.f;
        platform->mouse_scroll_y = 0.f;
        platform->left_mouse_down = platform->pre_frame_left_mouse_down;
        platform->right_mouse_down = platform->pre_frame_right_mouse_down;
        MemoryCopy(platform->key_down, platform->pre_frame_key_down, sizeof(platform->key_down[0]) * KEY_MAX);
        platform->left_mouse_pressed = 0;
        platform->right_mouse_pressed = 0;
        
        platform->last_frame_gamepads = platform->gamepads;
        if(platform->gamepads == platform->gamepad_states_1)
        {
            platform->gamepads = platform->gamepad_states_2;
        }
        else
        {
            platform->gamepads = platform->gamepad_states_1;
        }
    }
    
    platform->current_time += 1.f / platform->target_frames_per_second;
}

#ifdef _MSC_VER
#define APP_PROC __declspec(dllexport)
#else
#define APP_PROC
#endif

/* Loaded as "PermanentLoad" */
#define APPLICATION_PERMANENT_LOAD(name) void name(Platform *platform_)
typedef APPLICATION_PERMANENT_LOAD(ApplicationPermanentLoadCallback);
APPLICATION_PERMANENT_LOAD(ApplicationPermanentLoadStub) {}

/* Loaded as "HotLoad" */
#define APPLICATION_HOT_LOAD(name) void name(Platform *platform_)
typedef APPLICATION_HOT_LOAD(ApplicationHotLoadCallback);
APPLICATION_HOT_LOAD(ApplicationHotLoadStub) {}

/* Loaded as "HotUnload" */
#define APPLICATION_HOT_UNLOAD(name) void name(void)
typedef APPLICATION_HOT_UNLOAD(ApplicationHotUnloadCallback);
APPLICATION_HOT_UNLOAD(ApplicationHotUnloadStub) {}

/* Loaded as "Update" */
#define APPLICATION_UPDATE(name) void name(void)
typedef APPLICATION_UPDATE(ApplicationUpdateCallback);
APPLICATION_UPDATE(ApplicationUpdateStub) {}

#undef Assert
#define AssertStatement HardAssert
#define Assert HardAssert
#define HardAssert(b) do { if(!(b)) { _AssertFailure(#b, __LINE__, __FILE__, 1); } } while(0)
#define SoftAssert(b) do { if(!(b)) { _AssertFailure(#b, __LINE__, __FILE__, 0); } } while(0)
#define BreakDebugger() _DebugBreak_Internal_()
#define Log(...)         _DebugLog(0,           __FILE__, __LINE__, __VA_ARGS__)
#define LogWarning(...)  _DebugLog(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LogError(...)    _DebugLog(LOG_ERROR,   __FILE__, __LINE__, __VA_ARGS__)

#define LOG_WARNING (1<<0)
#define LOG_ERROR   (1<<1)

void _AssertFailure(char *expression, int line, char *file, int crash);
void _DebugLog(i32 flags, char *file, int line, char *format, ...);
void _DebugBreak_Internal_(void);
void _BeginTimer(char *file, int line, char *format, ...);
void _EndTimer(void);

void
_AssertFailure(char *expression, int line, char *file, int crash)
{
    if(crash)
    {
        platform->OutputError("Assertion Failure", "Assertion of %s at %s:%i failed. Trying to crash...",
                              expression, file, line);
        BreakDebugger();
        *(volatile int *)0 = 0;
    }
    else
    {
        LogError("[Soft Assertion] Assertion of %s at %s:%i failed.", expression, file, line);
    }
}

void
_DebugLog(i32 flags, char *file, int line, char *format, ...)
{
    // NOTE(rjf): Log to stdout
    {
        char *name = "Info";
        if(flags & LOG_ERROR)
        {
            name = "Error";
        }
        else if(flags & LOG_WARNING)
        {
            name = "Warning";
        }
        fprintf(stdout, "%s (%s:%i) ", name, file, line);
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fprintf(stdout, "%s", "\n");
    }
    
    // NOTE(rjf): Log to VS output, etc.
    {
        local_persist char string[4096] = {0};
        va_list args;
        va_start(args, format);
        vsnprintf(string, sizeof(string), format, args);
        va_end(args);
#if BUILD_WIN32
        OutputDebugStringA(string);
        OutputDebugStringA("\n");
#endif
    }
    
}

void
_DebugBreak_Internal_(void)
{
#if _MSC_VER
    __debugbreak();
#else
    *(volatile int *)0 = 0;
#endif
}
