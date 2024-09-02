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
#include <stdlib.h> // Required for: malloc(), free()
#include <string.h> // Required for: strcmp(), strlen() [Used in gluInit(), on extensions loading]
#include <math.h>   // Required for: sqrtf(), sinf(), cosf(), floor(), log()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef PI
#define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
#define DEG2RAD (PI / 180.0f)
#endif
#ifndef RAD2DEG
#define RAD2DEG (180.0f / PI)
#endif

#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES 0x8D64
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#endif
#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_4x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93b0
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93b7
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_PROGRAM_POINT_SIZE
#define GL_PROGRAM_POINT_SIZE 0x8642
#endif

#ifndef GL_LINE_WIDTH
#define GL_LINE_WIDTH 0x0B21
#endif

// Default shader vertex attribute names to set location points
#ifndef GLU_DEFAULT_SHADER_ATTRIB_NAME_POSITION
#define GLU_DEFAULT_SHADER_ATTRIB_NAME_POSITION "vertexPosition" // Bound by default to shader location: GLU_DEFAULT_SHADER_ATTRIB_NAME_POSITION
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD
#define GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD "vertexTexCoord" // Bound by default to shader location: GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_NAME_NORMAL
#define GLU_DEFAULT_SHADER_ATTRIB_NAME_NORMAL "vertexNormal" // Bound by default to shader location: GLU_DEFAULT_SHADER_ATTRIB_NAME_NORMAL
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_NAME_COLOR
#define GLU_DEFAULT_SHADER_ATTRIB_NAME_COLOR "vertexColor" // Bound by default to shader location: GLU_DEFAULT_SHADER_ATTRIB_NAME_COLOR
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_NAME_TANGENT
#define GLU_DEFAULT_SHADER_ATTRIB_NAME_TANGENT "vertexTangent" // Bound by default to shader location: GLU_DEFAULT_SHADER_ATTRIB_NAME_TANGENT
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2
#define GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2 "vertexTexCoord2" // Bound by default to shader location: GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2
#endif

#ifndef GLU_DEFAULT_SHADER_UNIFORM_NAME_MVP
#define GLU_DEFAULT_SHADER_UNIFORM_NAME_MVP "mvp" // model-view-projection matrix
#endif
#ifndef GLU_DEFAULT_SHADER_UNIFORM_NAME_VIEW
#define GLU_DEFAULT_SHADER_UNIFORM_NAME_VIEW "matView" // view matrix
#endif
#ifndef GLU_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION
#define GLU_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION "matProjection" // projection matrix
#endif
#ifndef GLU_DEFAULT_SHADER_UNIFORM_NAME_MODEL
#define GLU_DEFAULT_SHADER_UNIFORM_NAME_MODEL "matModel" // model matrix
#endif
#ifndef GLU_DEFAULT_SHADER_UNIFORM_NAME_NORMAL
#define GLU_DEFAULT_SHADER_UNIFORM_NAME_NORMAL "matNormal" // normal matrix (transpose(inverse(matModelView))
#endif
#ifndef GLU_DEFAULT_SHADER_UNIFORM_NAME_COLOR
#define GLU_DEFAULT_SHADER_UNIFORM_NAME_COLOR "colDiffuse" // color diffuse (base tint color, multiplied by texture color)
#endif
#ifndef GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0
#define GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0 "texture0" // texture0 (texture slot active 0)
#endif
#ifndef GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1
#define GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1 "texture1" // texture1 (texture slot active 1)
#endif
#ifndef GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2
#define GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2 "texture2" // texture2 (texture slot active 2)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct gluState {
    gluRenderBatch *currentBatch; // Current render batch
    gluRenderBatch defaultBatch;  // Default internal render batch

    int vertexCounter;                            // Current active render batch vertex counter (generic, used for all batches)
    float texcoordx, texcoordy;                   // Current active texture coordinate (added on glVertex*())
    float normalx, normaly, normalz;              // Current active normal (added on glVertex*())
    unsigned char colorr, colorg, colorb, colora; // Current active color (added on glVertex*())

    int currentMatrixMode;                  // Current matrix mode
    Matrix *currentMatrix;                  // Current matrix pointer
    Matrix modelview;                       // Default modelview matrix
    Matrix projection;                      // Default projection matrix
    Matrix transform;                       // Transform matrix to be used with gluTranslate, gluRotate, gluScale
    bool transformRequired;                 // Require transform matrix application to current draw-call vertex (if required)
    Matrix stack[GLU_MAX_MATRIX_STACK_SIZE]; // Matrix stack for push/pop
    int stackCounter;                       // Matrix stack counter

    unsigned int defaultTextureId;                                    // Default texture used on shapes/poly drawing (required by shader)
    unsigned int activeTextureId[GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS]; // Active texture ids to be enabled on batch drawing (0 active by default)
    unsigned int defaultVShaderId;                                    // Default vertex shader id (used by default shader program)
    unsigned int defaultFShaderId;                                    // Default fragment shader id (used by default shader program)
    unsigned int defaultShaderId;                                     // Default shader program id, supports vertex color and diffuse texture
    int *defaultShaderLocs;                                           // Default shader locations pointer to be used on rendering
    unsigned int currentShaderId;                                     // Current shader id to be used on rendering (by default, defaultShaderId)
    int *currentShaderLocs;                                           // Current shader locations pointer to be used on rendering (by default, defaultShaderLocs)

    bool stereoRender;          // Stereo rendering flag
    Matrix projectionStereo[2]; // VR stereo rendering eyes projection matrices
    Matrix viewOffsetStereo[2]; // VR stereo rendering eyes view offset matrices

    // Blending variables
    int currentBlendMode;           // Blending mode active
    int glBlendSrcFactor;           // Blending source factor
    int glBlendDstFactor;           // Blending destination factor
    int glBlendEquation;            // Blending equation
    int glBlendSrcFactorRGB;        // Blending source RGB factor
    int glBlendDestFactorRGB;       // Blending destination RGB factor
    int glBlendSrcFactorAlpha;      // Blending source alpha factor
    int glBlendDestFactorAlpha;     // Blending destination alpha factor
    int glBlendEquationRGB;         // Blending equation for RGB
    int glBlendEquationAlpha;       // Blending equation for alpha
    bool glCustomBlendModeModified; // Custom blending factor and equation modification status

    int framebufferWidth;  // Current framebuffer width
    int framebufferHeight; // Current framebuffer height
    
    struct {
        bool vao;            // VAO support (OpenGL ES2 could not support VAO extension) (GL_ARB_vertex_array_object)
        bool instancing;     // Instancing supported (GL_ANGLE_instanced_arrays, GL_EXT_draw_instanced + GL_EXT_instanced_arrays)
        bool texNPOT;        // NPOT textures full support (GL_ARB_texture_non_power_of_two, GL_OES_texture_npot)
        bool texDepth;       // Depth textures supported (GL_ARB_depth_texture, GL_OES_depth_texture)
        bool texDepthWebGL;  // Depth textures supported WebGL specific (GL_WEBGL_depth_texture)
        bool texFloat32;     // float textures support (32 bit per channel) (GL_OES_texture_float)
        bool texFloat16;     // half float textures support (16 bit per channel) (GL_OES_texture_half_float)
        bool texCompDXT;     // DDS texture compression support (GL_EXT_texture_compression_s3tc, GL_WEBGL_compressed_texture_s3tc, GL_WEBKIT_WEBGL_compressed_texture_s3tc)
        bool texCompETC1;    // ETC1 texture compression support (GL_OES_compressed_ETC1_RGB8_texture, GL_WEBGL_compressed_texture_etc1)
        bool texCompETC2;    // ETC2/EAC texture compression support (GL_ARB_ES3_compatibility)
        bool texCompPVRT;    // PVR texture compression support (GL_IMG_texture_compression_pvrtc)
        bool texCompASTC;    // ASTC texture compression support (GL_KHR_texture_compression_astc_hdr, GL_KHR_texture_compression_astc_ldr)
        bool texMirrorClamp; // Clamp mirror wrap mode supported (GL_EXT_texture_mirror_clamp)
        bool texAnisoFilter; // Anisotropic texture filtering support (GL_EXT_texture_filter_anisotropic)
        bool computeShader;  // Compute shaders support (GL_ARB_compute_shader)
        bool ssbo;           // Shader storage buffer object support (GL_ARB_shader_storage_buffer_object)

        float maxAnisotropyLevel; // Maximum anisotropy level supported (minimum is 2.0f)
        int maxDepthBits;         // Maximum bits for depth component

    } ExtSupported; // Extensions supported flags
} gluState;

typedef void *(*gluLoadProc)(const char *name); // OpenGL extension functions loader signature (same as GLADloadproc)

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static double gluCullDistanceNear = GLU_CULL_DISTANCE_NEAR;
static double gluCullDistanceFar = GLU_CULL_DISTANCE_FAR;
static gluState glu = {0};

static void gluLoadShaderDefault(void);   // Load default shader
static void gluUnloadShaderDefault(void); // Unload default shader
static int gluGetPixelDataSize(int width, int height, int format); // Get pixel data size in bytes (image or texture)

// Auxiliar matrix math functions
typedef struct glu_float16 {
    float v[16];
} glu_float16;
static glu_float16 gluMatrixToFloatV(Matrix mat);            // Get float array of matrix data
#define gluMatrixToFloat(mat) (gluMatrixToFloatV(mat).v)     // Get float vector for Matrix
static Matrix gluMatrixIdentity(void);                      // Get identity matrix
static Matrix gluMatrixMultiply(Matrix left, Matrix right); // Multiply two matrices
static Matrix gluMatrixTranspose(Matrix mat);               // Transposes provided matrix
static Matrix gluMatrixInvert(Matrix mat);                  // Invert provided matrix

//----------------------------------------------------------------------------------
// Module Functions Definition - Matrix operations
//----------------------------------------------------------------------------------

// Choose the current matrix to be transformed
void gluMatrixMode(int mode) {
    if (mode == GLU_PROJECTION)
        glu.currentMatrix = &glu.projection;
    else if (mode == GLU_MODELVIEW)
        glu.currentMatrix = &glu.modelview;
    // else if (mode == GLU_TEXTURE) // Not supported

    glu.currentMatrixMode = mode;
}

// Push the current matrix into glu.State.stack
void gluPushMatrix(void) {
    if (glu.stackCounter >= GLU_MAX_MATRIX_STACK_SIZE)
        TRACELOG(GLU_LOG_ERROR, "RLGL: Matrix stack overflow (GLU_MAX_MATRIX_STACK_SIZE)");

    if (glu.currentMatrixMode == GLU_MODELVIEW) {
        glu.transformRequired = true;
        glu.currentMatrix = &glu.transform;
    }

    glu.stack[glu.stackCounter] = *glu.currentMatrix;
    glu.stackCounter++;
}

// Pop lattest inserted matrix from glu.State.stack
void gluPopMatrix(void) {
    if (glu.stackCounter > 0) {
        Matrix mat = glu.stack[glu.stackCounter - 1];
        *glu.currentMatrix = mat;
        glu.stackCounter--;
    }

    if ((glu.stackCounter == 0) && (glu.currentMatrixMode == GLU_MODELVIEW)) {
        glu.currentMatrix = &glu.modelview;
        glu.transformRequired = false;
    }
}

// Reset current matrix to identity matrix
void gluLoadIdentity(void) {
    *glu.currentMatrix = gluMatrixIdentity();
}

// Multiply the current matrix by a translation matrix
void gluTranslatef(float x, float y, float z) {
    Matrix matTranslation = {
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f};

    // NOTE: We transpose matrix with multiplication order
    *glu.currentMatrix = gluMatrixMultiply(matTranslation, *glu.currentMatrix);
}

// Multiply the current matrix by a rotation matrix
// NOTE: The provided angle must be in degrees
void gluRotatef(float angle, float x, float y, float z) {
    Matrix matRotation = gluMatrixIdentity();

    // Axis vector (x, y, z) normalization
    float lengthSquared = x * x + y * y + z * z;
    if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) {
        float inverseLength = 1.0f / sqrtf(lengthSquared);
        x *= inverseLength;
        y *= inverseLength;
        z *= inverseLength;
    }

    // Rotation matrix generation
    float sinres = sinf(DEG2RAD * angle);
    float cosres = cosf(DEG2RAD * angle);
    float t = 1.0f - cosres;

    matRotation.m0 = x * x * t + cosres;
    matRotation.m1 = y * x * t + z * sinres;
    matRotation.m2 = z * x * t - y * sinres;
    matRotation.m3 = 0.0f;

    matRotation.m4 = x * y * t - z * sinres;
    matRotation.m5 = y * y * t + cosres;
    matRotation.m6 = z * y * t + x * sinres;
    matRotation.m7 = 0.0f;

    matRotation.m8 = x * z * t + y * sinres;
    matRotation.m9 = y * z * t - x * sinres;
    matRotation.m10 = z * z * t + cosres;
    matRotation.m11 = 0.0f;

    matRotation.m12 = 0.0f;
    matRotation.m13 = 0.0f;
    matRotation.m14 = 0.0f;
    matRotation.m15 = 1.0f;

    // NOTE: We transpose matrix with multiplication order
    *glu.currentMatrix = gluMatrixMultiply(matRotation, *glu.currentMatrix);
}

