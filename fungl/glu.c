/* glu.c -- http://www.github.com/takeiteasy/fungl
 
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
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "glu.h"
#define QOI_IMPLEMENTATION
#include "deps/qoi.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_NO_GIF
#include "deps/stb_image.h"

#define DEFAULT_MAX_MATRIX_STACK 32
#ifndef MAX_MATRIX_STACK
#define MAX_MATRIX_STACK DEFAULT_MAX_MATRIX_STACK
#endif

#define DEFAULT_MAX_TEXTURE_UNITS 4
#ifndef MAX_TEXTURE_UNITS
#define MAX_TEXTURE_UNITS DEFAULT_MAX_TEXTURE_UNITS
#endif

#define DEFAULT_CULL_DISTANCE_NEAR 0.01
#ifndef CULL_DISTANCE_NEAR
    #define CULL_DISTANCE_NEAR DEFAULT_CULL_DISTANCE_NEAR
#endif

#define DEFAULT_CULL_DISTANCE_FAR 1000.0
#ifndef CULL_DISTANCE_FAR
    #define CULL_DISTANCE_FAR DEFAULT_CULL_DISTANCE_FAR
#endif

typedef struct {
    int currentMatrixMode;
    mat4 *currentMatrix;
    mat4 modelview;
    mat4 projection;
    mat4 transform;
    int transformRequired;
    mat4 stack[MAX_MATRIX_STACK];
    int stack_count;
    
    unsigned int defaultTextureId;      // Default texture used on shapes/poly drawing (required by shader)
    unsigned int activeTextureId[MAX_TEXTURE_UNITS]; // Active texture ids to be enabled on batch drawing (0 active by default)
    unsigned int defaultVShaderId;      // Default vertex shader id (used by default shader program)
    unsigned int defaultFShaderId;      // Default fragment shader id (used by default shader program)
    unsigned int defaultShaderId;       // Default shader program id, supports vertex color and diffuse texture
    int *defaultShaderLocs;             // Default shader locations pointer to be used on rendering
    unsigned int currentShaderId;       // Current shader id to be used on rendering (by default, defaultShaderId)
    int *currentShaderLocs;             // Current shader locations pointer to be used on rendering (by default, defaultShaderLocs)
    
    int currentBlendMode;               // Blending mode active
    int glBlendSrcFactor;               // Blending source factor
    int glBlendDstFactor;               // Blending destination factor
    int glBlendEquation;                // Blending equation
    int glBlendSrcFactorRGB;            // Blending source RGB factor
    int glBlendDestFactorRGB;           // Blending destination RGB factor
    int glBlendSrcFactorAlpha;          // Blending source alpha factor
    int glBlendDestFactorAlpha;         // Blending destination alpha factor
    int glBlendEquationRGB;             // Blending equation for RGB
    int glBlendEquationAlpha;           // Blending equation for alpha
    int glCustomBlendModeModified;      // Custom blending factor and equation modification status
    
    int framebufferWidth;               // Current framebuffer width
    int framebufferHeight;              // Current framebuffer height
} glu_state_t;

typedef struct {
    
} glu_draw_call_t;

static struct {
    glu_state_t default_state;
    glu_state_t *current_state;
    double cull_distance_near;
    double cull_distance_far;
} glu = {
    .cull_distance_near = CULL_DISTANCE_NEAR,
    .cull_distance_far = CULL_DISTANCE_FAR
};

void gluInit(int width, int height) {
    memset(&glu.default_state, 0, sizeof(glu_state_t));
    glu.current_state = &glu.default_state;
}

void gluMatrixMode(int mode)  {
    switch (mode){
        case GLU_PROJECTION:
            glMatrixMode(GL_PROJECTION);
            break;
        case GLU_MODELVIEW:
            glMatrixMode(GL_MODELVIEW);
            break;
        case GLU_TEXTURE:
            glMatrixMode(GL_TEXTURE);
            break;
        default:
            break; // TODO: Warning
    }
}

void gluPushMatrix(void)  {
    if (glu.current_state->stack_count >= MAX_MATRIX_STACK)
        abort(); // TODO: Matrix stack overflow
    if (glu.current_state->currentMatrixMode == GLU_MODELVIEW) {
        glu.current_state->transformRequired = 1;
        glu.current_state->currentMatrix = &glu.current_state->transform;
    }
    glu.current_state->stack[glu.current_state->stack_count] = *glu.current_state->currentMatrix;
    glu.current_state->stack_count++;
}

void gluPopMatrix(void)  {
    if (glu.current_state->stack_count > 0)
        *glu.current_state->currentMatrix = glu.current_state->stack[--glu.current_state->stack_count];
    else {
        if (glu.current_state->currentMatrixMode == GLU_MODELVIEW) {
            glu.current_state->currentMatrix = &glu.current_state->modelview;
            glu.current_state->transformRequired = 0;
        }
    }
}

void gluLoadIdentity(void)  {
    *glu.current_state->currentMatrix = mat4_identity();
}

static void mul_current_matrix(mat4 *mat) {
    *glu.current_state->currentMatrix = *mat * *glu.current_state->currentMatrix;
}

void gluTranslatef(float x, float y, float z)  {
    mat4 transform = mat4_translate(Vec3(x, y, z));
    mul_current_matrix(&transform);
}

void gluRotatef(float angle, float x, float y, float z)  {
    mat4 rotation = mat4_rotate(Vec3(x, y, z), angle);
    mul_current_matrix(&rotation);
}

void gluScalef(float x, float y, float z)  {
    mat4 scaling = mat4_scale(Vec3(x, y, z));
    mul_current_matrix(&scaling);
}

void gluMultMatrixf(const float *matf)  {
    mat4 tmp;
    memcpy((float*)&tmp, matf, 16 * sizeof(float));
    mul_current_matrix(&tmp);
}

void gluFrustum(double left, double right, double bottom, double top, double znear, double zfar)  {
    mat4 frustum = Mat4();
    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(zfar - znear);
    frustum[0][0] = ((float) znear*2.0f)/rl;
    frustum[1][1] = ((float) znear*2.0f)/tb;
    frustum[0][2] = ((float)right + (float)left)/rl;
    frustum[1][2] = ((float)top + (float)bottom)/tb;
    frustum[2][2] = -((float)zfar + (float)znear)/fn;
    frustum[3][2] = -1.0f;
    frustum[2][3] = -((float)zfar*(float)znear*2.0f)/fn;
    mul_current_matrix(&frustum);
}

void gluPerspective(float fov, float aspect_ratio, float znear, float zfar) {
    mat4 mat = perspective(fov, aspect_ratio, znear, zfar);
    mul_current_matrix(&mat);
}

void gluOrtho(double left, double right, double bottom, double top, double znear, double zfar)  {
    mat4 mat = ortho(left, right, bottom, top, znear, zfar);
    mul_current_matrix(&mat);
}

void gluViewport(int x, int y, int width, int height)  {
    glViewport(x, y, width, height);
}

void gluSetClipPlanes(double nearPlane, double farPlane)  {
    glu.cull_distance_near = nearPlane;
    glu.cull_distance_far = farPlane;
}

double gluGetCullDistanceNear(void)  {
    return glu.cull_distance_near;
}

double gluGetCullDistanceFar(void)  {
    return glu.cull_distance_far;
}

void gluBegin(int mode)  {
    // ...
}

void gluEnd(void)  {
    // ...
}

void gluVertex2i(int x, int y)  {
    // ...
}

void gluVertex2f(float x, float y)  {
    // ...
}

void gluVertex3f(float x, float y, float z)  {
    // ...
}

void gluTexCoord2f(float x, float y)  {
    // ...
}

void gluNormal3f(float x, float y, float z)  {
    // ...
}

void gluColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)  {
    // ...
}

void gluColor3f(float x, float y, float z)  {
    // ...
}

void gluColor4f(float x, float y, float z, float w)  {
    // ...
}

