/* glm.h -- https://github.com/takeiteasy/fungl
 
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
extern "C"
{
#endif

#ifndef GLM_NO_PRINT
#include <stdio.h>
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

#define FLOAT_CMP(A, B) ((fabsf((A) - (B))) <= (EPSILON * fmaxf(1.f, fmaxf(fabsf((A)), fabsf((B))))))
#define FLOAT_CLOSE_ENOUGH FLOAT_CMP
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define CLAMP(VALUE, _MIN, _MAX) (MIN(MAX((VALUE), (_MIN)), (_MAX)))
#define TO_DEGREES(RADIANS) ((RADIANS) * (180.f / PI))
#define TO_RADIANS(DEGREES) ((DEGREES) * (PI / 180.f))
#define REMAP(X, INMIN, INMAX, OUTMIN, OUTMAX) ((X) - (INMIN)) * ((OUTMAX) - (OUTMIN)) / ((INMAX) - (INMIN)) + (OUTMIN)

#define BYTES(n) (n)
#define KILOBYTES(n) (n << 10)
#define MEGABYTES(n) (n << 20)
#define GIGABYTES(n) (((uint64_t)n) << 30)
#define TERABYTES(n) (((uint64_t)n) << 40)

#define THOUSAND(n) ((n) * 1000)
#define MILLION(n) ((n) * 1000000)
#define BILLION(n) ((n) * 1000000000LL)

#define __GLM_TYPES \
    X(2)            \
    X(3)            \
    X(4)

#define __DEF_GLM_VECTOR(SZ)                                    \
    typedef float vec##SZ __attribute__((ext_vector_type(SZ))); \
    vec##SZ Vec##SZ(float x, ...);                              \
    vec##SZ vec##SZ##_zero(void);                               \
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

#define X(N) \
    __DEF_GLM_VECTOR(N)
__GLM_TYPES
#undef X
typedef vec4 quat;
typedef vec4 color;

#define __DEF_GLM_IVECTOR(TYPE, SIZE) \
    typedef int vec##SIZE##i __attribute__((vector_size(SIZE * sizeof(TYPE))));

#define GLM_IVECTOR_TYPE_DEFAULT int
#ifndef GLM_IVECTOR_TYPE
#define GLM_IVECTOR_TYPE GLM_IVECTOR_TYPE_DEFAULT
#endif
#define X(N) __DEF_GLM_IVECTOR(int, N)
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
#define swizzle(V, ...) {MAP_LIST_UD(SWZL_MAP, V, __VA_ARGS__)}

#ifndef GLM_NO_MATRICES
#define __DEF_GLM_MATRIX(COLUMNS, ROWS) \
    typedef float mat##COLUMNS##ROWS __attribute__((matrix_type((COLUMNS), (ROWS))));

#define X(N)                                      \
    __DEF_GLM_MATRIX(N, N)                        \
    typedef mat##N##N mat##N;                     \
    mat##N Mat##N(void);                          \
    mat##N mat##N##_identity(void);               \
    float mat##N##_trace(mat##N mat);             \
    mat##N mat##N##_transpose(mat##N);            \
    vec##N mat##N##_column(mat##N, unsigned int); \
    vec##N mat##N##_row(mat##N, unsigned int);
__GLM_TYPES
#undef X
#endif

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

#if defined(__cplusplus)
}
#endif
#endif // GLM_HEADER