// Multiply the current matrix by a scaling matrix
void gluScalef(float x, float y, float z) {
    Matrix matScale = {
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    // NOTE: We transpose matrix with multiplication order
    *glu.currentMatrix = gluMatrixMultiply(matScale, *glu.currentMatrix);
}

// Multiply the current matrix by another matrix
void gluMultMatrixf(const float *matf) {
    // Matrix creation from array
    Matrix mat = {matf[0], matf[4], matf[8], matf[12],
                  matf[1], matf[5], matf[9], matf[13],
                  matf[2], matf[6], matf[10], matf[14],
                  matf[3], matf[7], matf[11], matf[15]};

    *glu.currentMatrix = gluMatrixMultiply(mat, *glu.currentMatrix);
}

// Multiply the current matrix by a perspective matrix generated by parameters
void gluFrustum(double left, double right, double bottom, double top, double znear, double zfar) {
    Matrix matFrustum = {0};

    float gu = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(zfar - znear);

    matFrustum.m0 = ((float)znear * 2.0f) / gu;
    matFrustum.m1 = 0.0f;
    matFrustum.m2 = 0.0f;
    matFrustum.m3 = 0.0f;

    matFrustum.m4 = 0.0f;
    matFrustum.m5 = ((float)znear * 2.0f) / tb;
    matFrustum.m6 = 0.0f;
    matFrustum.m7 = 0.0f;

    matFrustum.m8 = ((float)right + (float)left) / gu;
    matFrustum.m9 = ((float)top + (float)bottom) / tb;
    matFrustum.m10 = -((float)zfar + (float)znear) / fn;
    matFrustum.m11 = -1.0f;

    matFrustum.m12 = 0.0f;
    matFrustum.m13 = 0.0f;
    matFrustum.m14 = -((float)zfar * (float)znear * 2.0f) / fn;
    matFrustum.m15 = 0.0f;

    *glu.currentMatrix = gluMatrixMultiply(*glu.currentMatrix, matFrustum);
}

// Multiply the current matrix by an orthographic matrix generated by parameters
void gluOrtho(double left, double right, double bottom, double top, double znear, double zfar) {
    // NOTE: If left-right and top-botton values are equal it could create a division by zero,
    // response to it is platform/compiler dependant
    Matrix matOrtho = {0};

    float gu = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(zfar - znear);

    matOrtho.m0 = 2.0f / gu;
    matOrtho.m1 = 0.0f;
    matOrtho.m2 = 0.0f;
    matOrtho.m3 = 0.0f;
    matOrtho.m4 = 0.0f;
    matOrtho.m5 = 2.0f / tb;
    matOrtho.m6 = 0.0f;
    matOrtho.m7 = 0.0f;
    matOrtho.m8 = 0.0f;
    matOrtho.m9 = 0.0f;
    matOrtho.m10 = -2.0f / fn;
    matOrtho.m11 = 0.0f;
    matOrtho.m12 = -((float)left + (float)right) / gu;
    matOrtho.m13 = -((float)top + (float)bottom) / tb;
    matOrtho.m14 = -((float)zfar + (float)znear) / fn;
    matOrtho.m15 = 1.0f;

    *glu.currentMatrix = gluMatrixMultiply(*glu.currentMatrix, matOrtho);
}

// Set the viewport area (transformation from normalized device coordinates to window coordinates)
// NOTE: We store current viewport dimensions
void gluViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

// Set clip planes distances
void gluSetClipPlanes(double nearPlane, double farPlane) {
    gluCullDistanceNear = nearPlane;
    gluCullDistanceFar = farPlane;
}

// Get cull plane distance near
double gluGetCullDistanceNear(void) {
    return gluCullDistanceNear;
}

// Get cull plane distance far
double gluGetCullDistanceFar(void) {
    return gluCullDistanceFar;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vertex level operations
//----------------------------------------------------------------------------------
// Initialize drawing mode (how to organize vertex)
void gluBegin(int mode) {
    // Draw mode can be GLU_LINES, GLU_TRIANGLES and GLU_QUADS
    // NOTE: In all three cases, vertex are accumulated over default internal vertex buffer
    if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode != mode) {
        if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount > 0) {
            // Make sure current glu.currentBatch->draws[i].vertexCount is aligned a multiple of 4,
            // that way, following QUADS drawing will keep aligned with index processing
            // It implies adding some extra alignment vertex at the end of the draw,
            // those vertex are not processed but they are considered as an additional offset
            // for the next set of vertex to be drawn
            if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_LINES)
                glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment = ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount < 4) ? glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount : glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 4);
            else if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_TRIANGLES)
                glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment = ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount < 4) ? 1 : (4 - (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 4)));
            else
                glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment = 0;

            if (!gluCheckRenderBatchLimit(glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment)) {
                glu.vertexCounter += glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment;
                glu.currentBatch->drawCounter++;
            }
        }

        if (glu.currentBatch->drawCounter >= GLU_DEFAULT_BATCH_DRAWCALLS)
            gluDrawRenderBatch(glu.currentBatch);

        glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode = mode;
        glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount = 0;
        glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].textureId = glu.defaultTextureId;
    }
}

// Finish vertex providing
void gluEnd(void) {
    // NOTE: Depth increment is dependant on gluOrtho(): z-near and z-far values,
    // as well as depth buffer bit-depth (16bit or 24bit or 32bit)
    // Correct increment formula would be: depthInc = (zfar - znear)/pow(2, bits)
    glu.currentBatch->currentDepth += (1.0f / 20000.0f);
}

// Define one vertex (position)
// NOTE: Vertex position data is the basic information required for drawing
void gluVertex3f(float x, float y, float z) {
    float tx = x;
    float ty = y;
    float tz = z;

    // Transform provided vector if required
    if (glu.transformRequired) {
        tx = glu.transform.m0 * x + glu.transform.m4 * y + glu.transform.m8 * z + glu.transform.m12;
        ty = glu.transform.m1 * x + glu.transform.m5 * y + glu.transform.m9 * z + glu.transform.m13;
        tz = glu.transform.m2 * x + glu.transform.m6 * y + glu.transform.m10 * z + glu.transform.m14;
    }

    // WARNING: We can't break primitives when launching a new batch.
    // GLU_LINES comes in pairs, GLU_TRIANGLES come in groups of 3 vertices and GLU_QUADS come in groups of 4 vertices.
    // We must check current draw.mode when a new vertex is required and finish the batch only if the draw.mode draw.vertexCount is %2, %3 or %4
    if (glu.vertexCounter > (glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].elementCount * 4 - 4)) {
        if ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_LINES) &&
            (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 2 == 0)) {
            // Reached the maximum number of vertices for GLU_LINES drawing
            // Launch a draw call but keep current state for next vertices comming
            // NOTE: We add +1 vertex to the check for security
            gluCheckRenderBatchLimit(2 + 1);
        }
        else if ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_TRIANGLES) &&
                 (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 3 == 0)) {
            gluCheckRenderBatchLimit(3 + 1);
        }
        else if ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_QUADS) &&
                 (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 4 == 0)) {
            gluCheckRenderBatchLimit(4 + 1);
        }
    }

    // Add vertices
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].vertices[3 * glu.vertexCounter] = tx;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].vertices[3 * glu.vertexCounter + 1] = ty;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].vertices[3 * glu.vertexCounter + 2] = tz;

    // Add current texcoord
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].texcoords[2 * glu.vertexCounter] = glu.texcoordx;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].texcoords[2 * glu.vertexCounter + 1] = glu.texcoordy;

    // Add current normal
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].normals[3 * glu.vertexCounter] = glu.normalx;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].normals[3 * glu.vertexCounter + 1] = glu.normaly;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].normals[3 * glu.vertexCounter + 2] = glu.normalz;

    // Add current color
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].colors[4 * glu.vertexCounter] = glu.colorr;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].colors[4 * glu.vertexCounter + 1] = glu.colorg;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].colors[4 * glu.vertexCounter + 2] = glu.colorb;
    glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].colors[4 * glu.vertexCounter + 3] = glu.colora;

    glu.vertexCounter++;
    glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount++;
}

// Define one vertex (position)
void gluVertex2f(float x, float y) {
    gluVertex3f(x, y, glu.currentBatch->currentDepth);
}

// Define one vertex (position)
void gluVertex2i(int x, int y) {
    gluVertex3f((float)x, (float)y, glu.currentBatch->currentDepth);
}

// Define one vertex (texture coordinate)
// NOTE: Texture coordinates are limited to QUADS only
void gluTexCoord2f(float x, float y) {
    glu.texcoordx = x;
    glu.texcoordy = y;
}

// Define one vertex (normal)
// NOTE: Normals limited to TRIANGLES only?
void gluNormal3f(float x, float y, float z) {
    float normalx = x;
    float normaly = y;
    float normalz = z;
    if (glu.transformRequired) {
        normalx = glu.transform.m0 * x + glu.transform.m4 * y + glu.transform.m8 * z;
        normaly = glu.transform.m1 * x + glu.transform.m5 * y + glu.transform.m9 * z;
        normalz = glu.transform.m2 * x + glu.transform.m6 * y + glu.transform.m10 * z;
    }
    float length = sqrtf(normalx * normalx + normaly * normaly + normalz * normalz);
    if (length != 0.0f) {
        float ilength = 1.0f / length;
        normalx *= ilength;
        normaly *= ilength;
        normalz *= ilength;
    }
    glu.normalx = normalx;
    glu.normaly = normaly;
    glu.normalz = normalz;
}

// Define one vertex (color)
void gluColor4ub(unsigned char x, unsigned char y, unsigned char z, unsigned char w) {
    glu.colorr = x;
    glu.colorg = y;
    glu.colorb = z;
    glu.colora = w;
}

// Define one vertex (color)
void gluColor4f(float r, float g, float b, float a) {
    gluColor4ub((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));
}

// Define one vertex (color)
void gluColor3f(float x, float y, float z) {
    gluColor4ub((unsigned char)(x * 255), (unsigned char)(y * 255), (unsigned char)(z * 255), 255);
}

//--------------------------------------------------------------------------------------
// Module Functions Definition - OpenGL style functions (common to 1.1, 3.3+, ES2)
//--------------------------------------------------------------------------------------

// Set current texture to use
void gluSetTexture(unsigned int id) {
    if (id == 0) {
        // NOTE: If quads batch limit is reached, we force a draw call and next batch starts
        if (glu.vertexCounter >=
            glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].elementCount * 4) {
            gluDrawRenderBatch(glu.currentBatch);
        }
    }
    else {
        if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].textureId != id) {
            if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount > 0) {
                // Make sure current RLGL.currentBatch->draws[i].vertexCount is aligned a multiple of 4,
                // that way, following QUADS drawing will keep aligned with index processing
                // It implies adding some extra alignment vertex at the end of the draw,
                // those vertex are not processed but they are considered as an additional offset
                // for the next set of vertex to be drawn
                if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_LINES)
                    glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment = ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount < 4) ? glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount : glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 4);
                else if (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode == GLU_TRIANGLES)
                    glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment = ((glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount < 4) ? 1 : (4 - (glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount % 4)));
                else
                    glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment = 0;

                if (!gluCheckRenderBatchLimit(glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment)) {
                    glu.vertexCounter += glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexAlignment;

                    glu.currentBatch->drawCounter++;
                }
            }

            if (glu.currentBatch->drawCounter >= GLU_DEFAULT_BATCH_DRAWCALLS)
                gluDrawRenderBatch(glu.currentBatch);

            glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].textureId = id;
            glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].vertexCount = 0;
        }
    }
}

// Select and active a texture slot
void gluActiveTextureSlot(int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
}

// Enable texture
void gluEnableTexture(unsigned int id) {
    //    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);
}

// Disable texture
void gluDisableTexture(void) {
    //    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Enable texture cubemap
void gluEnableTextureCubemap(unsigned int id) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

// Disable texture cubemap
void gluDisableTextureCubemap(void) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// Set texture parameters (wrap mode/filter mode)
void gluTextureParameters(unsigned int id, int param, int value) {
    glBindTexture(GL_TEXTURE_2D, id);
    // Reset anisotropy filter, in case it was set
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);

    switch (param) {
    case GLU_TEXTURE_WRAP_S:
    case GLU_TEXTURE_WRAP_T: {
        if (value == GLU_TEXTURE_WRAP_MIRROR_CLAMP) {
            if (glu.ExtSupported.texMirrorClamp)
                glTexParameteri(GL_TEXTURE_2D, param, value);
            else
                TRACELOG(GLU_LOG_WARNING, "GL: Clamp mirror wrap mode not supported (GL_MIRROR_CLAMP_EXT)");
        }
        else
            glTexParameteri(GL_TEXTURE_2D, param, value);
    }
    break;
    case GLU_TEXTURE_MAG_FILTER:
    case GLU_TEXTURE_MIN_FILTER:
        glTexParameteri(GL_TEXTURE_2D, param, value);
        break;
    case GLU_TEXTURE_FILTER_ANISOTROPIC: {
        if (value <= glu.ExtSupported.maxAnisotropyLevel)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)value);
        else if (glu.ExtSupported.maxAnisotropyLevel > 0.0f) {
            TRACELOG(GLU_LOG_WARNING, "GL: Maximum anisotropic filter level supported is %iX", id, (int)RLGL.ExtSupported.maxAnisotropyLevel);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)value);
        }
        else
            TRACELOG(GLU_LOG_WARNING, "GL: Anisotropic filtering not supported");
    }
    break;
    case GLU_TEXTURE_MIPMAP_BIAS_RATIO:
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, value / 100.0f);
    default:
        break;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Set cubemap parameters (wrap mode/filter mode)
void gluCubemapParameters(unsigned int id, int param, int value) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    // Reset anisotropy filter, in case it was set
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);

    switch (param) {
    case GLU_TEXTURE_WRAP_S:
    case GLU_TEXTURE_WRAP_T: {
        if (value == GLU_TEXTURE_WRAP_MIRROR_CLAMP) {
            if (glu.ExtSupported.texMirrorClamp)
                glTexParameteri(GL_TEXTURE_CUBE_MAP, param, value);
            else
                TRACELOG(GLU_LOG_WARNING, "GL: Clamp mirror wrap mode not supported (GL_MIRROR_CLAMP_EXT)");
        }
        else
            glTexParameteri(GL_TEXTURE_CUBE_MAP, param, value);
    }
    break;
    case GLU_TEXTURE_MAG_FILTER:
    case GLU_TEXTURE_MIN_FILTER:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, param, value);
        break;
    case GLU_TEXTURE_FILTER_ANISOTROPIC: {
        if (value <= glu.ExtSupported.maxAnisotropyLevel)
            glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)value);
        else if (glu.ExtSupported.maxAnisotropyLevel > 0.0f) {
            TRACELOG(GLU_LOG_WARNING, "GL: Maximum anisotropic filter level supported is %iX", id, (int)RLGL.ExtSupported.maxAnisotropyLevel);
            glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)value);
        }
        else
            TRACELOG(GLU_LOG_WARNING, "GL: Anisotropic filtering not supported");
    }
    break;
    case GLU_TEXTURE_MIPMAP_BIAS_RATIO:
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, value / 100.0f);
    default:
        break;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// Enable shader program
void gluEnableShader(unsigned int id) {
    glUseProgram(id);
}

// Disable shader program
void gluDisableShader(void) {
    glUseProgram(0);
}

// Enable rendering to texture (fbo)
void gluEnableFramebuffer(unsigned int id) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

// return the active render texture (fbo)
unsigned int gluGetActiveFramebuffer(void) {
    GLint fboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fboId);
    return fboId;
}

// Disable rendering to texture
void gluDisableFramebuffer(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Blit active framebuffer to main framebuffer
void gluBlitFramebuffer(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, int bufferMask) {
    glBlitFramebuffer(srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, bufferMask, GL_NEAREST);
}

// Bind framebuffer object (fbo)
void gluBindFramebuffer(unsigned int target, unsigned int framebuffer) {
    glBindFramebuffer(target, framebuffer);
}

// Activate multiple draw color buffers
// NOTE: One color buffer is always active by default
void gluActiveDrawBuffers(int count) {
    // NOTE: Maximum number of draw buffers supported is implementation dependant,
    // it can be queried with glGet*() but it must be at least 8
    // GLint maxDrawBuffers = 0;
    // glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

    if (count > 0) {
        if (count > 8)
            TRACELOG(LOG_WARNING, "GL: Max color buffers limited to 8");
        else {
            unsigned int buffers[8] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,
                GL_COLOR_ATTACHMENT5,
                GL_COLOR_ATTACHMENT6,
                GL_COLOR_ATTACHMENT7};

            glDrawBuffers(count, buffers);
        }
    }
    else
        TRACELOG(LOG_WARNING, "GL: One color buffer active by default");
}

//----------------------------------------------------------------------------------
// General render state configuration
//----------------------------------------------------------------------------------

