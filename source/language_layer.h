#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define global         static
#define internal       static
#define local_persist  static

#define MemoryCopy memcpy
#define MemorySet  memset
#define CalculateCStringLength (u32)strlen
#define FModF fmodf
#define AbsoluteValueF fabsf
#define SquareRootF sqrtf

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define Bytes(n)      (n)
#define Kilobytes(n)  (Bytes(n)*1024)
#define Megabytes(n)  (Kilobytes(n)*1024)

#define PIf 3.1415926535897f

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

typedef struct v2
{
    f32 x;
    f32 y;
}
v2;

typedef union v3
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
}
v3;

typedef union v4
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
}
v4;

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
    return SquareRootF(V3LengthSquared(a));
}

internal v3
V3Normalize(v3 v)
{
    f32 length = V3Length(v);
    v3 result = {
        v.x / length,
        v.y / length,
        v.z / length,
    };
    return result;
}

internal f32
V3Dot(v3 a, v3 b)
{
    f32 dot =
        a.x * b.x + 
        a.y * b.y +
        a.z * b.z;
    return dot;
}

internal v3
V3Cross(v3 a, v3 b)
{
    v3 result = {
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

typedef struct iv2
{
    i32 x;
    i32 y;
}
iv2;

typedef union iv3
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
}
iv3;

typedef union iv4
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
}
iv4;

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

typedef struct m4
{
    f32 elements[4][4];
}
m4;

internal m4
M4InitD(f32 diagonal)
{
    m4 m = {
        {
            { diagonal },
            { 0.f, diagonal },
            { 0.f, 0.f, diagonal },
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
    f32 tan_theta_over_2 = tanf(fov * (PIf / 360.f));
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
    v3 scale = {
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

internal int
CharToLower(int c)
{
    if(c >= 'A' && c <= 'Z')
    {
        c += 32;
    }
    return c;
}

internal int
CharToUpper(int c)
{
    if(c >= 'a' && c <= 'z')
    {
        c -= 32;
    }
    return c;
}

internal b32
CharIsDigit(int c)
{
    return c >= '0' && c <= '9';
}

internal b32
CharIsAlpha(int c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

internal b32
CStringMatchCaseInsensitiveN(const char *str1, const char *str2, u32 n)
{
    b32 result = 1;
    
    if(str1 && str2)
    {
        for(u32 i = 0; i < n; ++i)
        {
            if(CharToLower(str1[i]) != CharToLower(str2[i]))
            {
                result = 0;
                break;
            }
            if(str1[i] == 0 && str2[i] == 0)
            {
                break;
            }
        }
    }
    else if(str1 || str2)
    {
        result = 0;
    }
    
    return result;
}

internal b32
CStringMatchCaseInsensitive(const char *str1, const char *str2)
{
    b32 result = 1;
    
    if(str1 && str2)
    {
        for(u32 i = 0;; ++i)
        {
            if(CharToLower(str1[i]) != CharToLower(str2[i]))
            {
                result = 0;
                break;
            }
            if(str1[i] == 0 && str2[i] == 0)
            {
                break;
            }
        }
    }
    else if(str1 || str2)
    {
        result = 0;
    }
    
    return result;
}

internal b32
CStringMatchCaseSensitiveN(const char *str1, const char *str2, u32 n)
{
    b32 result = 1;
    
    if(str1 && str2)
    {
        for(u32 i = 0; i < n; ++i)
        {
            if(str1[i] != str2[i])
            {
                result = 0;
                break;
            }
            if(str1[i] == 0 && str2[i] == 0)
            {
                break;
            }
        }
    }
    else if(str1 || str2)
    {
        result = 0;
    }
    
    return result;
}

internal b32
CStringMatchCaseSensitive(const char *str1, const char *str2)
{
    b32 result = 1;
    
    if(str1 && str2)
    {
        for(u32 i = 0;; ++i)
        {
            if(str1[i] != str2[i])
            {
                result = 0;
                break;
            }
            if(str1[i] == 0 && str2[i] == 0)
            {
                break;
            }
        }
    }
    else if(str1 || str2)
    {
        result = 0;
    }
    
    return result;
}

internal b32
CStringContains(const char *str, const char *substr)
{
    b32 result = 0;
    
    if(str && substr)
    {
        for(u32 i = 0; str[i]; ++i)
        {
            if(str[i] == substr[0])
            {
                result = 1;
                for(u32 j = i+1; str[j] && substr[j-i]; ++j)
                {
                    if(str[j] != substr[j-i] && substr[j-i] != 0)
                    {
                        result = 0;
                        break;
                    }
                }
                
                if(result)
                {
                    break;
                }
            }
        }
    }
    
    return result;
}

internal char *
CStringPointerAfterSubstring(char *str, char *substr)
{
    char *result = 0;
    
    if(str && substr)
    {
        for(u32 i = 0; str[i]; ++i)
        {
            result = str+i;
            if(str[i] == substr[0])
            {
                for(u32 j = 0;; ++j)
                {
                    if(str[i+j] == substr[j])
                    {
                        ++result;
                        if(str[i+j] == 0 && substr[j] == 0)
                        {
                            break;
                        }
                    }
                    else if(substr[j] == 0)
                    {
                        break;
                    }
                    else if(str[i+j] == 0)
                    {
                        result = 0;
                        break;
                    }
                    else
                    {
                        result = 0;
                        break;
                    }
                }
                
                if(result)
                {
                    break;
                }
            }
        }
    }
    
    return result;
}

internal char *
CStringFindSubstring(char *str, char *substr)
{
    char *result = 0;
    
    if(str && substr)
    {
        for(u32 i = 0; str[i]; ++i)
        {
            if(str[i] == substr[0])
            {
                char *possible_result = str+i;
                b32 match = 1;
                for(u32 j = i+1; str[j] && substr[j-i]; ++j)
                {
                    if(str[j] != substr[j-i] && substr[j-i] != 0)
                    {
                        match = 0;
                        break;
                    }
                }
                
                if(match)
                {
                    result = possible_result;
                    break;
                }
            }
        }
    }
    
    return result;
}

internal i32
GetFirstI32FromCString(char *str)
{
    i32 result = 0;
    
    int last_digit = -1;
    
    for(int i = 0; str[i]; ++i)
    {
        if(CharIsDigit(str[i]))
        {
            last_digit = i;
        }
        else if(last_digit != -1)
        {
            break;
        }
    }
    
    if(last_digit >= 0)
    {
        int digit_multiplier = 1;
        
        for(int i = last_digit; i >= 0; --i)
        {
            if(CharIsDigit(str[i]))
            {
                int digit = str[i] - '0';
                result += digit_multiplier * digit;
                digit_multiplier *= 10;
            }
            else
            {
                if(str[i] == '-')
                {
                    result *= -1;
                }
                break;
            }
        }
    }
    
    return result;
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
    f32 h = FModF(hsv.x * 360.f, 360.f);
    f32 s = hsv.y;
    f32 v = hsv.z;
    
    f32 c = v * s;
    f32 x = c * (1 - AbsoluteValueF(FModF((h / 60.f), 2) - 1));
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
SeedRandomNumberGenerator(void)
{
    // TODO(rjf): Get rid of rand()
    srand((unsigned int)time(0));
}

internal f32
RandomF32(f32 low, f32 high)
{
    return low + (high - low) * ((rand() % 10000) / 10000.f);
}

global int global_perlin_noise_seed = 0;
global int global_perlin_noise_hash[] = {
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