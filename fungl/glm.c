/* glm.c -- Basic Linear Algebra library
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

#include "glm.h"

#ifndef GLM_NO_MATRICES
#define X(N)                                                \
    mat##N Mat##N(void)                                     \
    {                                                       \
        mat##N mat;                                         \
        memset(&mat, 0, sizeof(float) * (N * N));           \
        return mat;                                         \
    }                                                       \
    mat##N mat##N##_identity(void)                          \
    {                                                       \
        mat##N mat = Mat##N();                              \
        for (int i = 0; i < N; i++)                         \
            mat[i][i] = 1.f;                                \
        return mat;                                         \
    }                                                       \
    bool mat##N##_is_identity(mat##N mat)                   \
    {                                                       \
        for (int y = 0; y < N; y++)                         \
            for (int x = 0; x < N; x++)                     \
                if (x == y) {                               \
                    if (mat[y][x] != 1.f)                   \
                        return false;                       \
                } else {                                    \
                    if (mat[y][x] != 0.f)                   \
                        return false;                       \
                }                                           \
        return true;                                        \
    }                                                       \
    bool mat##N##_is_zero(mat##N mat)                       \
    {                                                       \
        for (int y = 0; y < N; y++)                         \
            for (int x = 0; x < N; x++)                     \
                if (mat[y][x] != 0.f)                       \
                    return false;                           \
        return true;                                        \
    }                                                       \
    float mat##N##_trace(mat##N mat)                        \
    {                                                       \
        float result = 0.f;                                 \
        for (int i = 0; i < N; i++)                         \
            result += mat[i][i];                            \
        return result;                                      \
    }                                                       \
    mat##N mat##N##_transpose(mat##N mat)                   \
    {                                                       \
        mat##N result = Mat##N();                           \
        for (int x = 0; x < N; x++)                         \
            for (int y = 0; y < N; y++)                     \
                result[x][y] = mat[y][x];                   \
        return result;                                      \
    }                                                       \
    vec##N mat##N##_column(mat##N mat, unsigned int column) \
    {                                                       \
        vec##N result = vec##N##_zero();                    \
        if (column >= N)                                    \
            return result;                                  \
        for (int i = 0; i < N; i++)                         \
            result[i] = mat[i][column];                     \
        return result;                                      \
    }                                                       \
    vec##N mat##N##_row(mat##N mat, unsigned int row)       \
    {                                                       \
        vec##N result = vec##N##_zero();                    \
        if (row >= N)                                       \
            return result;                                  \
        for (int i = 0; i < N; i++)                         \
            result[i] = mat[row][i];                        \
        return result;                                      \
    }
__GLM_TYPES
#undef X
#endif // GLM_NO_MATRICES

#define X(N)                                                  \
    vec##N vec##N##_zero(void)                                \
    {                                                         \
        return (vec##N){0};                                   \
    }                                                         \
    bool vec##N##_is_zero(vec##N vec)                         \
    {                                                         \
        for (int i = 0; i < N; i++)                           \
            if (vec[i] != 0.f)                                \
                return false;                                 \
                return true;                                  \
    }                                                         \
    float vec##N##_sum(vec##N vec)                            \
    {                                                         \
        float result = 0.f;                                   \
        for (int i = 0; i < N; i++)                           \
            result += vec[i];                                 \
        return result;                                        \
    }                                                         \
    int vec##N##_cmp(vec##N a, vec##N b)                      \
    {                                                         \
        int result = 1;                                       \
        for (int i = 0; i < N; i++)                           \
            if (!float_cmp(a[i], b[i]))                       \
                return 0;                                     \
        return result;                                        \
    }                                                         \
    float vec##N##_length_sqr(vec##N vec)                     \
    {                                                         \
        return vec##N##_sum(vec * vec);                       \
    }                                                         \
    float vec##N##_length(vec##N vec)                         \
    {                                                         \
        return sqrtf(vec##N##_length_sqr(vec));               \
    }                                                         \
    float vec##N##_dot(vec##N a, vec##N b)                    \
    {                                                         \
        return vec##N##_sum(a * b);                           \
    }                                                         \
    vec##N vec##N##_normalize(vec##N vec)                     \
    {                                                         \
        vec##N result = vec##N##_zero();                      \
        float L = vec##N##_length(vec);                       \
        for (int i = 0; i < N; i++)                           \
            result[i] = vec[i] * (1.f / L);                   \
        return result;                                        \
    }                                                         \
    float vec##N##_distance_sqr(vec##N a, vec##N b)           \
    {                                                         \
        return vec##N##_length_sqr(b - a);                    \
    }                                                         \
    float vec##N##_distance(vec##N a, vec##N b)               \
    {                                                         \
        return sqrtf(vec##N##_distance_sqr(a, b));            \
    }                                                         \
    vec##N vec##N##_clamp(vec##N vec, vec##N min, vec##N max) \
    {                                                         \
        vec##N result = vec##N##_zero();                      \
        for (int i = 0; i < N; i++)                           \
            result[i] = CLAMP(vec[i], min[i], max[i]);        \
        return result;                                        \
    }                                                         \
    vec##N vec##N##_lerp(vec##N a, vec##N b, float t)         \
    {                                                         \
        return a + t * (b - a);                               \
    }
__GLM_TYPES
#undef X

#ifndef GLM_NO_PRINT
#ifndef GLM_NO_MATRICES
#define X(N)                                \
    void mat##N##_print(mat##N mat)         \
    {                                       \
        for (int x = 0; x < N; x++)         \
        {                                   \
            printf("| ");                   \
            for (int y = 0; y < N; y++)     \
            {                               \
                printf("%.2f ", mat[y][x]); \
            }                               \
            puts("|");                      \
        }                                   \
    }
__GLM_TYPES
#undef X
#define X(N)                        \
    void vec##N##_print(vec##N vec) \
    {                               \
        printf("{ ");               \
        for (int i = 0; i < N; i++) \
            printf("%f ", vec[i]);  \
        puts("}");                  \
    }
__GLM_TYPES
#undef X
#endif // GLM_NO_MATRICES
#endif // GLM_NO_PRINT

float vec2_angle(vec2 v1, vec2 v2) {
    return atan2f(v2.y, v2.x) - atan2f(v1.y, v1.x);
}

vec2 vec2_rotate(vec2 v, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return (vec2) { v.x*c - v.y*s,  v.x*s + v.y*c };
}

vec2 vec2_move_towards(vec2 v, vec2 target, float maxDistance) {
    vec2 delta = target - v;
    float dist = vec2_length_sqr(delta);
    if (!dist || (maxDistance >= 0 && dist <= maxDistance * maxDistance))
        return target;
    else
        return v + (delta / (sqrtf(dist) * maxDistance));
}

vec2 vec2_reflect(vec2 v, vec2 normal) {
    return v - (2.f * normal) * vec2_dot(v, normal);
}

float vec2_cross(vec2 a, vec2 b) {
    return a.x * b.y - a.y * b.x;
}

vec3 vec3_reflect(vec3 v, vec3 normal) {
    return v - (2.f * normal) * vec3_dot(v, normal);
}

vec3 vec3_cross(vec3 v1, vec3 v2) {
    return (vec3) { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
}

vec3 vec3_perpendicular(vec3 v) {
    float min = (float) fabs(v.x);
    vec3 a = {1.0f, 0.0f, 0.0f};
    if (fabsf(v.y) < min) {
        min = (float) fabs(v.y);
        a = (vec3){0.0f, 1.0f, 0.0f};
    }
    if (fabsf(v.z) < min)
        a = (vec3){0.0f, 0.0f, 1.0f};
    return (vec3) { v.y*a.z - v.z*a.y,
                    v.z*a.x - v.x*a.z,
                    v.x*a.y - v.y*a.x };
}

float vec3_angle(vec3 v1, vec3 v2) {
    return atan2f(vec3_length(vec3_cross(v1, v2)), vec3_dot(v1, v2));
}

vec3 quat_rotate_vec3(vec3 v, quat q) {
    return (vec3) {
        v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + v.y*(2*q.x*q.y - 2*q.w*q.z) + v.z*(2*q.x*q.z + 2*q.w*q.y),
        v.x*(2*q.w*q.z + 2*q.x*q.y) + v.y*(q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z) + v.z*(-2*q.w*q.x + 2*q.y*q.z),
        v.x*(-2*q.w*q.y + 2*q.x*q.z) + v.y*(2*q.w*q.x + 2*q.y*q.z)+ v.z*(q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z)
    };
}

vec3 vec3_rotate_axis_angle(vec3 v, vec3 axis, float angle) {
    // Using Euler-Rodrigues Formula
    // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

    axis *= (1.0f / (CLAMP(vec3_length(axis), 0.f, 1.f)));

    angle /= 2.0f;
    vec3 w = axis * sinf(angle);
    vec3 wv = vec3_cross(w, v);
    return v + (wv * (cosf(angle) * 2.f)) + (vec3_cross(w, wv) * 2.f);
}

vec3 vec3_refract(vec3 v, vec3 n, float r) {
    float dot = vec3_dot(v, n);
    float d = 1.0f - r*r*(1.0f - dot*dot);
    return d < 0 ? vec3_zero() : r * v - (r * dot + sqrtf(d)) * n;
}

vec3 vec3_barycenter(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = b - a;
    vec3 v1 = c - a;
    vec3 v2 = p - a;
    float d00 = vec3_dot(v0, v1);
    float d01 = vec3_dot(v1, v1);
    float d11 = vec3_dot(v1, v1);
    float d20 = vec3_dot(v2, v0);
    float d21 = vec3_dot(v2, v1);
    float denom = d00*d11 - d01*d01;
    float y = (d11*d20 - d01*d21)/denom;
    float z = (d00*d21 - d01*d20)/denom;
    return (vec3) {1.0f - (z + y), y, z};
}

quat quat_zero(void) {
    return (quat){0};
}

quat quat_identity(void) {
    return (quat){0.f, 0.f, 0.f, 1.f};
}

quat quat_mul(quat q1, quat q2) {
    float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
    float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;
    return (quat) {
        qax*qbw + qaw*qbx + qay*qbz - qaz*qby,
        qay*qbw + qaw*qby + qaz*qbx - qax*qbz,
        qaz*qbw + qaw*qbz + qax*qby - qay*qbx,
        qaw*qbw - qax*qbx - qay*qby - qaz*qbz
    };
}

quat quat_invert(quat q) {
    float lsqr = vec4_length_sqr(q);
    quat result = q;
    if (lsqr != 0.f) {
        float inv = 1.f / lsqr;
        result *= (vec4){-inv, -inv, -inv, inv};
    }
    return result;
}

quat quat_to_from_vec3(vec3 from, vec3 to) {
    vec3 cross = vec3_cross(from, to);
    return vec4_normalize((quat){cross.x, cross.y, cross.z, 1.f + vec3_dot(from, to)});
}

quat quat_from_mat4(mat4 mat) {
    float fourWSquaredMinus1 = mat[0][0] + mat[1][1] + mat[2][2];
    float fourXSquaredMinus1 = mat[0][0] - mat[1][1] - mat[2][2];
    float fourYSquaredMinus1 = mat[1][1] - mat[0][0] - mat[2][2];
    float fourZSquaredMinus1 = mat[2][2] - mat[0][0] - mat[1][1];

    int biggestIndex = 0;
    float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.f) * .5f;
    float mult = .25f / biggestVal;
    switch (biggestIndex)
    {
        case 0:
            return (quat) {
                (mat[2][1] - mat[1][2]) * mult,
                (mat[0][2] - mat[2][0]) * mult,
                (mat[1][0] - mat[0][1]) * mult,
                biggestVal
            };
            break;
        case 1:
            return (quat) {
                biggestVal,
                (mat[1][0] + mat[0][1]) * mult,
                (mat[0][2] + mat[2][0]) * mult,
                (mat[2][1] - mat[1][2]) * mult
            };
        case 2:
            return (quat) {
                biggestVal,
                (mat[1][0] + mat[0][1]) * mult,
                (mat[0][2] + mat[2][0]) * mult,
                (mat[2][1] - mat[1][2]) * mult
            };
        case 3:
            return (quat) {
                biggestVal,
                (mat[1][0] + mat[0][1]) * mult,
                (mat[0][2] + mat[2][0]) * mult,
                (mat[2][1] - mat[1][2]) * mult
            };
        default:
            return quat_zero();
    }
}

vec3 vec3_unproject(vec3 source, mat4 projection, mat4 view) {
    quat p = quat_transform((quat){source.x, source.y, source.z, 1.f }, mat4_invert(view * projection));
    return (vec3) {
        p.x / p.w,
        p.y / p.w,
        p.z / p.w
    };
}

#ifndef GLM_NO_MATRICES
vec3 vec3_transform(vec3 v, mat4 mat) {
    return (vec3) {
        mat[0][0]*v.x + mat[0][1]*v.y + mat[0][2]*v.z + mat[0][3],
        mat[1][0]*v.x + mat[1][1]*v.y + mat[1][2]*v.z + mat[1][3],
        mat[2][0]*v.x + mat[2][1]*v.y + mat[2][2]*v.z + mat[2][3]
    };
}

mat4 mat4_from_quat(quat q) {
    float a2 = q.x*q.x;
    float b2 = q.y*q.y;
    float c2 = q.z*q.z;
    float ac = q.x*q.z;
    float ab = q.x*q.y;
    float bc = q.y*q.z;
    float ad = q.w*q.x;
    float bd = q.w*q.y;
    float cd = q.w*q.z;

    mat4 result = mat4_identity();
    result[0][0] = 1 - 2*(b2 + c2);
    result[1][0] = 2*(ab + cd);
    result[2][0] = 2*(ac - bd);

    result[0][1] = 2*(ab - cd);
    result[1][1] = 1 - 2*(a2 + c2);
    result[2][1] = 2*(bc + ad);

    result[0][2] = 2*(ac + bd);
    result[1][2] = 2*(bc - ad);
    result[2][2] = 1 - 2*(a2 + b2);
    return result;
}

quat quat_from_axis_angle(vec3 axis, float angle) {
    float axisLength = vec3_length(axis);
    if (axisLength == 0.f)
        return quat_identity();

    axis = vec3_normalize(axis);
    angle *= .5f;
    float sinres = sinf(angle);
    float cosres = cosf(angle);
    return (quat) {
        axis.x*sinres,
        axis.y*sinres,
        axis.z*sinres,
        cosres
    };
}

void quat_to_axis_angle(quat q, vec3 *outAxis, float *outAngle) {
    if (fabsf(q.w) > 1.0f)
        q = vec4_normalize(q);
    float resAngle = 2.0f*acosf(q.w);
    float den = sqrtf(1.0f - q.w*q.w);
    vec3 qxyz = (vec3){q.x, q.y, q.z};
    vec3 resAxis = den > EPSILON ? qxyz / den : (vec3){1.f, 0.f, 0.f};
    *outAxis = resAxis;
    *outAngle = resAngle;
}

quat quat_from_euler(float pitch, float yaw, float roll) {
    float x0 = cosf(pitch*0.5f);
    float x1 = sinf(pitch*0.5f);
    float y0 = cosf(yaw*0.5f);
    float y1 = sinf(yaw*0.5f);
    float z0 = cosf(roll*0.5f);
    float z1 = sinf(roll*0.5f);

    return (quat) {
        x1*y0*z0 - x0*y1*z1,
        x0*y1*z0 + x1*y0*z1,
        x0*y0*z1 - x1*y1*z0,
        x0*y0*z0 + x1*y1*z1
    };
}

vec3 quat_to_euler(quat q) {
    // Roll (x-axis rotation)
    float x0 = 2.0f*(q.w*q.x + q.y*q.z);
    float x1 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
    // Pitch (y-axis rotation)
    float y0 = 2.0f*(q.w*q.y - q.z*q.x);
    y0 = y0 > 1.0f ? 1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    // Yaw (z-axis rotation)
    float z0 = 2.0f*(q.w*q.z + q.x*q.y);
    float z1 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
    return (vec3) {
        atan2f(x0, x1),
        asinf(y0),
        atan2f(z0, z1)
    };
}

quat quat_transform(quat q, mat4 mat) {
    return (quat) {
        mat[0][0]*q.x + mat[0][1]*q.y + mat[0][2]*q.z + mat[0][3]*q.w,
        mat[1][0]*q.x + mat[1][1]*q.y + mat[1][2]*q.z + mat[1][3]*q.w,
        mat[2][0]*q.x + mat[2][1]*q.y + mat[2][2]*q.z + mat[2][3]*q.w,
        mat[3][0]*q.x + mat[3][1]*q.y + mat[3][2]*q.z + mat[3][3]*q.w
    };
}

int quat_cmp(quat p, quat q) {
    return (((fabsf(p.x - q.x)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) && ((fabsf(p.y - q.y)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) && ((fabsf(p.z - q.z)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) && ((fabsf(p.w - q.w)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w)))))) || (((fabsf(p.x + q.x)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) && ((fabsf(p.y + q.y)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) && ((fabsf(p.z + q.z)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) && ((fabsf(p.w + q.w)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w))))));
}

float mat4_determinant(mat4 mat) {
    return mat[0][3]*mat[1][2]*mat[2][1]*mat[3][0] - mat[0][2]*mat[1][3]*mat[2][1]*mat[3][0] - mat[0][3]*mat[1][1]*mat[2][2]*mat[3][0] + mat[0][1]*mat[1][3]*mat[2][2]*mat[3][0] + mat[0][2]*mat[1][1]*mat[2][3]*mat[3][0] - mat[0][1]*mat[1][2]*mat[2][3]*mat[3][0] - mat[0][3]*mat[1][2]*mat[2][0]*mat[3][1] + mat[0][2]*mat[1][3]*mat[2][0]*mat[3][1] + mat[0][3]*mat[1][0]*mat[2][2]*mat[3][1] - mat[0][0]*mat[1][3]*mat[2][2]*mat[3][1] - mat[0][2]*mat[1][0]*mat[2][3]*mat[3][1] + mat[0][0]*mat[1][2]*mat[2][3]*mat[3][1] + mat[0][3]*mat[1][1]*mat[2][0]*mat[3][2] - mat[0][1]*mat[1][3]*mat[2][0]*mat[3][2] - mat[0][3]*mat[1][0]*mat[2][1]*mat[3][2] + mat[0][0]*mat[1][3]*mat[2][1]*mat[3][2] + mat[0][1]*mat[1][0]*mat[2][3]*mat[3][2] - mat[0][0]*mat[1][1]*mat[2][3]*mat[3][2] - mat[0][2]*mat[1][1]*mat[2][0]*mat[3][3] + mat[0][1]*mat[1][2]*mat[2][0]*mat[3][3] + mat[0][2]*mat[1][0]*mat[2][1]*mat[3][3] - mat[0][0]*mat[1][2]*mat[2][1]*mat[3][3] - mat[0][1]*mat[1][0]*mat[2][2]*mat[3][3] + mat[0][0]*mat[1][1]*mat[2][2]*mat[3][3];
}

mat4 mat4_invert(mat4 mat) {
    float b00 = mat[0][0]*mat[1][1] - mat[1][0]*mat[0][1];
    float b01 = mat[0][0]*mat[2][1] - mat[2][0]*mat[0][1];
    float b02 = mat[0][0]*mat[3][1] - mat[3][0]*mat[0][1];
    float b03 = mat[1][0]*mat[2][1] - mat[2][0]*mat[1][1];
    float b04 = mat[1][0]*mat[3][1] - mat[3][0]*mat[1][1];
    float b05 = mat[2][0]*mat[3][1] - mat[3][0]*mat[2][1];
    float b06 = mat[0][2]*mat[1][3] - mat[1][2]*mat[0][3];
    float b07 = mat[0][2]*mat[2][3] - mat[2][2]*mat[0][3];
    float b08 = mat[0][2]*mat[3][3] - mat[3][2]*mat[0][3];
    float b09 = mat[1][2]*mat[2][3] - mat[2][2]*mat[1][3];
    float b10 = mat[1][2]*mat[3][3] - mat[3][2]*mat[1][3];
    float b11 = mat[2][2]*mat[3][3] - mat[3][2]*mat[2][3];
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    mat4 result = Mat4();
    result[0][0] = (mat[1][1]*b11 - mat[2][1]*b10 + mat[3][1]*b09)*invDet;
    result[1][0] = (-mat[1][0]*b11 + mat[2][0]*b10 - mat[3][0]*b09)*invDet;
    result[2][0] = (mat[1][3]*b05 - mat[2][3]*b04 + mat[3][3]*b03)*invDet;
    result[3][0] = (-mat[1][2]*b05 + mat[2][2]*b04 - mat[3][2]*b03)*invDet;
    result[0][1] = (-mat[0][1]*b11 + mat[2][1]*b08 - mat[3][1]*b07)*invDet;
    result[1][1] = (mat[0][0]*b11 - mat[2][0]*b08 + mat[3][0]*b07)*invDet;
    result[2][1] = (-mat[0][3]*b05 + mat[2][3]*b02 - mat[3][3]*b01)*invDet;
    result[3][1] = (mat[0][2]*b05 - mat[2][2]*b02 + mat[3][2]*b01)*invDet;
    result[0][2] = (mat[0][1]*b10 - mat[1][1]*b08 + mat[3][1]*b06)*invDet;
    result[1][2] = (-mat[0][0]*b10 + mat[1][0]*b08 - mat[3][0]*b06)*invDet;
    result[2][2] = (mat[0][3]*b04 - mat[1][3]*b02 + mat[3][3]*b00)*invDet;
    result[3][2] = (-mat[0][2]*b04 + mat[1][2]*b02 - mat[3][2]*b00)*invDet;
    result[0][3] = (-mat[0][1]*b09 + mat[1][1]*b07 - mat[2][1]*b06)*invDet;
    result[1][3] = (mat[0][0]*b09 - mat[1][0]*b07 + mat[2][0]*b06)*invDet;
    result[2][3] = (-mat[0][3]*b03 + mat[1][3]*b01 - mat[2][3]*b00)*invDet;
    result[3][3] = (mat[0][2]*b03 - mat[1][2]*b01 + mat[2][2]*b00)*invDet;
    return result;
}

mat4 mat4_translate(vec3 v) {
    mat4 result = mat4_identity();
    result[0][3] = v.x;
    result[1][3] = v.y;
    result[2][3] = v.z;
    return result;
}

mat4 mat4_rotate(vec3 axis, float angle) {
    vec3 a = vec3_length_sqr(axis);
    float s = sinf(angle);
    float c = cosf(angle);
    float t = 1.f - c;

    mat4 result = mat4_identity();
    result[0][0] = a.x*a.x*t + c;
    result[1][0] = a.y*a.x*t + a.z*s;
    result[2][0] = a.z*a.x*t - a.y*s;
    result[0][1] = a.x*a.y*t - a.z*s;
    result[1][1] = a.y*a.y*t + c;
    result[2][1] = a.z*a.y*t + a.x*s;
    result[0][2] = a.x*a.z*t + a.y*s;
    result[1][2] = a.y*a.z*t - a.x*s;
    result[2][2] = a.z*a.z*t + c;
    return result;
}

mat4 mat4_scale(vec3 scale) {
    mat4 result = Mat4();
    result[0][0] = scale.x;
    result[1][1] = scale.y;
    result[2][2] = scale.z;
    result[3][3] = 1.f;
    return result;
}

mat4 frustum(float left, float right, float bottom, float top, float near, float far) {
    float rl = right - left;
    float tb = top - bottom;
    float fn = far - near;

    mat4 result = Mat4();
    result[0][0] = (near*2.0f)/rl;
    result[1][1] = (near*2.0f)/tb;
    result[0][2] = (right + left)/rl;
    result[1][2] = (top + bottom)/tb;
    result[2][2] = -(far + near)/fn;
    result[3][2] = -1.0f;
    result[2][3] = -(far*near*2.0f)/fn;
    return result;
}

mat4 perspective(float fovY, float aspect, float nearPlane, float farPlane) {
    float top = nearPlane*tan(fovY*0.5);
    float bottom = -top;
    float right = top*aspect;
    float left = -right;
    float rl = right - left;
    float tb = top - bottom;
    float fn = farPlane - nearPlane;

    mat4 result = Mat4();
    result[0][0] = (nearPlane*2.0f)/rl;
    result[1][1] = (nearPlane*2.0f)/tb;
    result[0][2] = (right + left)/rl;
    result[1][2] = (top + bottom)/tb;
    result[2][2] = -(farPlane + nearPlane)/fn;
    result[3][2] = -1.0f;
    result[2][3] = -(farPlane*nearPlane*2.0f)/fn;
    return result;
}

mat4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    float rl = right - left;
    float tb = top - bottom;
    float fn = farPlane - nearPlane;

    mat4 result = Mat4();
    result[0][0] = 2.0f/rl;
    result[1][1] = 2.0f/tb;
    result[2][2] = -2.0f/fn;
    result[0][3] = -(left + right)/rl;
    result[1][3] = -(top + bottom)/tb;
    result[2][3] = -(farPlane + nearPlane)/fn;
    result[3][3] = 1.0f;
    return result;
}

mat4 look_at(vec3 eye, vec3 target, vec3 up) {
    vec3 vz = vec3_normalize(eye - target);
    vec3 vx = vec3_normalize(vec3_cross(up, vz));
    vec3 vy = vec3_cross(vz, vx);

    mat4 result = Mat4();
    result[0][0] = vx.x;
    result[1][0] = vy.x;
    result[2][0] = vz.x;
    result[0][1] = vx.y;
    result[1][1] = vy.y;
    result[2][1] = vz.y;
    result[0][2] = vx.z;
    result[1][2] = vy.z;
    result[2][2] = vz.z;
    result[0][3] = -vec3_dot(vx, eye);
    result[1][3] = -vec3_dot(vy, eye);
    result[2][3] = -vec3_dot(vz, eye);
    result[3][3] = 1.0f;
    return result;
}
#endif

static float ease_linear_None(float t, float b, float c, float d) {
    return (c * t / d + b);
}

static float ease_linear_in(float t, float b, float c, float d) {
    return (c * t / d + b);
}

static float ease_linear_out(float t, float b, float c, float d) {
    return (c * t / d + b);
}

static float ease_linear_inout(float t, float b, float c, float d) {
    return (c * t / d + b);
}

static float ease_sine_in(float t, float b, float c, float d) {
    return (-c * cosf(t / d * (PI / 2.0f)) + c + b);
}

static float ease_sine_out(float t, float b, float c, float d) {
    return (c * sinf(t / d * (PI / 2.0f)) + b);
}

static float ease_sine_inout(float t, float b, float c, float d) {
    return (-c / 2.0f * (cosf(PI * t / d) - 1.0f) + b);
}

static float ease_circ_in(float t, float b, float c, float d) {
    t /= d;
    return (-c * (sqrtf(1.0f - t * t) - 1.0f) + b);
}

static float ease_circ_out(float t, float b, float c, float d) {
    t = t / d - 1.0f;
    return (c * sqrtf(1.0f - t * t) + b);
}

static float ease_circ_inout(float t, float b, float c, float d) {
    if ((t /= d / 2.0f) < 1.0f)
        return (-c / 2.0f * (sqrtf(1.0f - t * t) - 1.0f) + b);
    t -= 2.0f;
    return (c / 2.0f * (sqrtf(1.0f - t * t) + 1.0f) + b);
}

static float ease_cubic_in(float t, float b, float c, float d) {
    t /= d;
    return (c * t * t * t + b);
}

static float ease_cubic_out(float t, float b, float c, float d) {
    t = t / d - 1.0f;
    return (c * (t * t * t + 1.0f) + b);
}

static float ease_cubic_inout(float t, float b, float c, float d) {
    if ((t /= d / 2.0f) < 1.0f)
        return (c / 2.0f * t * t * t + b);
    t -= 2.0f;
    return (c / 2.0f * (t * t * t + 2.0f) + b);
}

static float ease_quad_in(float t, float b, float c, float d) {
    t /= d;
    return (c * t * t + b);
}

static float ease_quad_out(float t, float b, float c, float d) {
    t /= d;
    return (-c * t * (t - 2.0f) + b);
}

static float ease_quad_inout(float t, float b, float c, float d) {
    if ((t /= d / 2) < 1)
        return (((c / 2) * (t * t)) + b);
    return (-c / 2.0f * (((t - 1.0f) * (t - 3.0f)) - 1.0f) + b);
}

static float ease_expo_in(float t, float b, float c, float d) {
    return (t == 0.0f) ? b : (c * powf(2.0f, 10.0f * (t / d - 1.0f)) + b);
}

static float ease_expo_out(float t, float b, float c, float d) {
    return (t == d) ? (b + c) : (c * (-powf(2.0f, -10.0f * t / d) + 1.0f) + b);
}

static float ease_expo_inout(float t, float b, float c, float d) {
    if (t == 0.0f)
        return b;
    if (t == d)
        return (b + c);
    if ((t /= d / 2.0f) < 1.0f)
        return (c / 2.0f * powf(2.0f, 10.0f * (t - 1.0f)) + b);

    return (c / 2.0f * (-powf(2.0f, -10.0f * (t - 1.0f)) + 2.0f) + b);
}

static float ease_back_in(float t, float b, float c, float d) {
    float s = 1.70158f;
    float postFix = t /= d;
    return (c * (postFix)*t * ((s + 1.0f) * t - s) + b);
}

static float ease_back_out(float t, float b, float c, float d) {
    float s = 1.70158f;
    t = t / d - 1.0f;
    return (c * (t * t * ((s + 1.0f) * t + s) + 1.0f) + b);
}

static float ease_back_inout(float t, float b, float c, float d) {
    float s = 1.70158f;
    if ((t /= d / 2.0f) < 1.0f) {
        s *= 1.525f;
        return (c / 2.0f * (t * t * ((s + 1.0f) * t - s)) + b);
    }

    float postFix = t -= 2.0f;
    s *= 1.525f;
    return (c / 2.0f * ((postFix)*t * ((s + 1.0f) * t + s) + 2.0f) + b);
}

static float ease_bounce_out(float t, float b, float c, float d) {
    if ((t /= d) < (1.0f / 2.75f)) {
        return (c * (7.5625f * t * t) + b);
    } else if (t < (2.0f / 2.75f)) {
        float postFix = t -= (1.5f / 2.75f);
        return (c * (7.5625f * (postFix)*t + 0.75f) + b);
    } else if (t < (2.5 / 2.75)) {
        float postFix = t -= (2.25f / 2.75f);
        return (c * (7.5625f * (postFix)*t + 0.9375f) + b);
    } else {
        float postFix = t -= (2.625f / 2.75f);
        return (c * (7.5625f * (postFix)*t + 0.984375f) + b);
    }
}

static float ease_bounce_in(float t, float b, float c, float d) {
    return (c - ease_bounce_out(d - t, 0.0f, c, d) + b);

}

static float ease_bounce_inout(float t, float b, float c, float d) {
    if (t < d / 2.0f)
        return (ease_bounce_in(t * 2.0f, 0.0f, c, d) * 0.5f + b);
    else
        return (ease_bounce_out(t * 2.0f - d, 0.0f, c, d) * 0.5f + c * 0.5f + b);
}

static float ease_elastic_in(float t, float b, float c, float d) {
    if (t == 0.0f)
        return b;
    if ((t /= d) == 1.0f)
        return (b + c);

    float p = d * 0.3f;
    float a = c;
    float s = p / 4.0f;
    float postFix = a * powf(2.0f, 10.0f * (t -= 1.0f));

    return (-(postFix * sinf((t * d - s) * (2.0f * PI) / p)) + b);
}

static float ease_elastic_out(float t, float b, float c, float d) {
    if (t == 0.0f)
        return b;
    if ((t /= d) == 1.0f)
        return (b + c);

    float p = d * 0.3f;
    float a = c;
    float s = p / 4.0f;

    return (a * powf(2.0f, -10.0f * t) * sinf((t * d - s) * (2.0f * PI) / p) + c + b);
}

static float ease_elastic_inout(float t, float b, float c, float d) {
    if (t == 0.0f)
        return b;
    if ((t /= d / 2.0f) == 2.0f)
        return (b + c);

    float p = d * (0.3f * 1.5f);
    float a = c;
    float s = p / 4.0f;

    if (t < 1.0f) {
        float postFix = a * powf(2.0f, 10.0f * (t -= 1.0f));
        return -0.5f * (postFix * sinf((t * d - s) * (2.0f * PI) / p)) + b;
    }

    float postFix = a * powf(2.0f, -10.0f * (t -= 1.0f));

    return (postFix * sinf((t * d - s) * (2.0f * PI) / p) * 0.5f + c + b);
}

float easing(enum glm_easing_fn fn, enum glm_easing_t type, float t, float b, float c, float d) {
    switch (fn) {
        default:
        case GLM_EASING_LINEAR:
            switch ((int)type) {
                default:
                case 0:
                    return ease_linear_None(t, b, c, d);
                case EASE_IN:
                    return ease_linear_in(t, b, c, d);
                case EASE_OUT:
                    return ease_linear_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_linear_inout(t, b, c, d);
            }
        case GLM_EASING_SINE:
            switch (type) {
                case EASE_IN:
                    return ease_sine_in(t, b, c, d);
                case EASE_OUT:
                    return ease_sine_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_sine_inout(t, b, c, d);
            };
        case GLM_EASING_CIRCULAR:
            switch (type) {
                case EASE_IN:
                    return ease_circ_in(t, b, c, d);
                case EASE_OUT:
                    return ease_circ_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_circ_inout(t, b, c, d);
            };
        case GLM_EASING_CUBIC:
            switch (type) {
                case EASE_IN:
                    return ease_cubic_in(t, b, c, d);
                case EASE_OUT:
                    return ease_cubic_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_cubic_inout(t, b, c, d);
            };
        case GLM_EASING_QUAD:
            switch (type) {
                case EASE_IN:
                    return ease_quad_in(t, b, c, d);
                case EASE_OUT:
                    return ease_quad_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_quad_inout(t, b, c, d);
            };
        case GLM_EASING_EXPONENTIAL:
            switch (type) {
                case EASE_IN:
                    return ease_expo_in(t, b, c, d);
                case EASE_OUT:
                    return ease_expo_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_expo_inout(t, b, c, d);
            };
        case GLM_EASING_BACK:
            switch (type) {
                case EASE_IN:
                    return ease_back_in(t, b, c, d);
                case EASE_OUT:
                    return ease_back_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_back_inout(t, b, c, d);
            };
        case GLM_EASING_BOUNCE:
            switch (type) {
                case EASE_IN:
                    return ease_bounce_in(t, b, c, d);
                case EASE_OUT:
                    return ease_bounce_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_bounce_inout(t, b, c, d);
            };
        case GLM_EASING_ELASTIC:
            switch (type) {
                case EASE_IN:
                    return ease_elastic_in(t, b, c, d);
                case EASE_OUT:
                    return ease_elastic_out(t, b, c, d);
                case EASE_INOUT:
                    return ease_elastic_inout(t, b, c, d);
            };
    }
}

bool float_cmp(float a, float b) {
    return fabsf(a - b) <= EPSILON * fmaxf(1.f, fmaxf(fabsf(a), fabsf(b)));
}

bool double_cmp(double a, double b) {
    return fabs(a - b) <= EPSILON * fmax(1.f, fmax(fabs(a), fabs(b)));
}

float remap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}