// Enable color blending
void gluEnableColorBlend(void) { glEnable(GL_BLEND); }

// Disable color blending
void gluDisableColorBlend(void) { glDisable(GL_BLEND); }

// Enable depth test
void gluEnableDepthTest(void) { glEnable(GL_DEPTH_TEST); }

// Disable depth test
void gluDisableDepthTest(void) { glDisable(GL_DEPTH_TEST); }

// Enable depth write
void gluEnableDepthMask(void) { glDepthMask(GL_TRUE); }

// Disable depth write
void gluDisableDepthMask(void) { glDepthMask(GL_FALSE); }

// Enable backface culling
void gluEnableBackfaceCulling(void) { glEnable(GL_CULL_FACE); }

// Disable backface culling
void gluDisableBackfaceCulling(void) { glDisable(GL_CULL_FACE); }

// Set color mask active for screen read/draw
void gluColorMask(bool r, bool g, bool b, bool a) { glColorMask(r, g, b, a); }

// Set face culling mode
void gluSetCullFace(int mode) {
    switch (mode) {
    case GLU_CULL_FACE_BACK:
        glCullFace(GL_BACK);
        break;
    case GLU_CULL_FACE_FRONT:
        glCullFace(GL_FRONT);
        break;
    default:
        break;
    }
}

// Enable scissor test
void gluEnableScissorTest(void) { glEnable(GL_SCISSOR_TEST); }

// Disable scissor test
void gluDisableScissorTest(void) { glDisable(GL_SCISSOR_TEST); }

// Scissor test
void gluScissor(int x, int y, int width, int height) { glScissor(x, y, width, height); }

// Enable wire mode
void gluEnableWireMode(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Enable point mode
void gluEnablePointMode(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

// Disable wire mode
void gluDisableWireMode(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Set the line drawing width
void gluSetLineWidth(float width) { glLineWidth(width); }

// Get the line drawing width
float gluGetLineWidth(void) {
    float width = 0;
    glGetFloatv(GL_LINE_WIDTH, &width);
    return width;
}

// Enable line aliasing
void gluEnableSmoothLines(void) {
    glEnable(GL_LINE_SMOOTH);
}

// Disable line aliasing
void gluDisableSmoothLines(void) {
    glDisable(GL_LINE_SMOOTH);
}

// Enable stereo rendering
void gluEnableStereoRender(void) {
    glu.stereoRender = true;
}

// Disable stereo rendering
void gluDisableStereoRender(void) {
    glu.stereoRender = false;
}

// Check if stereo render is enabled
bool gluIsStereoRenderEnabled(void) {
    return glu.stereoRender;
}

// Clear color buffer with color
void gluClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    // Color values clamp to 0.0f(0) and 1.0f(255)
    float cr = (float)r / 255;
    float cg = (float)g / 255;
    float cb = (float)b / 255;
    float ca = (float)a / 255;

    glClearColor(cr, cg, cb, ca);
}

// Clear used screen buffers (color and depth)
void gluClearScreenBuffers(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear used buffers: Color and Depth (Depth is used for 3D)
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);     // Stencil buffer not used...
}

// Check and log OpenGL error codes
void gluCheckErrors(void) {
    int check = 1;
    while (check) {
        const GLenum err = glGetError();
        switch (err) {
        case GL_NO_ERROR:
            check = 0;
            break;
        case 0x0500:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_INVALID_ENUM");
            break;
        case 0x0501:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_INVALID_VALUE");
            break;
        case 0x0502:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_INVALID_OPERATION");
            break;
        case 0x0503:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_STACK_OVERFLOW");
            break;
        case 0x0504:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_STACK_UNDERFLOW");
            break;
        case 0x0505:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_OUT_OF_MEMORY");
            break;
        case 0x0506:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: GL_INVALID_FRAMEBUFFER_OPERATION");
            break;
        default:
            TRACELOG(GLU_LOG_WARNING, "GL: Error detected: Unknown error code: %x", err);
            break;
        }
    }
}

// Set blend mode
void gluSetBlendMode(int mode) {
    if ((glu.currentBlendMode != mode) || ((mode == GLU_BLEND_CUSTOM || mode == GLU_BLEND_CUSTOM_SEPARATE) && glu.glCustomBlendModeModified)) {
        gluDrawRenderBatch(glu.currentBatch);

        switch (mode) {
        case GLU_BLEND_ALPHA:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case GLU_BLEND_ADDITIVE:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case GLU_BLEND_MULTIPLIED:
            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case GLU_BLEND_ADD_COLORS:
            glBlendFunc(GL_ONE, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case GLU_BLEND_SUBTRACT_COLORS:
            glBlendFunc(GL_ONE, GL_ONE);
            glBlendEquation(GL_FUNC_SUBTRACT);
            break;
        case GLU_BLEND_ALPHA_PREMULTIPLY:
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case GLU_BLEND_CUSTOM: {
            // NOTE: Using GL blend src/dst factors and GL equation configured with gluSetBlendFactors()
            glBlendFunc(glu.glBlendSrcFactor, glu.glBlendDstFactor);
            glBlendEquation(glu.glBlendEquation);
        }
        break;
        case GLU_BLEND_CUSTOM_SEPARATE: {
            // NOTE: Using GL blend src/dst factors and GL equation configured with gluSetBlendFactorsSeparate()
            glBlendFuncSeparate(glu.glBlendSrcFactorRGB, glu.glBlendDestFactorRGB, glu.glBlendSrcFactorAlpha, glu.glBlendDestFactorAlpha);
            glBlendEquationSeparate(glu.glBlendEquationRGB, glu.glBlendEquationAlpha);
        }
        break;
        default:
            break;
        }

        glu.currentBlendMode = mode;
        glu.glCustomBlendModeModified = false;
    }
}

// Set blending mode factor and equation
void gluSetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation) {
    if ((glu.glBlendSrcFactor != glSrcFactor) ||
        (glu.glBlendDstFactor != glDstFactor) ||
        (glu.glBlendEquation != glEquation)) {
        glu.glBlendSrcFactor = glSrcFactor;
        glu.glBlendDstFactor = glDstFactor;
        glu.glBlendEquation = glEquation;

        glu.glCustomBlendModeModified = true;
    }
}

// Set blending mode factor and equation separately for RGB and alpha
void gluSetBlendFactorsSeparate(int glSrcRGB, int glDstRGB, int glSrcAlpha, int glDstAlpha, int glEqRGB, int glEqAlpha) {
    if ((glu.glBlendSrcFactorRGB != glSrcRGB) ||
        (glu.glBlendDestFactorRGB != glDstRGB) ||
        (glu.glBlendSrcFactorAlpha != glSrcAlpha) ||
        (glu.glBlendDestFactorAlpha != glDstAlpha) ||
        (glu.glBlendEquationRGB != glEqRGB) ||
        (glu.glBlendEquationAlpha != glEqAlpha)) {
        glu.glBlendSrcFactorRGB = glSrcRGB;
        glu.glBlendDestFactorRGB = glDstRGB;
        glu.glBlendSrcFactorAlpha = glSrcAlpha;
        glu.glBlendDestFactorAlpha = glDstAlpha;
        glu.glBlendEquationRGB = glEqRGB;
        glu.glBlendEquationAlpha = glEqAlpha;

        glu.glCustomBlendModeModified = true;
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition - glu functionality
//----------------------------------------------------------------------------------

// Initialize glu: OpenGL extensions, default buffers/shaders/textures, OpenGL states
void gluInit(int width, int height) {
    // Init default white texture
    unsigned char pixels[4] = {255, 255, 255, 255}; // 1 pixel RGBA (4 bytes)
    glu.defaultTextureId = gluLoadTexture(pixels, 1, 1, GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);

    if (glu.defaultTextureId != 0)
        TRACELOG(GLU_LOG_INFO, "TEXTURE: [ID %i] Default texture loaded successfully", RLGL.State.defaultTextureId);
    else
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: Failed to load default texture");

    // Init default Shader (customized for GL 3.3 and ES2)
    // Loaded: glu.State.defaultShaderId + glu.State.defaultShaderLocs
    gluLoadShaderDefault();
    glu.currentShaderId = glu.defaultShaderId;
    glu.currentShaderLocs = glu.defaultShaderLocs;

    // Init default vertex arrays buffers
    // Simulate that the default shader has the location GLU_SHADER_LOC_VERTEX_NORMAL to bind the normal buffer for the default render batch
    glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_NORMAL] = GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL;
    glu.defaultBatch = gluLoadRenderBatch(GLU_DEFAULT_BATCH_BUFFERS, GLU_DEFAULT_BATCH_BUFFER_ELEMENTS);
    glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_NORMAL] = -1;
    glu.currentBatch = &glu.defaultBatch;

    // Init stack matrices (emulating OpenGL 1.1)
    for (int i = 0; i < GLU_MAX_MATRIX_STACK_SIZE; i++)
        glu.stack[i] = gluMatrixIdentity();

    // Init internal matrices
    glu.transform = gluMatrixIdentity();
    glu.projection = gluMatrixIdentity();
    glu.modelview = gluMatrixIdentity();
    glu.currentMatrix = &glu.modelview;

    // Initialize OpenGL default states
    //----------------------------------------------------------
    // Init state: Depth test
    glDepthFunc(GL_LEQUAL);   // Type of depth testing to apply
    glDisable(GL_DEPTH_TEST); // Disable depth testing for 2D (only used for 3D)

    // Init state: Blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Color blending function (how colors are mixed)
    glEnable(GL_BLEND);                                // Enable color blending (required to work with transparencies)

    // Init state: Culling
    // NOTE: All shapes/models triangles are drawn CCW
    glCullFace(GL_BACK);    // Cull the back face (default)
    glFrontFace(GL_CCW);    // Front face are defined counter clockwise (default)
    glEnable(GL_CULL_FACE); // Enable backface culling

    // Init state: Cubemap seamless
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Seamless cubemaps (not supported on OpenGL ES 2.0)

    // Store screen size into global variables
    glu.framebufferWidth = width;
    glu.framebufferHeight = height;

    TRACELOG(GLU_LOG_INFO, "RLGL: Default OpenGL state initialized successfully");
    //----------------------------------------------------------

    // Init state: Color/Depth buffers clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Set clear color (black)
    glClearDepth(1.0f);                                 // Set clear depth value (default)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers (depth buffer required for 3D)
}

// Vertex Buffer Object deinitialization (memory free)
void gluClose(void) {
    gluUnloadRenderBatch(glu.defaultBatch);

    gluUnloadShaderDefault(); // Unload default shader

    glDeleteTextures(1, &glu.defaultTextureId); // Unload default texture
    TRACELOG(GLU_LOG_INFO, "TEXTURE: [ID %i] Default texture unloaded successfully", RLGL.State.defaultTextureId);
}

// Load OpenGL extensions
// NOTE: External loader function must be provided
void gluLoadExtensions(void *loader) {
    // NOTE: glad is generated and contains only required OpenGL 3.3 Core extensions (and lower versions)
    //    if (gladLoadGL((GLADloadfunc)loader) == 0) TRACELOG(GLU_LOG_WARNING, "GLAD: Cannot load OpenGL extensions");
    //    else TRACELOG(GLU_LOG_INFO, "GLAD: OpenGL extensions loaded successfully");

    // Get number of supported extensions
    GLint numExt = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
    TRACELOG(GLU_LOG_INFO, "GL: Supported extensions count: %i", numExt);

    // Register supported extensions flags
    // OpenGL 3.3 extensions supported by default (core)
    glu.ExtSupported.vao = true;
    glu.ExtSupported.instancing = true;
    glu.ExtSupported.texNPOT = true;
    glu.ExtSupported.texFloat32 = true;
    glu.ExtSupported.texFloat16 = true;
    glu.ExtSupported.texDepth = true;
    glu.ExtSupported.maxDepthBits = 32;
    glu.ExtSupported.texAnisoFilter = true;
    glu.ExtSupported.texMirrorClamp = true;

    // Optional OpenGL 3.3 extensions
    //    glu.ExtSupported.texCompASTC = GLAD_GL_KHR_texture_compression_astc_hdr && GLAD_GL_KHR_texture_compression_astc_ldr;
    //    glu.ExtSupported.texCompDXT = GLAD_GL_EXT_texture_compression_s3tc;  // Texture compression: DXT
    //    glu.ExtSupported.texCompETC2 = GLAD_GL_ARB_ES3_compatibility;        // Texture compression: ETC2/EAC

    // Check OpenGL information and capabilities
    //------------------------------------------------------------------------------
    // Show current OpenGL and GLSL version
    TRACELOG(GLU_LOG_INFO, "GL: OpenGL device information:");
    TRACELOG(GLU_LOG_INFO, "    > Vendor:   %s", glGetString(GL_VENDOR));
    TRACELOG(GLU_LOG_INFO, "    > Renderer: %s", glGetString(GL_RENDERER));
    TRACELOG(GLU_LOG_INFO, "    > Version:  %s", glGetString(GL_VERSION));
    TRACELOG(GLU_LOG_INFO, "    > GLSL:     %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

// NOTE: Anisotropy levels capability is an extension
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glu.ExtSupported.maxAnisotropyLevel);
}

// Get current OpenGL version
int gluGetVersion(void) {
    int glVersion = 0;
#if defined(GRAPHICS_API_OPENGL_11)
    glVersion = GLU_OPENGL_11;
#endif
#if defined(GRAPHICS_API_OPENGL_21)
    glVersion = GLU_OPENGL_21;
#elif defined(GRAPHICS_API_OPENGL_43)
    glVersion = GLU_OPENGL_43;
#elif defined(GRAPHICS_API_OPENGL_33)
    glVersion = GLU_OPENGL_33;
#endif
#if defined(GRAPHICS_API_OPENGL_ES3)
    glVersion = GLU_OPENGL_ES_30;
#elif defined(GRAPHICS_API_OPENGL_ES2)
    glVersion = GLU_OPENGL_ES_20;
#endif

    return glVersion;
}

// Set current framebuffer width
void gluSetFramebufferWidth(int width) {
    glu.framebufferWidth = width;
}

// Set current framebuffer height
void gluSetFramebufferHeight(int height) {
    glu.framebufferHeight = height;
}

// Get default framebuffer width
int gluGetFramebufferWidth(void) {
    int width = 0;
    width = glu.framebufferWidth;
    return width;
}

// Get default framebuffer height
int gluGetFramebufferHeight(void) {
    int height = 0;
    height = glu.framebufferHeight;
    return height;
}

// Get default internal texture (white texture)
// NOTE: Default texture is a 1x1 pixel UNCOMPRESSED_R8G8B8A8
unsigned int gluGetTextureIdDefault(void) {
    unsigned int id = 0;
    id = glu.defaultTextureId;
    return id;
}

// Get default shader id
unsigned int gluGetShaderIdDefault(void) {
    unsigned int id = 0;
    id = glu.defaultShaderId;
    return id;
}

// Get default shader locs
int *gluGetShaderLocsDefault(void) {
    int *locs = NULL;
    locs = glu.defaultShaderLocs;
    return locs;
}

// Render batch management
//------------------------------------------------------------------------------------------------
// Load render batch
gluRenderBatch gluLoadRenderBatch(int numBuffers, int bufferElements) {
    gluRenderBatch batch = {0};

    // Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes)
    //--------------------------------------------------------------------------------------------
    batch.vertexBuffer = (gluVertexBuffer *)GLU_MALLOC(numBuffers * sizeof(gluVertexBuffer));

    for (int i = 0; i < numBuffers; i++) {
        batch.vertexBuffer[i].elementCount = bufferElements;

        batch.vertexBuffer[i].vertices = (float *)GLU_MALLOC(bufferElements * 3 * 4 * sizeof(float));               // 3 float by vertex, 4 vertex by quad
        batch.vertexBuffer[i].texcoords = (float *)GLU_MALLOC(bufferElements * 2 * 4 * sizeof(float));              // 2 float by texcoord, 4 texcoord by quad
        batch.vertexBuffer[i].normals = (float *)GLU_MALLOC(bufferElements * 3 * 4 * sizeof(float));                // 3 float by vertex, 4 vertex by quad
        batch.vertexBuffer[i].colors = (unsigned char *)GLU_MALLOC(bufferElements * 4 * 4 * sizeof(unsigned char)); // 4 float by color, 4 colors by quad
        batch.vertexBuffer[i].indices = (unsigned int *)GLU_MALLOC(bufferElements * 6 * sizeof(unsigned int));      // 6 int by quad (indices)

        for (int j = 0; j < (3 * 4 * bufferElements); j++)
            batch.vertexBuffer[i].vertices[j] = 0.0f;
        for (int j = 0; j < (2 * 4 * bufferElements); j++)
            batch.vertexBuffer[i].texcoords[j] = 0.0f;
        for (int j = 0; j < (3 * 4 * bufferElements); j++)
            batch.vertexBuffer[i].normals[j] = 0.0f;
        for (int j = 0; j < (4 * 4 * bufferElements); j++)
            batch.vertexBuffer[i].colors[j] = 0;

        int k = 0;

        // Indices can be initialized right now
        for (int j = 0; j < (6 * bufferElements); j += 6) {
            batch.vertexBuffer[i].indices[j] = 4 * k;
            batch.vertexBuffer[i].indices[j + 1] = 4 * k + 1;
            batch.vertexBuffer[i].indices[j + 2] = 4 * k + 2;
            batch.vertexBuffer[i].indices[j + 3] = 4 * k;
            batch.vertexBuffer[i].indices[j + 4] = 4 * k + 2;
            batch.vertexBuffer[i].indices[j + 5] = 4 * k + 3;

            k++;
        }

        glu.vertexCounter = 0;
    }

    TRACELOG(GLU_LOG_INFO, "RLGL: Render batch vertex buffers loaded successfully in RAM (CPU)");
    //--------------------------------------------------------------------------------------------

    // Upload to GPU (VRAM) vertex data and initialize VAOs/VBOs
    //--------------------------------------------------------------------------------------------
    for (int i = 0; i < numBuffers; i++) {
        if (glu.ExtSupported.vao) {
            // Initialize Quads VAO
            glGenVertexArrays(1, &batch.vertexBuffer[i].vaoId);
            glBindVertexArray(batch.vertexBuffer[i].vaoId);
        }

        // Quads - Vertex buffers binding and attributes enable
        // Vertex position buffer (shader-location = 0)
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[0]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[0]);
        glBufferData(GL_ARRAY_BUFFER, bufferElements * 3 * 4 * sizeof(float), batch.vertexBuffer[i].vertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_POSITION]);
        glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_POSITION], 3, GL_FLOAT, 0, 0, 0);

        // Vertex texcoord buffer (shader-location = 1)
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[1]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[1]);
        glBufferData(GL_ARRAY_BUFFER, bufferElements * 2 * 4 * sizeof(float), batch.vertexBuffer[i].texcoords, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_TEXCOORD01]);
        glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_TEXCOORD01], 2, GL_FLOAT, 0, 0, 0);

        // Vertex normal buffer (shader-location = 2)
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[2]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[2]);
        glBufferData(GL_ARRAY_BUFFER, bufferElements * 3 * 4 * sizeof(float), batch.vertexBuffer[i].normals, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_NORMAL]);
        glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_NORMAL], 3, GL_FLOAT, 0, 0, 0);

        // Vertex color buffer (shader-location = 3)
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[3]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[3]);
        glBufferData(GL_ARRAY_BUFFER, bufferElements * 4 * 4 * sizeof(unsigned char), batch.vertexBuffer[i].colors, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_COLOR]);
        glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

        // Fill index buffer
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[4]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[4]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferElements * 6 * sizeof(int), batch.vertexBuffer[i].indices, GL_STATIC_DRAW);
    }

    TRACELOG(GLU_LOG_INFO, "RLGL: Render batch vertex buffers loaded successfully in VRAM (GPU)");

    // Unbind the current VAO
    if (glu.ExtSupported.vao)
        glBindVertexArray(0);
    //--------------------------------------------------------------------------------------------

    // Init draw calls tracking system
    //--------------------------------------------------------------------------------------------
    batch.draws = (gluDrawCall *)GLU_MALLOC(GLU_DEFAULT_BATCH_DRAWCALLS * sizeof(gluDrawCall));

    for (int i = 0; i < GLU_DEFAULT_BATCH_DRAWCALLS; i++) {
        batch.draws[i].mode = GLU_QUADS;
        batch.draws[i].vertexCount = 0;
        batch.draws[i].vertexAlignment = 0;
        // batch.draws[i].vaoId = 0;
        // batch.draws[i].shaderId = 0;
        batch.draws[i].textureId = glu.defaultTextureId;
        // batch.draws[i].glu.State.projection = gluMatrixIdentity();
        // batch.draws[i].glu.State.modelview = gluMatrixIdentity();
    }

    batch.bufferCount = numBuffers; // Record buffer count
    batch.drawCounter = 1;          // Reset draws counter
    batch.currentDepth = -1.0f;     // Reset depth value
    //--------------------------------------------------------------------------------------------

    return batch;
}

