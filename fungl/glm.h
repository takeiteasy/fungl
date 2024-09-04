/* glm.h -- Basic Linear Algebra library
 https://github.com/takeiteasy/fungl

 Functionality relies on Clang + GCC extensions.
 When building `-fenable-matrix` for Matrix support.
 To disable Matrix support define `GLM_NO_MATRICES`

 Acknowledgements:
 - A lot of this was hand translated from raylib's raymath.h header
 https://github.com/raysan5/raylib/blob/master/src/raymath.h (Zlib)

 The MIT License (MIT)

 Copyright (c) 2022 George Watson

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#if !defined(__clang__) && !(defined(__GNUC__) || defined(__GNUG__))
#define GLM_NO_MATRICES
#endif

#ifdef __has_extension
#if !__has_extension(c_generic_selections)
#define GLM_NO_GENERICS
#endif
#else
#if !defined(__STDC__) || !__STDC__ || __STDC_VERSION__ < 201112L
#define GLM_NO_GENERICS
#endif
#endif

#ifndef GLM_HEADER
#define GLM_HEADER
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef GLM_NO_PRINT
#include <stdio.h>
#endif
#if !defined(__STDC__) || !__STDC__ || __STDC_VERSION__ < 199901L || defined(_MSC_VER)
#if defined(_MSC_VER)
#include <windef.h>
#define bool BOOL
#define true 1
#define false 0
#else
typedef enum bool { false = 0, true = !false } bool;
#endif
#else
#include <stdbool.h>
#endif
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288
#define TWO_PI 6.28318530717958647692 // 2 * pi
#define TAU TWO_PI
#define HALF_PI 1.57079632679489661923132169163975144     // pi / 2
#define QUARTER_PI 0.785398163397448309615660845819875721 // pi / 4
#define PHI 1.61803398874989484820
#define INV_PHI 0.61803398874989484820
#define EULER 2.71828182845904523536
#define EPSILON 0.000001f
#define SQRT2 1.41421356237309504880168872420969808

#define BYTES(n) (n)
#define KILOBYTES(n) (n << 10)
#define MEGABYTES(n) (n << 20)
#define GIGABYTES(n) (((uint64_t)n) << 30)
#define TERABYTES(n) (((uint64_t)n) << 40)

#define THOUSAND(n) ((n) * 1000)
#define MILLION(n) ((n) * 1000000)
#define BILLION(n) ((n) * 1000000000LL)

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define CLAMP(VALUE, _MIN, _MAX) (MIN(MAX((VALUE), (_MIN)), (_MAX)))
#define TO_DEGREES(RADIANS) ((RADIANS) * (180.f / PI))
#define TO_RADIANS(DEGREES) ((DEGREES) * (PI / 180.f))

#define __GLM_TYPES \
    X(2)            \
    X(3)            \
    X(4)

#define X(SZ)                                                   \
    typedef float vec##SZ __attribute__((ext_vector_type(SZ))); \
    vec##SZ vec##SZ##_zero(void);                               \
    bool vec##SZ##_is_zero(vec##SZ);                            \
    float vec##SZ##_sum(vec##SZ);                               \
    int vec##SZ##_cmp(vec##SZ, vec##SZ);                        \
    float vec##SZ##_length_sqr(vec##SZ);                        \
    float vec##SZ##_length(vec##SZ);                            \
    float vec##SZ##_dot(vec##SZ, vec##SZ);                      \
    vec##SZ vec##SZ##_normalize(vec##SZ);                       \
    float vec##SZ##_distance_sqr(vec##SZ, vec##SZ);             \
    float vec##SZ##_distance(vec##SZ, vec##SZ);                 \
    vec##SZ vec##SZ##_clamp(vec##SZ, vec##SZ, vec##SZ);         \
    vec##SZ vec##SZ##_lerp(vec##SZ, vec##SZ, float);
__GLM_TYPES
#undef X
typedef vec4 quat;
typedef vec4 color;

#ifndef GLM_NO_GENERICS
#endif

#define X(N) \
    typedef int vec##N##i __attribute__((ext_vector_type(N)));
__GLM_TYPES
#undef X

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_END(...)
#define MAP_OUT
#define MAP_COMMA ,

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0(test, next, 0)
#define MAP_NEXT(test, next) MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP1)(f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP0)(f, peek, __VA_ARGS__)

#define MAP_LIST_NEXT1(test, next) MAP_NEXT0(test, MAP_COMMA next, 0)
#define MAP_LIST_NEXT(test, next) MAP_LIST_NEXT1(MAP_GET_END test, next)

#define MAP_LIST0(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST1)(f, peek, __VA_ARGS__)
#define MAP_LIST1(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST0)(f, peek, __VA_ARGS__)

#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define MAP_LIST(f, ...) EVAL(MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define MAP_LIST0_UD(f, userdata, x, peek, ...) f(x, userdata) MAP_LIST_NEXT(peek, MAP_LIST1_UD)(f, userdata, peek, __VA_ARGS__)
#define MAP_LIST1_UD(f, userdata, x, peek, ...) f(x, userdata) MAP_LIST_NEXT(peek, MAP_LIST0_UD)(f, userdata, peek, __VA_ARGS__)

#define MAP_LIST_UD(f, userdata, ...) EVAL(MAP_LIST1_UD(f, userdata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define SWZL_MAP(V, VEC) VEC.V
#define explode(V, ...) MAP_LIST_UD(SWZL_MAP, V, __VA_ARGS__)
#define swizzle(V, ...) {explode(V, __VA_ARGS__}

// Taken from: https://gist.github.com/61131/7a22ac46062ee292c2c8bd6d883d28de
#define N_ARGS(...) _NARG_(__VA_ARGS__, _RSEQ())
#define _NARG_(...) _SEQ(__VA_ARGS__)
#define _SEQ(_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,_101,_102,_103,_104,_105,_106,_107,_108,_109,_110,_111,_112,_113,_114,_115,_116,_117,_118,_119,_120,_121,_122,_123,_124,_125,_126,_127,N,...) N
#define _RSEQ() 127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#define GETMACRO___(NAME, N) NAME##N
#define GETMACRO__(NAME, N) GETMACRO___(NAME, N)
#define GETMACRO_(FUNC, ...) GETMACRO__(FUNC, N_ARGS(__VA_ARGS__)) (__VA_ARGS__)
#define Vec(...) GETMACRO_(Vec, __VA_ARGS__)
#define Vec2(x, y) (vec2){x, y}
#define Vec3(x, y, z) (vec3){x, y, z}
#define Vec4(x, y, z, w) (vec4){x, y, z, w}

#ifndef GLM_NO_MATRICES
#define __DEF_GLM_MATRIX(COLUMNS, ROWS) \
    typedef float mat##COLUMNS##ROWS __attribute__((matrix_type((COLUMNS), (ROWS))));

#define X(N)                                      \
    __DEF_GLM_MATRIX(N, N)                        \
    typedef mat##N##N mat##N;                     \
    mat##N Mat##N(void);                          \
    mat##N mat##N##_identity(void);               \
    bool mat##N##_is_identity(mat##N mat);        \
    bool mat##N##_is_zero(mat##N mat);            \
    float mat##N##_trace(mat##N mat);             \
    mat##N mat##N##_transpose(mat##N);            \
    vec##N mat##N##_column(mat##N, unsigned int); \
    vec##N mat##N##_row(mat##N, unsigned int);
__GLM_TYPES
#undef X
#endif

#ifndef GLM_NO_PRINT
#ifndef GLM_NO_MATRICES
#define X(SZ) void mat##SZ##_print(mat##SZ);
__GLM_TYPES
#undef X
#endif // GLM_NO_MATRICES
#define X(SZ) void vec##SZ##_print(vec##SZ);
__GLM_TYPES
#undef X

#ifndef GLM_NO_GENERICS
#ifndef GLM_NO_MATRICES
#define glm_print(data) _Generic((data), \
    vec2: vec2_print,                    \
    vec3: vec3_print,                    \
    vec4: vec4_print,                    \
    mat2: mat2_print,                    \
    mat3: mat3_print,                    \
    mat4: mat4_print,                    \
    default: printf("Unsupported type\n"))(data)
#else
#define glm_print(data) _Generic((data), \
    vec2: vec2_print,                    \
    vec3: vec3_print,                    \
    vec4: vec4_print,                    \
    default: printf("Unsupported type\n"))(data)
#endif // GLM_NO_MATRICES
#endif // GLM_NO_GENERICS
#endif // GLM_NO_PRINT

float vec2_angle(vec2 v1, vec2 v2);
vec2 vec2_rotate(vec2 v, float angle);
vec2 vec2_move_towards(vec2 v, vec2 target, float maxDistance);
vec2 vec2_reflect(vec2 v, vec2 normal);
float vec2_cross(vec2 a, vec2 b);

vec3 vec3_reflect(vec3 v, vec3 normal);
vec3 vec3_cross(vec3 v1, vec3 v2);
vec3 vec3_perpendicular(vec3 v);
float vec3_angle(vec3 v1, vec3 v2);
vec3 quat_rotate_vec3(vec3 v, quat q);
vec3 vec3_rotate_axis_angle(vec3 v, vec3 axis, float angle);
vec3 vec3_refract(vec3 v, vec3 n, float r);
vec3 vec3_barycenter(vec3 p, vec3 a, vec3 b, vec3 c);

quat quat_identity(void);
quat quat_mul(quat q1, quat q2);
quat quat_invert(quat q);
quat quat_to_from_vec3(vec3 from, vec3 to);
quat quat_from_axis_angle(vec3 axis, float angle);
void quat_to_axis_angle(quat q, vec3 *outAxis, float *outAngle);
quat quat_from_euler(float pitch, float yaw, float roll);
vec3 quat_to_euler(quat q);
int quat_cmp(quat p, quat q);

#ifndef GLM_NO_MATRICES
vec3 vec3_transform(vec3 v, mat4 mat);
vec3 vec3_unproject(vec3 source, mat4 projection, mat4 view);
float mat4_determinant(mat4 mat);

quat quat_from_mat4(mat4 mat);
mat4 mat4_from_quat(quat q);
quat quat_transform(quat q, mat4 mat);

mat4 mat4_invert(mat4 mat);
mat4 mat4_translate(vec3 v);
mat4 mat4_rotate(vec3 axis, float angle);
mat4 mat4_scale(vec3 scale);
mat4 frustum(float left, float right, float bottom, float top, float near, float far);
mat4 perspective(float fovY, float aspect, float nearPlane, float farPlane);
mat4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
mat4 look_at(vec3 center, vec3 target, vec3 up);
#endif

enum glm_easing_fn {
    GLM_EASING_LINEAR = 0,
    GLM_EASING_SINE,
    GLM_EASING_CIRCULAR,
    GLM_EASING_CUBIC,
    GLM_EASING_QUAD,
    GLM_EASING_EXPONENTIAL,
    GLM_EASING_BACK,
    GLM_EASING_BOUNCE,
    GLM_EASING_ELASTIC
};

enum glm_easing_t {
    EASE_IN = 1,
    EASE_OUT,
    EASE_INOUT
};

float easing(enum glm_easing_fn fn, enum glm_easing_t, float t, float b, float c, float d);
bool float_cmp(float a, float b);
bool double_cmp(double a, double b);
float remap(float x, float in_min, float in_max, float out_min, float out_max);

#if defined(__cplusplus)
}
#endif
#endif // GLM_HEADER