// Unload default internal buffers vertex data from CPU and GPU
void gluUnloadRenderBatch(gluRenderBatch batch) {
    // Unbind everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unload all vertex buffers data
    for (int i = 0; i < batch.bufferCount; i++) {
        // Unbind VAO attribs data
        if (glu.ExtSupported.vao) {
            glBindVertexArray(batch.vertexBuffer[i].vaoId);
            glDisableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION);
            glDisableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD);
            glDisableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL);
            glDisableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR);
            glBindVertexArray(0);
        }

        // Delete VBOs from GPU (VRAM)
        glDeleteBuffers(1, &batch.vertexBuffer[i].vboId[0]);
        glDeleteBuffers(1, &batch.vertexBuffer[i].vboId[1]);
        glDeleteBuffers(1, &batch.vertexBuffer[i].vboId[2]);
        glDeleteBuffers(1, &batch.vertexBuffer[i].vboId[3]);
        glDeleteBuffers(1, &batch.vertexBuffer[i].vboId[4]);

        // Delete VAOs from GPU (VRAM)
        if (glu.ExtSupported.vao)
            glDeleteVertexArrays(1, &batch.vertexBuffer[i].vaoId);

        // Free vertex arrays memory from CPU (RAM)
        GLU_FREE(batch.vertexBuffer[i].vertices);
        GLU_FREE(batch.vertexBuffer[i].texcoords);
        GLU_FREE(batch.vertexBuffer[i].normals);
        GLU_FREE(batch.vertexBuffer[i].colors);
        GLU_FREE(batch.vertexBuffer[i].indices);
    }

    // Unload arrays
    GLU_FREE(batch.vertexBuffer);
    GLU_FREE(batch.draws);
}

// Draw render batch
// NOTE: We require a pointer to reset batch and increase current buffer (multi-buffer)
void gluDrawRenderBatch(gluRenderBatch *batch) {
    // Update batch vertex buffers
    //------------------------------------------------------------------------------------------------------------
    // NOTE: If there is not vertex data, buffers doesn't need to be updated (vertexCount > 0)
    // TODO: If no data changed on the CPU arrays --> No need to re-update GPU arrays (use a change detector flag?)
    if (glu.vertexCounter > 0) {
        // Activate elements VAO
        if (glu.ExtSupported.vao)
            glBindVertexArray(batch->vertexBuffer[batch->currentBuffer].vaoId);

        // Vertex positions buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, glu.vertexCounter * 3 * sizeof(float), batch->vertexBuffer[batch->currentBuffer].vertices);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*4*batch->vertexBuffer[batch->currentBuffer].elementCount, batch->vertexBuffer[batch->currentBuffer].vertices, GL_DYNAMIC_DRAW);  // Update all buffer

        // Texture coordinates buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, glu.vertexCounter * 2 * sizeof(float), batch->vertexBuffer[batch->currentBuffer].texcoords);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*4*batch->vertexBuffer[batch->currentBuffer].elementCount, batch->vertexBuffer[batch->currentBuffer].texcoords, GL_DYNAMIC_DRAW); // Update all buffer

        // Normals buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, glu.vertexCounter * 3 * sizeof(float), batch->vertexBuffer[batch->currentBuffer].normals);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*4*batch->vertexBuffer[batch->currentBuffer].elementCount, batch->vertexBuffer[batch->currentBuffer].normals, GL_DYNAMIC_DRAW); // Update all buffer

        // Colors buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[3]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, glu.vertexCounter * 4 * sizeof(unsigned char), batch->vertexBuffer[batch->currentBuffer].colors);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4*batch->vertexBuffer[batch->currentBuffer].elementCount, batch->vertexBuffer[batch->currentBuffer].colors, GL_DYNAMIC_DRAW);    // Update all buffer

        // NOTE: glMapBuffer() causes sync issue.
        // If GPU is working with this buffer, glMapBuffer() will wait(stall) until GPU to finish its job.
        // To avoid waiting (idle), you can call first glBufferData() with NULL pointer before glMapBuffer().
        // If you do that, the previous data in PBO will be discarded and glMapBuffer() returns a new
        // allocated pointer immediately even if GPU is still working with the previous data.

        // Another option: map the buffer object into client's memory
        // Probably this code could be moved somewhere else...
        // batch->vertexBuffer[batch->currentBuffer].vertices = (float *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
        // if (batch->vertexBuffer[batch->currentBuffer].vertices)
        // {
        // Update vertex data
        // }
        // glUnmapBuffer(GL_ARRAY_BUFFER);

        // Unbind the current VAO
        if (glu.ExtSupported.vao)
            glBindVertexArray(0);
    }
    //------------------------------------------------------------------------------------------------------------

    // Draw batch vertex buffers (considering VR stereo if required)
    //------------------------------------------------------------------------------------------------------------
    Matrix matProjection = glu.projection;
    Matrix matModelView = glu.modelview;

    int eyeCount = 1;
    if (glu.stereoRender)
        eyeCount = 2;

    for (int eye = 0; eye < eyeCount; eye++) {
        if (eyeCount == 2) {
            // Setup current eye viewport (half screen width)
            gluViewport(eye * glu.framebufferWidth / 2, 0, glu.framebufferWidth / 2, glu.framebufferHeight);

            // Set current eye view offset to modelview matrix
            gluSetMatrixModelview(gluMatrixMultiply(matModelView, glu.viewOffsetStereo[eye]));
            // Set current eye projection matrix
            gluSetMatrixProjection(glu.projectionStereo[eye]);
        }

        // Draw buffers
        if (glu.vertexCounter > 0) {
            // Set current shader and upload current MVP matrix
            glUseProgram(glu.currentShaderId);

            // Create modelview-projection matrix and upload to shader
            Matrix matMVP = gluMatrixMultiply(glu.modelview, glu.projection);
            glUniformMatrix4fv(glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_MVP], 1, false, gluMatrixToFloat(matMVP));

            if (glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_PROJECTION] != -1) {
                glUniformMatrix4fv(glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_PROJECTION], 1, false, gluMatrixToFloat(glu.projection));
            }

            // WARNING: For the following setup of the view, model, and normal matrices, it is expected that
            // transformations and rendering occur between gluPushMatrix and gluPopMatrix.

            if (glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_VIEW] != -1) {
                glUniformMatrix4fv(glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_VIEW], 1, false, gluMatrixToFloat(glu.modelview));
            }

            if (glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_MODEL] != -1) {
                glUniformMatrix4fv(glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_MODEL], 1, false, gluMatrixToFloat(glu.transform));
            }

            if (glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_NORMAL] != -1) {
                glUniformMatrix4fv(glu.currentShaderLocs[GLU_SHADER_LOC_MATRIX_NORMAL], 1, false, gluMatrixToFloat(gluMatrixTranspose(gluMatrixInvert(glu.transform))));
            }

            if (glu.ExtSupported.vao)
                glBindVertexArray(batch->vertexBuffer[batch->currentBuffer].vaoId);
            else {
                // Bind vertex attrib: position (shader-location = 0)
                glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[0]);
                glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_POSITION], 3, GL_FLOAT, 0, 0, 0);
                glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_POSITION]);

                // Bind vertex attrib: texcoord (shader-location = 1)
                glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[1]);
                glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_TEXCOORD01], 2, GL_FLOAT, 0, 0, 0);
                glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_TEXCOORD01]);

                // Bind vertex attrib: normal (shader-location = 2)
                glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[2]);
                glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_NORMAL], 3, GL_FLOAT, 0, 0, 0);
                glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_NORMAL]);

                // Bind vertex attrib: color (shader-location = 3)
                glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[3]);
                glVertexAttribPointer(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
                glEnableVertexAttribArray(glu.currentShaderLocs[GLU_SHADER_LOC_VERTEX_COLOR]);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[4]);
            }

            // Setup some default shader values
            glUniform4f(glu.currentShaderLocs[GLU_SHADER_LOC_COLOR_DIFFUSE], 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1i(glu.currentShaderLocs[GLU_SHADER_LOC_MAP_DIFFUSE], 0); // Active default sampler2D: texture0

            // Activate additional sampler textures
            // Those additional textures will be common for all draw calls of the batch
            for (int i = 0; i < GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++) {
                if (glu.activeTextureId[i] > 0) {
                    glActiveTexture(GL_TEXTURE0 + 1 + i);
                    glBindTexture(GL_TEXTURE_2D, glu.activeTextureId[i]);
                }
            }

            // Activate default sampler2D texture0 (one texture is always active for default batch shader)
            // NOTE: Batch system accumulates calls by texture0 changes, additional textures are enabled for all the draw calls
            glActiveTexture(GL_TEXTURE0);

            for (int i = 0, vertexOffset = 0; i < batch->drawCounter; i++) {
                // Bind current draw call texture, activated as GL_TEXTURE0 and Bound to sampler2D texture0 by default
                glBindTexture(GL_TEXTURE_2D, batch->draws[i].textureId);

                if ((batch->draws[i].mode == GLU_LINES) || (batch->draws[i].mode == GLU_TRIANGLES))
                    glDrawArrays(batch->draws[i].mode, vertexOffset, batch->draws[i].vertexCount);
                else {
                    // We need to define the number of indices to be processed: elementCount*6
                    // NOTE: The final parameter tells the GPU the offset in bytes from the
                    // start of the index buffer to the location of the first index to process
                    glDrawElements(GL_TRIANGLES, batch->draws[i].vertexCount / 4 * 6, GL_UNSIGNED_INT, (GLvoid *)(vertexOffset / 4 * 6 * sizeof(GLuint)));
                }

                vertexOffset += (batch->draws[i].vertexCount + batch->draws[i].vertexAlignment);
            }

            if (!glu.ExtSupported.vao) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            glBindTexture(GL_TEXTURE_2D, 0); // Unbind textures
        }

        if (glu.ExtSupported.vao)
            glBindVertexArray(0); // Unbind VAO

        glUseProgram(0); // Unbind shader program
    }

    // Restore viewport to default measures
    if (eyeCount == 2)
        gluViewport(0, 0, glu.framebufferWidth, glu.framebufferHeight);
    //------------------------------------------------------------------------------------------------------------

    // Reset batch buffers
    //------------------------------------------------------------------------------------------------------------
    // Reset vertex counter for next frame
    glu.vertexCounter = 0;

    // Reset depth for next draw
    batch->currentDepth = -1.0f;

    // Restore projection/modelview matrices
    glu.projection = matProjection;
    glu.modelview = matModelView;

    // Reset RLGL.currentBatch->draws array
    for (int i = 0; i < GLU_DEFAULT_BATCH_DRAWCALLS; i++) {
        batch->draws[i].mode = GLU_QUADS;
        batch->draws[i].vertexCount = 0;
        batch->draws[i].textureId = glu.defaultTextureId;
    }

    // Reset active texture units for next batch
    for (int i = 0; i < GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++)
        glu.activeTextureId[i] = 0;

    // Reset draws counter to one draw for the batch
    batch->drawCounter = 1;
    //------------------------------------------------------------------------------------------------------------

    // Change to next buffer in the list (in case of multi-buffering)
    batch->currentBuffer++;
    if (batch->currentBuffer >= batch->bufferCount)
        batch->currentBuffer = 0;
}

// Set the active render batch for glu
void gluSetRenderBatchActive(gluRenderBatch *batch) {
    gluDrawRenderBatch(glu.currentBatch);

    if (batch != NULL)
        glu.currentBatch = batch;
    else
        glu.currentBatch = &glu.defaultBatch;
}

// Update and draw internal render batch
void gluDrawRenderBatchActive(void) {
    gluDrawRenderBatch(glu.currentBatch); // NOTE: Stereo rendering is checked inside
}

// Check internal buffer overflow for a given number of vertex
// and force a gluRenderBatch draw call if required
bool gluCheckRenderBatchLimit(int vCount) {
    bool overflow = false;

    if ((glu.vertexCounter + vCount) >=
        (glu.currentBatch->vertexBuffer[glu.currentBatch->currentBuffer].elementCount * 4)) {
        overflow = true;

        // Store current primitive drawing mode and texture id
        int currentMode = glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode;
        int currentTexture = glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].textureId;

        gluDrawRenderBatch(glu.currentBatch); // NOTE: Stereo rendering is checked inside

        // Restore state of last batch so we can continue adding vertices
        glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].mode = currentMode;
        glu.currentBatch->draws[glu.currentBatch->drawCounter - 1].textureId = currentTexture;
    }

    return overflow;
}

// Textures data management
//-----------------------------------------------------------------------------------------
// Convert image data to OpenGL texture (returns OpenGL valid Id)
unsigned int gluLoadTexture(const void *data, int width, int height, int format, int mipmapCount) {
    unsigned int id = 0;

    glBindTexture(GL_TEXTURE_2D, 0); // Free any old binding

    // Check texture format support by OpenGL 1.1 (compressed textures not supported)
    if ((!glu.ExtSupported.texCompDXT) && ((format == GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB) || (format == GLU_PIXELFORMAT_COMPRESSED_DXT1_RGBA) ||
                                            (format == GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA) || (format == GLU_PIXELFORMAT_COMPRESSED_DXT5_RGBA))) {
        TRACELOG(GLU_LOG_WARNING, "GL: DXT compressed texture format not supported");
        return id;
    }
    if ((!glu.ExtSupported.texCompETC1) && (format == GLU_PIXELFORMAT_COMPRESSED_ETC1_RGB)) {
        TRACELOG(GLU_LOG_WARNING, "GL: ETC1 compressed texture format not supported");
        return id;
    }

    if ((!glu.ExtSupported.texCompETC2) && ((format == GLU_PIXELFORMAT_COMPRESSED_ETC2_RGB) || (format == GLU_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA))) {
        TRACELOG(GLU_LOG_WARNING, "GL: ETC2 compressed texture format not supported");
        return id;
    }

    if ((!glu.ExtSupported.texCompPVRT) && ((format == GLU_PIXELFORMAT_COMPRESSED_PVRT_RGB) || (format == GLU_PIXELFORMAT_COMPRESSED_PVRT_RGBA))) {
        TRACELOG(GLU_LOG_WARNING, "GL: PVRT compressed texture format not supported");
        return id;
    }

    if ((!glu.ExtSupported.texCompASTC) && ((format == GLU_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA) || (format == GLU_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA))) {
        TRACELOG(GLU_LOG_WARNING, "GL: ASTC compressed texture format not supported");
        return id;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &id); // Generate texture id

    glBindTexture(GL_TEXTURE_2D, id);

    int mipWidth = width;
    int mipHeight = height;
    int mipOffset = 0; // Mipmap data offset, only used for tracelog

    // NOTE: Added pointer math separately from function to avoid UBSAN complaining
    unsigned char *dataPtr = NULL;
    if (data != NULL)
        dataPtr = (unsigned char *)data;

    // Load the different mipmap levels
    for (int i = 0; i < mipmapCount; i++) {
        unsigned int mipSize = gluGetPixelDataSize(mipWidth, mipHeight, format);

        unsigned int glInternalFormat, glFormat, glType;
        gluGetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);

        TRACELOGD("TEXTURE: Load mipmap level %i (%i x %i), size: %i, offset: %i", i, mipWidth, mipHeight, mipSize, mipOffset);

        if (glInternalFormat != 0) {
            if (format < GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB)
                glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, dataPtr);
            else
                glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, mipSize, dataPtr);

            if (format == GLU_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE) {
                GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
            else if (format == GLU_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA) {
                GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
        }

        mipWidth /= 2;
        mipHeight /= 2;
        mipOffset += mipSize; // Increment offset position to next mipmap
        if (data != NULL)
            dataPtr += mipSize; // Increment data pointer to next mipmap

        // Security check for NPOT textures
        if (mipWidth < 1)
            mipWidth = 1;
        if (mipHeight < 1)
            mipHeight = 1;
    }

    // Texture parameters configuration
    // NOTE: glTexParameteri does NOT affect texture uploading, just the way it's used
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture to repeat on x-axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Set texture to repeat on y-axis

    // Magnification and minification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Alternative: GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Alternative: GL_LINEAR

    if (mipmapCount > 1) {
        // Activate Trilinear filtering if mipmaps are available
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    // At this point we have the texture loaded in GPU and texture parameters configured

    // NOTE: If mipmaps were not in data, they are not generated automatically

    // Unbind current texture
    glBindTexture(GL_TEXTURE_2D, 0);

    if (id > 0)
        TRACELOG(GLU_LOG_INFO, "TEXTURE: [ID %i] Texture loaded successfully (%ix%i | %s | %i mipmaps)", id, width, height, gluGetPixelFormatName(format), mipmapCount);
    else
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: Failed to load texture");

    return id;
}

// Load depth texture/renderbuffer (to be attached to fbo)
// WARNING: OpenGL ES 2.0 requires GL_OES_depth_texture and WebGL requires WEBGL_depth_texture extensions
unsigned int gluLoadTextureDepth(int width, int height, bool useRenderBuffer) {
    unsigned int id = 0;

    // In case depth textures not supported, we force renderbuffer usage
    if (!glu.ExtSupported.texDepth)
        useRenderBuffer = true;

    // NOTE: We let the implementation to choose the best bit-depth
    // Possible formats: GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32 and GL_DEPTH_COMPONENT32F
    unsigned int glInternalFormat = GL_DEPTH_COMPONENT;
    if (!useRenderBuffer && glu.ExtSupported.texDepth) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

        TRACELOG(GLU_LOG_INFO, "TEXTURE: Depth texture loaded successfully");
    }
    else {
        // Create the renderbuffer that will serve as the depth attachment for the framebuffer
        // NOTE: A renderbuffer is simpler than a texture and could offer better performance on embedded devices
        glGenRenderbuffers(1, &id);
        glBindRenderbuffer(GL_RENDERBUFFER, id);
        glRenderbufferStorage(GL_RENDERBUFFER, glInternalFormat, width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        TRACELOG(GLU_LOG_INFO, "TEXTURE: [ID %i] Depth renderbuffer loaded successfully (%i bits)", id, (RLGL.ExtSupported.maxDepthBits >= 24) ? RLGL.ExtSupported.maxDepthBits : 16);
    }
    return id;
}

// Load texture cubemap
// NOTE: Cubemap data is expected to be 6 images in a single data array (one after the other),
// expected the following convention: +X, -X, +Y, -Y, +Z, -Z
unsigned int gluLoadTextureCubemap(const void *data, int size, int format) {
    unsigned int id = 0;

    unsigned int dataSize = gluGetPixelDataSize(size, size, format);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    unsigned int glInternalFormat, glFormat, glType;
    gluGetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);

    if (glInternalFormat != 0) {
        // Load cubemap faces
        for (unsigned int i = 0; i < 6; i++) {
            if (data == NULL) {
                if (format < GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB) {
                    if ((format == GLU_PIXELFORMAT_UNCOMPRESSED_R32) || (format == GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32) || (format == GLU_PIXELFORMAT_UNCOMPRESSED_R16) || (format == GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16))
                        TRACELOG(GLU_LOG_WARNING, "TEXTURES: Cubemap requested format not supported");
                    else
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, size, size, 0, glFormat, glType, NULL);
                }
                else
                    TRACELOG(GLU_LOG_WARNING, "TEXTURES: Empty cubemap creation does not support compressed format");
            }
            else {
                if (format < GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, size, size, 0, glFormat, glType, (unsigned char *)data + i * dataSize);
                else
                    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, size, size, 0, dataSize, (unsigned char *)data + i * dataSize);
            }

            if (format == GLU_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE) {
                GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
                glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
            else if (format == GLU_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA) {
                GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
                glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
        }
    }

    // Set cubemap texture sampling parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Flag not supported on OpenGL ES 2.0

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (id > 0)
        TRACELOG(GLU_LOG_INFO, "TEXTURE: [ID %i] Cubemap texture loaded successfully (%ix%i)", id, size, size);
    else
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: Failed to load cubemap texture");

    return id;
}

// Update already loaded texture in GPU with new data
// NOTE: We don't know safely if internal texture format is the expected one...
void gluUpdateTexture(unsigned int id, int offsetX, int offsetY, int width, int height, int format, const void *data) {
    glBindTexture(GL_TEXTURE_2D, id);

    unsigned int glInternalFormat, glFormat, glType;
    gluGetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);

    if ((glInternalFormat != 0) && (format < GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB)) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, glFormat, glType, data);
    }
    else
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: [ID %i] Failed to update for current texture format (%i)", id, format);
}

// Get OpenGL internal formats and data type from raylib PixelFormat
void gluGetGlTextureFormats(int format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType) {
    *glInternalFormat = 0;
    *glFormat = 0;
    *glType = 0;

    switch (format) {
    case GLU_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
        *glInternalFormat = GL_R8;
        *glFormat = GL_RED;
        *glType = GL_UNSIGNED_BYTE;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
        *glInternalFormat = GL_RG8;
        *glFormat = GL_RG;
        *glType = GL_UNSIGNED_BYTE;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R5G6B5:
        *glInternalFormat = GL_RGB565;
        *glFormat = GL_RGB;
        *glType = GL_UNSIGNED_SHORT_5_6_5;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8:
        *glInternalFormat = GL_RGB8;
        *glFormat = GL_RGB;
        *glType = GL_UNSIGNED_BYTE;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
        *glInternalFormat = GL_RGB5_A1;
        *glFormat = GL_RGBA;
        *glType = GL_UNSIGNED_SHORT_5_5_5_1;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
        *glInternalFormat = GL_RGBA4;
        *glFormat = GL_RGBA;
        *glType = GL_UNSIGNED_SHORT_4_4_4_4;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
        *glInternalFormat = GL_RGBA8;
        *glFormat = GL_RGBA;
        *glType = GL_UNSIGNED_BYTE;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32:
        if (glu.ExtSupported.texFloat32)
            *glInternalFormat = GL_R32F;
        *glFormat = GL_RED;
        *glType = GL_FLOAT;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32:
        if (glu.ExtSupported.texFloat32)
            *glInternalFormat = GL_RGB32F;
        *glFormat = GL_RGB;
        *glType = GL_FLOAT;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:
        if (glu.ExtSupported.texFloat32)
            *glInternalFormat = GL_RGBA32F;
        *glFormat = GL_RGBA;
        *glType = GL_FLOAT;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16:
        if (glu.ExtSupported.texFloat16)
            *glInternalFormat = GL_R16F;
        *glFormat = GL_RED;
        *glType = GL_HALF_FLOAT;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16:
        if (glu.ExtSupported.texFloat16)
            *glInternalFormat = GL_RGB16F;
        *glFormat = GL_RGB;
        *glType = GL_HALF_FLOAT;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16:
        if (glu.ExtSupported.texFloat16)
            *glInternalFormat = GL_RGBA16F;
        *glFormat = GL_RGBA;
        *glType = GL_HALF_FLOAT;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB:
        if (glu.ExtSupported.texCompDXT)
            *glInternalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_DXT1_RGBA:
        if (glu.ExtSupported.texCompDXT)
            *glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA:
        if (glu.ExtSupported.texCompDXT)
            *glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_DXT5_RGBA:
        if (glu.ExtSupported.texCompDXT)
            *glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_ETC1_RGB:
        if (glu.ExtSupported.texCompETC1)
            *glInternalFormat = GL_ETC1_RGB8_OES;
        break; // NOTE: Requires OpenGL ES 2.0 or OpenGL 4.3
    case GLU_PIXELFORMAT_COMPRESSED_ETC2_RGB:
        if (glu.ExtSupported.texCompETC2)
            *glInternalFormat = GL_COMPRESSED_RGB8_ETC2;
        break; // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
    case GLU_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA:
        if (glu.ExtSupported.texCompETC2)
            *glInternalFormat = GL_COMPRESSED_RGBA8_ETC2_EAC;
        break; // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
    case GLU_PIXELFORMAT_COMPRESSED_PVRT_RGB:
        if (glu.ExtSupported.texCompPVRT)
            *glInternalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
        break; // NOTE: Requires PowerVR GPU
    case GLU_PIXELFORMAT_COMPRESSED_PVRT_RGBA:
        if (glu.ExtSupported.texCompPVRT)
            *glInternalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
        break; // NOTE: Requires PowerVR GPU
    case GLU_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA:
        if (glu.ExtSupported.texCompASTC)
            *glInternalFormat = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
        break; // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
    case GLU_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA:
        if (glu.ExtSupported.texCompASTC)
            *glInternalFormat = GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
        break; // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
    default:
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: Current format not supported (%i)", format);
        break;
    }
}

// Unload texture from GPU memory
void gluUnloadTexture(unsigned int id) {
    glDeleteTextures(1, &id);
}

// Generate mipmap data for selected texture
// NOTE: Only supports GPU mipmap generation
void gluGenTextureMipmaps(unsigned int id, int width, int height, int format, int *mipmaps) {
    glBindTexture(GL_TEXTURE_2D, id);

    // Check if texture is power-of-two (POT)
    bool texIsPOT = false;

    if (((width > 0) && ((width & (width - 1)) == 0)) &&
        ((height > 0) && ((height & (height - 1)) == 0)))
        texIsPOT = true;

    if ((texIsPOT) || (glu.ExtSupported.texNPOT)) {
        // glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);   // Hint for mipmaps generation algorithm: GL_FASTEST, GL_NICEST, GL_DONT_CARE
        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps automatically
        *mipmaps = 1 + (int)floor(log(MAX(width, height)) / log(2));
        TRACELOG(GLU_LOG_INFO, "TEXTURE: [ID %i] Mipmaps generated automatically, total: %i", id, *mipmaps);
    }
    else
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: [ID %i] Failed to generate mipmaps", id);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Read texture pixel data
void *gluReadTexturePixels(unsigned int id, int width, int height, int format) {
    void *pixels = NULL;

    glBindTexture(GL_TEXTURE_2D, id);

    // NOTE: Using texture id, we can retrieve some texture info (but not on OpenGL ES 2.0)
    // Possible texture info: GL_TEXTURE_RED_SIZE, GL_TEXTURE_GREEN_SIZE, GL_TEXTURE_BLUE_SIZE, GL_TEXTURE_ALPHA_SIZE
    // int width, height, format;
    // glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    // glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    // glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);

    // NOTE: Each row written to or read from by OpenGL pixel operations like glGetTexImage are aligned to a 4 byte boundary by default, which may add some padding.
    // Use glPixelStorei to modify padding with the GL_[UN]PACK_ALIGNMENT setting.
    // GL_PACK_ALIGNMENT affects operations that read from OpenGL memory (glReadPixels, glGetTexImage, etc.)
    // GL_UNPACK_ALIGNMENT affects operations that write to OpenGL memory (glTexImage, etc.)
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    unsigned int glInternalFormat, glFormat, glType;
    gluGetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);
    unsigned int size = gluGetPixelDataSize(width, height, format);

    if ((glInternalFormat != 0) && (format < GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB)) {
        pixels = GLU_MALLOC(size);
        glGetTexImage(GL_TEXTURE_2D, 0, glFormat, glType, pixels);
    }
    else
        TRACELOG(GLU_LOG_WARNING, "TEXTURE: [ID %i] Data retrieval not suported for pixel format (%i)", id, format);

    glBindTexture(GL_TEXTURE_2D, 0);

    return pixels;
}

// Read screen pixel data (color buffer)
unsigned char *gluReadScreenPixels(int width, int height) {
    unsigned char *screenData = (unsigned char *)GLU_CALLOC(width * height * 4, sizeof(unsigned char));

    // NOTE 1: glReadPixels returns image flipped vertically -> (0,0) is the bottom left corner of the framebuffer
    // NOTE 2: We are getting alpha channel! Be careful, it can be transparent if not cleared properly!
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, screenData);

    // Flip image vertically!
    unsigned char *imgData = (unsigned char *)GLU_MALLOC(width * height * 4 * sizeof(unsigned char));

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < (width * 4); x++) {
            imgData[((height - 1) - y) * width * 4 + x] = screenData[(y * width * 4) + x]; // Flip line

            // Set alpha component value to 255 (no trasparent image retrieval)
            // NOTE: Alpha value has already been applied to RGB in framebuffer, we don't need it!
            if (((x + 1) % 4) == 0)
                imgData[((height - 1) - y) * width * 4 + x] = 255;
        }
    }

    GLU_FREE(screenData);

    return imgData; // NOTE: image data should be freed
}

// Framebuffer management (fbo)
//-----------------------------------------------------------------------------------------
// Load a framebuffer to be used for rendering
// NOTE: No textures attached
unsigned int gluLoadFramebuffer(void) {
    unsigned int fboId = 0;

    glGenFramebuffers(1, &fboId);         // Create the framebuffer object
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind any framebuffer
    
    return fboId;
}

// Attach color buffer texture to an fbo (unloads previous attachment)
// NOTE: Attach type: 0-Color, 1-Depth renderbuffer, 2-Depth texture
void gluFramebufferAttach(unsigned int fboId, unsigned int texId, int attachType, int texType, int mipLevel) {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    switch (attachType) {
    case GLU_ATTACHMENT_COLOR_CHANNEL0:
    case GLU_ATTACHMENT_COLOR_CHANNEL1:
    case GLU_ATTACHMENT_COLOR_CHANNEL2:
    case GLU_ATTACHMENT_COLOR_CHANNEL3:
    case GLU_ATTACHMENT_COLOR_CHANNEL4:
    case GLU_ATTACHMENT_COLOR_CHANNEL5:
    case GLU_ATTACHMENT_COLOR_CHANNEL6:
    case GLU_ATTACHMENT_COLOR_CHANNEL7: {
        if (texType == GLU_ATTACHMENT_TEXTURE2D)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_TEXTURE_2D, texId, mipLevel);
        else if (texType == GLU_ATTACHMENT_RENDERBUFFER)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_RENDERBUFFER, texId);
        else if (texType >= GLU_ATTACHMENT_CUBEMAP_POSITIVE_X)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_TEXTURE_CUBE_MAP_POSITIVE_X + texType, texId, mipLevel);
    }
    break;
    case GLU_ATTACHMENT_DEPTH: {
        if (texType == GLU_ATTACHMENT_TEXTURE2D)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, mipLevel);
        else if (texType == GLU_ATTACHMENT_RENDERBUFFER)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, texId);
    }
    break;
    case GLU_ATTACHMENT_STENCIL: {
        if (texType == GLU_ATTACHMENT_TEXTURE2D)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texId, mipLevel);
        else if (texType == GLU_ATTACHMENT_RENDERBUFFER)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, texId);
    }
    break;
    default:
        break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Verify render texture is complete
bool gluFramebufferComplete(unsigned int id) {
    bool result = false;

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        switch (status) {
        case GL_FRAMEBUFFER_UNSUPPORTED:
            TRACELOG(GLU_LOG_WARNING, "FBO: [ID %i] Framebuffer is unsupported", id);
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            TRACELOG(GLU_LOG_WARNING, "FBO: [ID %i] Framebuffer has incomplete attachment", id);
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            TRACELOG(GLU_LOG_WARNING, "FBO: [ID %i] Framebuffer has a missing attachment", id);
            break;
        default:
            break;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    result = (status == GL_FRAMEBUFFER_COMPLETE);

    return result;
}

// Unload framebuffer from GPU memory
// NOTE: All attached textures/cubemaps/renderbuffers are also deleted
void gluUnloadFramebuffer(unsigned int id) {
    // Query depth attachment to automatically delete texture/renderbuffer
    int depthType = 0, depthId = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, id); // Bind framebuffer to query depth texture type
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &depthType);

    // TODO: Review warning retrieving object name in WebGL
    // WARNING: WebGL: INVALID_ENUM: getFramebufferAttachmentParameter: invalid parameter name
    // https://registry.khronos.org/webgl/specs/latest/1.0/
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &depthId);

    unsigned int depthIdU = (unsigned int)depthId;
    if (depthType == GL_RENDERBUFFER)
        glDeleteRenderbuffers(1, &depthIdU);
    else if (depthType == GL_TEXTURE)
        glDeleteTextures(1, &depthIdU);

    // NOTE: If a texture object is deleted while its image is attached to the *currently bound* framebuffer,
    // the texture image is automatically detached from the currently bound framebuffer.

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &id);

    TRACELOG(GLU_LOG_INFO, "FBO: [ID %i] Unloaded framebuffer from VRAM (GPU)", id);
}

// Vertex data management
//-----------------------------------------------------------------------------------------
// Load a new attributes buffer
unsigned int gluLoadVertexBuffer(const void *buffer, int size, bool dynamic) {
    unsigned int id = 0;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    return id;
}

// Load a new attributes element buffer
unsigned int gluLoadVertexBufferElement(const void *buffer, int size, bool dynamic) {
    unsigned int id = 0;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    return id;
}

// Enable vertex buffer (VBO)
void gluEnableVertexBuffer(unsigned int id) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

// Disable vertex buffer (VBO)
void gluDisableVertexBuffer(void) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Enable vertex buffer element (VBO element)
void gluEnableVertexBufferElement(unsigned int id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

// Disable vertex buffer element (VBO element)
void gluDisableVertexBufferElement(void) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Update vertex buffer with new data
// NOTE: dataSize and offset must be provided in bytes
void gluUpdateVertexBuffer(unsigned int id, const void *data, int dataSize, int offset) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
}

// Update vertex buffer elements with new data
// NOTE: dataSize and offset must be provided in bytes
void gluUpdateVertexBufferElements(unsigned int id, const void *data, int dataSize, int offset) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, dataSize, data);
}

// Enable vertex array object (VAO)
bool gluEnableVertexArray(unsigned int vaoId) {
    bool result = false;
    if (glu.ExtSupported.vao) {
        glBindVertexArray(vaoId);
        result = true;
    }
    return result;
}

// Disable vertex array object (VAO)
void gluDisableVertexArray(void) {
    if (glu.ExtSupported.vao)
        glBindVertexArray(0);
}

// Enable vertex attribute index
void gluEnableVertexAttribute(unsigned int index) {
    glEnableVertexAttribArray(index);
}

// Disable vertex attribute index
void gluDisableVertexAttribute(unsigned int index) {
    glDisableVertexAttribArray(index);
}

// Draw vertex array
void gluDrawVertexArray(int offset, int count) {
    glDrawArrays(GL_TRIANGLES, offset, count);
}

// Draw vertex array elements
void gluDrawVertexArrayElements(int offset, int count, const void *buffer) {
    // NOTE: Added pointer math separately from function to avoid UBSAN complaining
    unsigned short *bufferPtr = (unsigned short *)buffer;
    if (offset > 0)
        bufferPtr += offset;

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (const unsigned short *)bufferPtr);
}

// Draw vertex array instanced
void gluDrawVertexArrayInstanced(int offset, int count, int instances) {
    glDrawArraysInstanced(GL_TRIANGLES, 0, count, instances);
}

// Draw vertex array elements instanced
void gluDrawVertexArrayElementsInstanced(int offset, int count, const void *buffer, int instances) {
    // NOTE: Added pointer math separately from function to avoid UBSAN complaining
    unsigned short *bufferPtr = (unsigned short *)buffer;
    if (offset > 0)
        bufferPtr += offset;

    glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (const unsigned short *)bufferPtr, instances);
}

// Load vertex array object (VAO)
unsigned int gluLoadVertexArray(void) {
    unsigned int vaoId = 0;
    if (glu.ExtSupported.vao)
        glGenVertexArrays(1, &vaoId);
    return vaoId;
}

// Set vertex attribute
void gluSetVertexAttribute(unsigned int index, int compSize, int type, bool normalized, int stride, int offset) {
    // NOTE: Data type could be: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT
    // Additional types (depends on OpenGL version or extensions):
    //  - GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED,
    //  - GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV

    size_t offsetNative = offset;
    glVertexAttribPointer(index, compSize, type, normalized, stride, (void *)offsetNative);
}

// Set vertex attribute divisor
void gluSetVertexAttributeDivisor(unsigned int index, int divisor) {
#if FUNGL_VERSION >= GL_VERSION_3_3
    glVertexAttribDivisor(index, divisor);
#else
    TRACELOG(GLU_LOG_WARNING, "VAO: glSetVertexAttributeDivisor requires FUNGL_VERSION>=GL_VERSION_3_3", shader);
#endif
}

// Unload vertex array object (VAO)
void gluUnloadVertexArray(unsigned int vaoId) {
    if (glu.ExtSupported.vao) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vaoId);
        TRACELOG(GLU_LOG_INFO, "VAO: [ID %i] Unloaded vertex array data from VRAM (GPU)", vaoId);
    }
}

// Unload vertex buffer (VBO)
void gluUnloadVertexBuffer(unsigned int vboId) {
    glDeleteBuffers(1, &vboId);
    // TRACELOG(GLU_LOG_INFO, "VBO: Unloaded vertex data from VRAM (GPU)");
}

// Shaders management
//-----------------------------------------------------------------------------------------------
// Load shader from code strings
// NOTE: If shader string is NULL, using default vertex/fragment shaders
unsigned int gluLoadShaderCode(const char *vsCode, const char *fsCode) {
    unsigned int id = 0;

    unsigned int vertexShaderId = 0;
    unsigned int fragmentShaderId = 0;

    // Compile vertex shader (if provided)
    // NOTE: If not vertex shader is provided, use default one
    if (vsCode != NULL)
        vertexShaderId = gluCompileShader(vsCode, GL_VERTEX_SHADER);
    else
        vertexShaderId = glu.defaultVShaderId;

    // Compile fragment shader (if provided)
    // NOTE: If not vertex shader is provided, use default one
    if (fsCode != NULL)
        fragmentShaderId = gluCompileShader(fsCode, GL_FRAGMENT_SHADER);
    else
        fragmentShaderId = glu.defaultFShaderId;

    // In case vertex and fragment shader are the default ones, no need to recompile, we can just assign the default shader program id
    if ((vertexShaderId == glu.defaultVShaderId) && (fragmentShaderId == glu.defaultFShaderId))
        id = glu.defaultShaderId;
    else if ((vertexShaderId > 0) && (fragmentShaderId > 0)) {
        // One of or both shader are new, we need to compile a new shader program
        id = gluLoadShaderProgram(vertexShaderId, fragmentShaderId);

        // We can detach and delete vertex/fragment shaders (if not default ones)
        // NOTE: We detach shader before deletion to make sure memory is freed
        if (vertexShaderId != glu.defaultVShaderId) {
            // WARNING: Shader program linkage could fail and returned id is 0
            if (id > 0)
                glDetachShader(id, vertexShaderId);
            glDeleteShader(vertexShaderId);
        }
        if (fragmentShaderId != glu.defaultFShaderId) {
            // WARNING: Shader program linkage could fail and returned id is 0
            if (id > 0)
                glDetachShader(id, fragmentShaderId);
            glDeleteShader(fragmentShaderId);
        }

        // In case shader program loading failed, we assign default shader
        if (id == 0) {
            // In case shader loading fails, we return the default shader
            TRACELOG(GLU_LOG_WARNING, "SHADER: Failed to load custom shader code, using default shader");
            id = glu.defaultShaderId;
        }
        /*
        else {
            // Get available shader uniforms
            // NOTE: This information is useful for debug...
            int uniformCount = -1;
            glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount);

            for (int i = 0; i < uniformCount; i++) {
                int namelen = -1;
                int num = -1;
                char name[256] = { 0 };     // Assume no variable names longer than 256
                GLenum type = GL_ZERO;

                // Get the name of the uniforms
                glGetActiveUniform(id, i, sizeof(name) - 1, &namelen, &num, &type, name);

                name[namelen] = 0;
                TRACELOGD("SHADER: [ID %i] Active uniform (%s) set at location: %i", id, name, glGetUniformLocation(id, name));
            }
        }
        */
    }

    return id;
}

// Compile custom shader and return shader id
unsigned int gluCompileShader(const char *shaderCode, int type) {
    unsigned int shader = 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);

    GLint success = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        switch (type) {
        case GL_VERTEX_SHADER:
            TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to compile vertex shader code", shader);
            break;
        case GL_FRAGMENT_SHADER:
            TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to compile fragment shader code", shader);
            break;
            // case GL_GEOMETRY_SHADER:
#if FUNGL_VERSION >= GL_VERSION_4_3
        case GL_COMPUTE_SHADER:
            TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to compile compute shader code", shader);
            break;
#else
        case GL_COMPUTE_SHADER:
            TRACELOG(GLU_LOG_WARNING, "SHADER: Compute shaders requires FUNGL_VERSION>=GL_VERSION_4_3", shader);
            break;
#endif
        default:
            break;
        }

        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0) {
            int length = 0;
            char *log = (char *)GLU_CALLOC(maxLength, sizeof(char));
            glGetShaderInfoLog(shader, maxLength, &length, log);
            TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Compile error: %s", shader, log);
            GLU_FREE(log);
        }

        shader = 0;
    }
    else {
        switch (type) {
        case GL_VERTEX_SHADER:
            TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Vertex shader compiled successfully", shader);
            break;
        case GL_FRAGMENT_SHADER:
            TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Fragment shader compiled successfully", shader);
            break;
            // case GL_GEOMETRY_SHADER:
#if FUNGL_VERSION >= GL_VERSION_4_3
        case GL_COMPUTE_SHADER:
            TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Compute shader compiled successfully", shader);
            break;
#else
        case GL_COMPUTE_SHADER:
            TRACELOG(GLU_LOG_WARNING, "SHADER: Compute shaders requires FUNGL_VERSION>=GL_VERSION_4_3", shader);
            break;
#endif
        default:
            break;
        }
    }

    return shader;
}

// Load custom shader strings and return program id
unsigned int gluLoadShaderProgram(unsigned int vShaderId, unsigned int fShaderId) {
    unsigned int program = 0;

    GLint success = 0;
    program = glCreateProgram();

    glAttachShader(program, vShaderId);
    glAttachShader(program, fShaderId);

    // NOTE: Default attribute shader locations must be Bound before linking
    glBindAttribLocation(program, GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION, GLU_DEFAULT_SHADER_ATTRIB_NAME_POSITION);
    glBindAttribLocation(program, GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD, GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
    glBindAttribLocation(program, GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL, GLU_DEFAULT_SHADER_ATTRIB_NAME_NORMAL);
    glBindAttribLocation(program, GLU_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR, GLU_DEFAULT_SHADER_ATTRIB_NAME_COLOR);
    glBindAttribLocation(program, GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT, GLU_DEFAULT_SHADER_ATTRIB_NAME_TANGENT);
    glBindAttribLocation(program, GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2, GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2);

    // NOTE: If some attrib name is no found on the shader, it locations becomes -1

    glLinkProgram(program);

    // NOTE: All uniform variables are intitialised to 0 when a program links

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to link shader program", program);

        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0) {
            int length = 0;
            char *log = (char *)GLU_CALLOC(maxLength, sizeof(char));
            glGetProgramInfoLog(program, maxLength, &length, log);
            TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Link error: %s", program, log);
            GLU_FREE(log);
        }

        glDeleteProgram(program);

        program = 0;
    }
    else {
        // Get the size of compiled shader program (not available on OpenGL ES 2.0)
        // NOTE: If GL_LINK_STATUS is GL_FALSE, program binary length is zero.
        // GLint binarySize = 0;
        // glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binarySize);

        TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Program shader loaded successfully", program);
    }
    return program;
}

// Unload shader program
void gluUnloadShaderProgram(unsigned int id) {
    glDeleteProgram(id);

    TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Unloaded shader program data from VRAM (GPU)", id);
}

// Get shader location uniform
int gluGetLocationUniform(unsigned int shaderId, const char *uniformName) {
    int location = -1;
    location = glGetUniformLocation(shaderId, uniformName);

    // if (location == -1) TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to find shader uniform: %s", shaderId, uniformName);
    // else TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Shader uniform (%s) set at location: %i", shaderId, uniformName, location);
    return location;
}

// Get shader location attribute
int gluGetLocationAttrib(unsigned int shaderId, const char *attribName) {
    int location = -1;
    location = glGetAttribLocation(shaderId, attribName);

    // if (location == -1) TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to find shader attribute: %s", shaderId, attribName);
    // else TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Shader attribute (%s) set at location: %i", shaderId, attribName, location);
    return location;
}

// Set shader value uniform
void gluSetUniform(int locIndex, const void *value, int uniformType, int count) {
    switch (uniformType) {
    case GLU_SHADER_UNIFORM_FLOAT:
        glUniform1fv(locIndex, count, (float *)value);
        break;
    case GLU_SHADER_UNIFORM_VEC2:
        glUniform2fv(locIndex, count, (float *)value);
        break;
    case GLU_SHADER_UNIFORM_VEC3:
        glUniform3fv(locIndex, count, (float *)value);
        break;
    case GLU_SHADER_UNIFORM_VEC4:
        glUniform4fv(locIndex, count, (float *)value);
        break;
    case GLU_SHADER_UNIFORM_INT:
        glUniform1iv(locIndex, count, (int *)value);
        break;
    case GLU_SHADER_UNIFORM_IVEC2:
        glUniform2iv(locIndex, count, (int *)value);
        break;
    case GLU_SHADER_UNIFORM_IVEC3:
        glUniform3iv(locIndex, count, (int *)value);
        break;
    case GLU_SHADER_UNIFORM_IVEC4:
        glUniform4iv(locIndex, count, (int *)value);
        break;
    case GLU_SHADER_UNIFORM_UINT:
        glUniform1uiv(locIndex, count, (unsigned int *)value);
        break;
    case GLU_SHADER_UNIFORM_UIVEC2:
        glUniform2uiv(locIndex, count, (unsigned int *)value);
        break;
    case GLU_SHADER_UNIFORM_UIVEC3:
        glUniform3uiv(locIndex, count, (unsigned int *)value);
        break;
    case GLU_SHADER_UNIFORM_UIVEC4:
        glUniform4uiv(locIndex, count, (unsigned int *)value);
        break;
    case GLU_SHADER_UNIFORM_SAMPLER2D:
        glUniform1iv(locIndex, count, (int *)value);
        break;
    default:
        TRACELOG(GLU_LOG_WARNING, "SHADER: Failed to set uniform value, data type not recognized");

        // TODO: Support glUniform1uiv(), glUniform2uiv(), glUniform3uiv(), glUniform4uiv()
    }
}

// Set shader value attribute
void gluSetVertexAttributeDefault(int locIndex, const void *value, int attribType, int count) {
    switch (attribType) {
    case GLU_SHADER_ATTRIB_FLOAT:
        if (count == 1)
            glVertexAttrib1fv(locIndex, (float *)value);
        break;
    case GLU_SHADER_ATTRIB_VEC2:
        if (count == 2)
            glVertexAttrib2fv(locIndex, (float *)value);
        break;
    case GLU_SHADER_ATTRIB_VEC3:
        if (count == 3)
            glVertexAttrib3fv(locIndex, (float *)value);
        break;
    case GLU_SHADER_ATTRIB_VEC4:
        if (count == 4)
            glVertexAttrib4fv(locIndex, (float *)value);
        break;
    default:
        TRACELOG(GLU_LOG_WARNING, "SHADER: Failed to set attrib default value, data type not recognized");
    }
}

// Set shader value uniform matrix
void gluSetUniformMatrix(int locIndex, Matrix mat) {
    float matfloat[16] = {
        mat.m0, mat.m1, mat.m2, mat.m3,
        mat.m4, mat.m5, mat.m6, mat.m7,
        mat.m8, mat.m9, mat.m10, mat.m11,
        mat.m12, mat.m13, mat.m14, mat.m15};
    glUniformMatrix4fv(locIndex, 1, false, matfloat);
}

// Set shader value uniform sampler
void gluSetUniformSampler(int locIndex, unsigned int textureId) {
    // Check if texture is already active
    for (int i = 0; i < GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++) {
        if (glu.activeTextureId[i] == textureId) {
            glUniform1i(locIndex, 1 + i);
            return;
        }
    }

    // Register a new active texture for the internal batch system
    // NOTE: Default texture is always activated as GL_TEXTURE0
    for (int i = 0; i < GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++) {
        if (glu.activeTextureId[i] == 0) {
            glUniform1i(locIndex, 1 + i);              // Activate new texture unit
            glu.activeTextureId[i] = textureId; // Save texture id for binding on drawing
            break;
        }
    }
}

// Set shader currently active (id and locations)
void gluSetShader(unsigned int id, int *locs) {
    if (glu.currentShaderId != id) {
        gluDrawRenderBatch(glu.currentBatch);
        glu.currentShaderId = id;
        glu.currentShaderLocs = locs;
    }
}

// Load compute shader program
unsigned int gluLoadComputeShaderProgram(unsigned int shaderId) {
    unsigned int program = 0;

#if FUNGL_VERSION >= GL_VERSION_4_3
    GLint success = 0;
    program = glCreateProgram();
    glAttachShader(program, shaderId);
    glLinkProgram(program);

    // NOTE: All uniform variables are intitialised to 0 when a program links

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to link compute shader program", program);

        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0) {
            int length = 0;
            char *log = (char *)GLU_CALLOC(maxLength, sizeof(char));
            glGetProgramInfoLog(program, maxLength, &length, log);
            TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Link error: %s", program, log);
            GLU_FREE(log);
        }

        glDeleteProgram(program);

        program = 0;
    }
    else {
        // Get the size of compiled shader program (not available on OpenGL ES 2.0)
        // NOTE: If GL_LINK_STATUS is GL_FALSE, program binary length is zero.
        // GLint binarySize = 0;
        // glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binarySize);

        TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Compute shader program loaded successfully", program);
    }
#else
    TRACELOG(GLU_LOG_WARNING, "SHADER: Compute shaders requires FUNGL_VERSION>=GL_VERSION_4_3");
#endif

    return program;
}

// Dispatch compute shader (equivalent to *draw* for graphics pilepine)
void gluComputeShaderDispatch(unsigned int groupX, unsigned int groupY, unsigned int groupZ) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    glDispatchCompute(groupX, groupY, groupZ);
#endif
}

// Load shader storage buffer object (SSBO)
unsigned int gluLoadShaderBuffer(unsigned int size, const void *data, int usageHint) {
    unsigned int ssbo = 0;

#if FUNGL_VERSION >= GL_VERSION_4_3
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usageHint ? usageHint : GLU_STREAM_COPY);
    if (data == NULL)
        glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL); // Clear buffer data to 0
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#else
    TRACELOG(GLU_LOG_WARNING, "SSBO: SSBO requires FUNGL_VERSION>=GL_VERSION_4_3");
#endif

    return ssbo;
}

// Unload shader storage buffer object (SSBO)
void gluUnloadShaderBuffer(unsigned int ssboId) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    glDeleteBuffers(1, &ssboId);
#else
    TRACELOG(GLU_LOG_WARNING, "SSBO: SSBO requires FUNGL_VERSION>=GL_VERSION_4_3");
#endif
}

// Update SSBO buffer data
void gluUpdateShaderBuffer(unsigned int id, const void *data, unsigned int dataSize, unsigned int offset) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, dataSize, data);
#endif
}

// Get SSBO buffer size
unsigned int gluGetShaderBufferSize(unsigned int id) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    GLint64 size = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glGetBufferParameteri64v(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
    return (size > 0) ? (unsigned int)size : 0;
#else
    return 0;
#endif
}

// Read SSBO buffer data (GPU->CPU)
void gluReadShaderBuffer(unsigned int id, void *dest, unsigned int count, unsigned int offset) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, count, dest);
#endif
}

// Bind SSBO buffer
void gluBindShaderBuffer(unsigned int id, unsigned int index) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
#endif
}

// Copy SSBO buffer data
void gluCopyShaderBuffer(unsigned int destId, unsigned int srcId, unsigned int destOffset, unsigned int srcOffset, unsigned int count) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    glBindBuffer(GL_COPY_READ_BUFFER, srcId);
    glBindBuffer(GL_COPY_WRITE_BUFFER, destId);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffset, destOffset, count);
#endif
}

// Bind image texture
void gluBindImageTexture(unsigned int id, unsigned int index, int format, bool readonly) {
#if FUNGL_VERSION >= GL_VERSION_4_3
    unsigned int glInternalFormat = 0, glFormat = 0, glType = 0;

    gluGetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);
    glBindImageTexture(index, id, 0, 0, 0, readonly ? GL_READ_ONLY : GL_READ_WRITE, glInternalFormat);
#else
    TRACELOG(GLU_LOG_WARNING, "TEXTURE: Image texture binding requires FUNGL_VERSION>=GL_VERSION_4_3");
#endif
}

// Matrix state management
//-----------------------------------------------------------------------------------------
// Get internal modelview matrix
Matrix gluGetMatrixModelview(void) {
    Matrix matrix = gluMatrixIdentity();
    matrix = glu.modelview;
    return matrix;
}

// Get internal projection matrix
Matrix gluGetMatrixProjection(void) {
    return glu.projection;
}

// Get internal accumulated transform matrix
Matrix gluGetMatrixTransform(void) {
    Matrix mat = gluMatrixIdentity();
    // TODO: Consider possible transform matrices in the glu.State.stack
    // Is this the right order? or should we start with the first stored matrix instead of the last one?
    // Matrix matStackTransform = gluMatrixIdentity();
    // for (int i = glu.State.stackCounter; i > 0; i--) matStackTransform = gluMatrixMultiply(glu.State.stack[i], matStackTransform);
    mat = glu.transform;
    return mat;
}

// Get internal projection matrix for stereo render (selected eye)
Matrix gluGetMatrixProjectionStereo(int eye) {
    Matrix mat = gluMatrixIdentity();
    mat = glu.projectionStereo[eye];
    return mat;
}

// Get internal view offset matrix for stereo render (selected eye)
Matrix gluGetMatrixViewOffsetStereo(int eye) {
    Matrix mat = gluMatrixIdentity();
    mat = glu.viewOffsetStereo[eye];
    return mat;
}

// Set a custom modelview matrix (replaces internal modelview matrix)
void gluSetMatrixModelview(Matrix view) {
    glu.modelview = view;
}

// Set a custom projection matrix (replaces internal projection matrix)
void gluSetMatrixProjection(Matrix projection) {
    glu.projection = projection;
}

// Set eyes projection matrices for stereo rendering
void gluSetMatrixProjectionStereo(Matrix right, Matrix left) {
    glu.projectionStereo[0] = right;
    glu.projectionStereo[1] = left;
}

// Set eyes view offsets matrices for stereo rendering
void gluSetMatrixViewOffsetStereo(Matrix right, Matrix left) {
    glu.viewOffsetStereo[0] = right;
    glu.viewOffsetStereo[1] = left;
}

// Load and draw a quad in NDC
void gluLoadDrawQuad(void) {
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    float vertices[] = {
        // Positions         Texcoords
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        0.0f,
        1.0f,
        0.0f,
    };

    // Gen VAO to contain VBO
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    // Gen and fill vertex buffer (VBO)
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // Bind vertex attributes (position, texcoords)
    glEnableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION);
    glVertexAttribPointer(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0); // Positions
    glEnableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD);
    glVertexAttribPointer(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))); // Texcoords

    // Draw quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    // Delete buffers (VBO and VAO)
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}

// Load and draw a cube in NDC
void gluLoadDrawCube(void) {
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    float vertices[] = {
        // Positions          Normals               Texcoords
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    // Gen VAO to contain VBO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Gen and fill vertex buffer (VBO)
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind vertex attributes (position, normals, texcoords)
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION);
    glVertexAttribPointer(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0); // Positions
    glEnableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL);
    glVertexAttribPointer(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Normals
    glEnableVertexAttribArray(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD);
    glVertexAttribPointer(GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Texcoords
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Draw cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Delete VBO and VAO
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
}

// Get name string for pixel format
const char *gluGetPixelFormatName(unsigned int format) {
    switch (format) {
    case GLU_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
        return "GRAYSCALE";
        break; // 8 bit per pixel (no alpha)
    case GLU_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
        return "GRAY_ALPHA";
        break; // 8*2 bpp (2 channels)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R5G6B5:
        return "R5G6B5";
        break; // 16 bpp
    case GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8:
        return "R8G8B8";
        break; // 24 bpp
    case GLU_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
        return "R5G5B5A1";
        break; // 16 bpp (1 bit alpha)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
        return "R4G4B4A4";
        break; // 16 bpp (4 bit alpha)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
        return "R8G8B8A8";
        break; // 32 bpp
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32:
        return "R32";
        break; // 32 bpp (1 channel - float)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32:
        return "R32G32B32";
        break; // 32*3 bpp (3 channels - float)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:
        return "R32G32B32A32";
        break; // 32*4 bpp (4 channels - float)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16:
        return "R16";
        break; // 16 bpp (1 channel - half float)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16:
        return "R16G16B16";
        break; // 16*3 bpp (3 channels - half float)
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16:
        return "R16G16B16A16";
        break; // 16*4 bpp (4 channels - half float)
    case GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB:
        return "DXT1_RGB";
        break; // 4 bpp (no alpha)
    case GLU_PIXELFORMAT_COMPRESSED_DXT1_RGBA:
        return "DXT1_RGBA";
        break; // 4 bpp (1 bit alpha)
    case GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA:
        return "DXT3_RGBA";
        break; // 8 bpp
    case GLU_PIXELFORMAT_COMPRESSED_DXT5_RGBA:
        return "DXT5_RGBA";
        break; // 8 bpp
    case GLU_PIXELFORMAT_COMPRESSED_ETC1_RGB:
        return "ETC1_RGB";
        break; // 4 bpp
    case GLU_PIXELFORMAT_COMPRESSED_ETC2_RGB:
        return "ETC2_RGB";
        break; // 4 bpp
    case GLU_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA:
        return "ETC2_RGBA";
        break; // 8 bpp
    case GLU_PIXELFORMAT_COMPRESSED_PVRT_RGB:
        return "PVRT_RGB";
        break; // 4 bpp
    case GLU_PIXELFORMAT_COMPRESSED_PVRT_RGBA:
        return "PVRT_RGBA";
        break; // 4 bpp
    case GLU_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA:
        return "ASTC_4x4_RGBA";
        break; // 8 bpp
    case GLU_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA:
        return "ASTC_8x8_RGBA";
        break; // 2 bpp
    default:
        return "UNKNOWN";
        break;
    }
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Load default shader (just vertex positioning and texture coloring)
// NOTE: This shader program is used for internal buffers
// NOTE: Loaded: glu.State.defaultShaderId, glu.State.defaultShaderLocs
static void gluLoadShaderDefault(void) {
    glu.defaultShaderLocs = (int *)GLU_CALLOC(GLU_MAX_SHADER_LOCATIONS, sizeof(int));

    // NOTE: All locations must be reseted to -1 (no location)
    for (int i = 0; i < GLU_MAX_SHADER_LOCATIONS; i++)
        glu.defaultShaderLocs[i] = -1;

    // Vertex shader directly defined, no external file required
    const char *defaultVShaderCode =
        "#version 330                       \n"
        "in vec3 vertexPosition;            \n"
        "in vec2 vertexTexCoord;            \n"
        "in vec4 vertexColor;               \n"
        "out vec2 fragTexCoord;             \n"
        "out vec4 fragColor;                \n"
        "uniform mat4 mvp;                  \n"
        "void main()                        \n"
        "{                                  \n"
        "    fragTexCoord = vertexTexCoord; \n"
        "    fragColor = vertexColor;       \n"
        "    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
        "}                                  \n";

    // Fragment shader directly defined, no external file required
    const char *defaultFShaderCode =
        "#version 330       \n"
        "in vec2 fragTexCoord;              \n"
        "in vec4 fragColor;                 \n"
        "out vec4 finalColor;               \n"
        "uniform sampler2D texture0;        \n"
        "uniform vec4 colDiffuse;           \n"
        "void main()                        \n"
        "{                                  \n"
        "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
        "    finalColor = texelColor*colDiffuse*fragColor;        \n"
        "}                                  \n";

    // NOTE: Compiled vertex/fragment shaders are not deleted,
    // they are kept for re-use as default shaders in case some shader loading fails
    glu.defaultVShaderId = gluCompileShader(defaultVShaderCode, GL_VERTEX_SHADER);   // Compile default vertex shader
    glu.defaultFShaderId = gluCompileShader(defaultFShaderCode, GL_FRAGMENT_SHADER); // Compile default fragment shader

    glu.defaultShaderId = gluLoadShaderProgram(glu.defaultVShaderId, glu.defaultFShaderId);

    if (glu.defaultShaderId > 0) {
        TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Default shader loaded successfully", RLGL.State.defaultShaderId);

        // Set default shader locations: attributes locations
        glu.defaultShaderLocs[GLU_SHADER_LOC_VERTEX_POSITION] = glGetAttribLocation(glu.defaultShaderId, GLU_DEFAULT_SHADER_ATTRIB_NAME_POSITION);
        glu.defaultShaderLocs[GLU_SHADER_LOC_VERTEX_TEXCOORD01] = glGetAttribLocation(glu.defaultShaderId, GLU_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
        glu.defaultShaderLocs[GLU_SHADER_LOC_VERTEX_COLOR] = glGetAttribLocation(glu.defaultShaderId, GLU_DEFAULT_SHADER_ATTRIB_NAME_COLOR);

        // Set default shader locations: uniform locations
        glu.defaultShaderLocs[GLU_SHADER_LOC_MATRIX_MVP] = glGetUniformLocation(glu.defaultShaderId, GLU_DEFAULT_SHADER_UNIFORM_NAME_MVP);
        glu.defaultShaderLocs[GLU_SHADER_LOC_COLOR_DIFFUSE] = glGetUniformLocation(glu.defaultShaderId, GLU_DEFAULT_SHADER_UNIFORM_NAME_COLOR);
        glu.defaultShaderLocs[GLU_SHADER_LOC_MAP_DIFFUSE] = glGetUniformLocation(glu.defaultShaderId, GLU_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);
    }
    else
        TRACELOG(GLU_LOG_WARNING, "SHADER: [ID %i] Failed to load default shader", RLGL.State.defaultShaderId);
}

// Unload default shader
// NOTE: Unloads: glu.State.defaultShaderId, glu.State.defaultShaderLocs
static void gluUnloadShaderDefault(void) {
    glUseProgram(0);

    glDetachShader(glu.defaultShaderId, glu.defaultVShaderId);
    glDetachShader(glu.defaultShaderId, glu.defaultFShaderId);
    glDeleteShader(glu.defaultVShaderId);
    glDeleteShader(glu.defaultFShaderId);

    glDeleteProgram(glu.defaultShaderId);

    GLU_FREE(glu.defaultShaderLocs);

    TRACELOG(GLU_LOG_INFO, "SHADER: [ID %i] Default shader unloaded successfully", RLGL.State.defaultShaderId);
}

// Get pixel data size in bytes (image or texture)
// NOTE: Size depends on pixel format
static int gluGetPixelDataSize(int width, int height, int format) {
    int dataSize = 0; // Size in bytes
    int bpp = 0;      // Bits per pixel

    switch (format) {
    case GLU_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
        bpp = 8;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
    case GLU_PIXELFORMAT_UNCOMPRESSED_R5G6B5:
    case GLU_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
    case GLU_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
        bpp = 16;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
        bpp = 32;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8:
        bpp = 24;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32:
        bpp = 32;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32:
        bpp = 32 * 3;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:
        bpp = 32 * 4;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16:
        bpp = 16;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16:
        bpp = 16 * 3;
        break;
    case GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16:
        bpp = 16 * 4;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB:
    case GLU_PIXELFORMAT_COMPRESSED_DXT1_RGBA:
    case GLU_PIXELFORMAT_COMPRESSED_ETC1_RGB:
    case GLU_PIXELFORMAT_COMPRESSED_ETC2_RGB:
    case GLU_PIXELFORMAT_COMPRESSED_PVRT_RGB:
    case GLU_PIXELFORMAT_COMPRESSED_PVRT_RGBA:
        bpp = 4;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA:
    case GLU_PIXELFORMAT_COMPRESSED_DXT5_RGBA:
    case GLU_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA:
    case GLU_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA:
        bpp = 8;
        break;
    case GLU_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA:
        bpp = 2;
        break;
    default:
        break;
    }

    double bytesPerPixel = (double)bpp / 8.0;
    dataSize = (int)(bytesPerPixel * width * height); // Total data size in bytes

    // Most compressed formats works on 4x4 blocks,
    // if texture is smaller, minimum dataSize is 8 or 16
    if ((width < 4) && (height < 4)) {
        if ((format >= GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB) && (format < GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA))
            dataSize = 8;
        else if ((format >= GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA) && (format < GLU_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA))
            dataSize = 16;
    }

    return dataSize;
}

// Auxiliar math functions

// Get float array of matrix data
static glu_float16 gluMatrixToFloatV(Matrix mat) {
    glu_float16 result = {0};

    result.v[0] = mat.m0;
    result.v[1] = mat.m1;
    result.v[2] = mat.m2;
    result.v[3] = mat.m3;
    result.v[4] = mat.m4;
    result.v[5] = mat.m5;
    result.v[6] = mat.m6;
    result.v[7] = mat.m7;
    result.v[8] = mat.m8;
    result.v[9] = mat.m9;
    result.v[10] = mat.m10;
    result.v[11] = mat.m11;
    result.v[12] = mat.m12;
    result.v[13] = mat.m13;
    result.v[14] = mat.m14;
    result.v[15] = mat.m15;

    return result;
}

// Get identity matrix
static Matrix gluMatrixIdentity(void) {
    Matrix result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    return result;
}

// Get two matrix multiplication
// NOTE: When multiplying matrices... the order matters!
static Matrix gluMatrixMultiply(Matrix left, Matrix right) {
    Matrix result = {0};

    result.m0 = left.m0 * right.m0 + left.m1 * right.m4 + left.m2 * right.m8 + left.m3 * right.m12;
    result.m1 = left.m0 * right.m1 + left.m1 * right.m5 + left.m2 * right.m9 + left.m3 * right.m13;
    result.m2 = left.m0 * right.m2 + left.m1 * right.m6 + left.m2 * right.m10 + left.m3 * right.m14;
    result.m3 = left.m0 * right.m3 + left.m1 * right.m7 + left.m2 * right.m11 + left.m3 * right.m15;
    result.m4 = left.m4 * right.m0 + left.m5 * right.m4 + left.m6 * right.m8 + left.m7 * right.m12;
    result.m5 = left.m4 * right.m1 + left.m5 * right.m5 + left.m6 * right.m9 + left.m7 * right.m13;
    result.m6 = left.m4 * right.m2 + left.m5 * right.m6 + left.m6 * right.m10 + left.m7 * right.m14;
    result.m7 = left.m4 * right.m3 + left.m5 * right.m7 + left.m6 * right.m11 + left.m7 * right.m15;
    result.m8 = left.m8 * right.m0 + left.m9 * right.m4 + left.m10 * right.m8 + left.m11 * right.m12;
    result.m9 = left.m8 * right.m1 + left.m9 * right.m5 + left.m10 * right.m9 + left.m11 * right.m13;
    result.m10 = left.m8 * right.m2 + left.m9 * right.m6 + left.m10 * right.m10 + left.m11 * right.m14;
    result.m11 = left.m8 * right.m3 + left.m9 * right.m7 + left.m10 * right.m11 + left.m11 * right.m15;
    result.m12 = left.m12 * right.m0 + left.m13 * right.m4 + left.m14 * right.m8 + left.m15 * right.m12;
    result.m13 = left.m12 * right.m1 + left.m13 * right.m5 + left.m14 * right.m9 + left.m15 * right.m13;
    result.m14 = left.m12 * right.m2 + left.m13 * right.m6 + left.m14 * right.m10 + left.m15 * right.m14;
    result.m15 = left.m12 * right.m3 + left.m13 * right.m7 + left.m14 * right.m11 + left.m15 * right.m15;

    return result;
}

// Transposes provided matrix
static Matrix gluMatrixTranspose(Matrix mat) {
    Matrix result = {0};

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
}

// Invert provided matrix
static Matrix gluMatrixInvert(Matrix mat) {
    Matrix result = {0};

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    float b00 = a00 * a11 - a01 * a10;
    float b01 = a00 * a12 - a02 * a10;
    float b02 = a00 * a13 - a03 * a10;
    float b03 = a01 * a12 - a02 * a11;
    float b04 = a01 * a13 - a03 * a11;
    float b05 = a02 * a13 - a03 * a12;
    float b06 = a20 * a31 - a21 * a30;
    float b07 = a20 * a32 - a22 * a30;
    float b08 = a20 * a33 - a23 * a30;
    float b09 = a21 * a32 - a22 * a31;
    float b10 = a21 * a33 - a23 * a31;
    float b11 = a22 * a33 - a23 * a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

    result.m0 = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;
    result.m1 = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
    result.m2 = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;
    result.m3 = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;
    result.m4 = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
    result.m5 = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;
    result.m6 = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
    result.m7 = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;
    result.m8 = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;
    result.m9 = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
    result.m10 = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;
    result.m11 = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;
    result.m12 = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
    result.m13 = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;
    result.m14 = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
    result.m15 = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;

    return result;
}
