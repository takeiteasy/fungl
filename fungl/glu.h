/* glu.h -- http://www.github.com/takeiteasy/fungl

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

#if !defined(glu_h) && !defined(FUNGL_NO_GLU)
#define glu_h
#if defined(__cplusplus)
extern "C" {
#endif
#if FUNGL_VERSION < GL_VERSION_3_2
#error "glu.h requires FUNGL_VERSION >= GL_VERSION_3_2"
#endif
#include "gl.h"
#include "glm.h"

// Support TRACELOG macros
#ifndef TRACELOG
#define TRACELOG(level, ...) (void)0
#define TRACELOGD(...) (void)0
#endif

// Allow custom memory allocators
#ifndef GLU_MALLOC
#define GLU_MALLOC(sz) malloc(sz)
#endif
#ifndef GLU_CALLOC
#define GLU_CALLOC(n, sz) calloc(n, sz)
#endif
#ifndef GLU_REALLOC
#define GLU_REALLOC(n, sz) realloc(n, sz)
#endif
#ifndef GLU_FREE
#define GLU_FREE(p) free(p)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------

// Default internal render batch elements limits
#ifndef GLU_DEFAULT_BATCH_BUFFER_ELEMENTS
// This is the maximum amount of elements (quads) per batch
// NOTE: Be careful with text, every letter maps to a quad
#define GLU_DEFAULT_BATCH_BUFFER_ELEMENTS 8192
#endif
#ifndef GLU_DEFAULT_BATCH_BUFFERS
#define GLU_DEFAULT_BATCH_BUFFERS 1 // Default number of batch buffers (multi-buffering)
#endif
#ifndef GLU_DEFAULT_BATCH_DRAWCALLS
#define GLU_DEFAULT_BATCH_DRAWCALLS 256 // Default number of batch draw calls (by state changes: mode, texture)
#endif
#ifndef GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS
#define GLU_DEFAULT_BATCH_MAX_TEXTURE_UNITS 4 // Maximum number of textures units that can be activated on batch drawing (SetShaderValueTexture())
#endif

// Internal Matrix stack
#ifndef GLU_MAX_MATRIX_STACK_SIZE
#define GLU_MAX_MATRIX_STACK_SIZE 32 // Maximum size of Matrix stack
#endif

// Shader limits
#ifndef GLU_MAX_SHADER_LOCATIONS
#define GLU_MAX_SHADER_LOCATIONS 32 // Maximum number of shader locations supported
#endif

// Projection matrix culling
#ifndef GLU_CULL_DISTANCE_NEAR
#define GLU_CULL_DISTANCE_NEAR 0.01 // Default near cull distance
#endif
#ifndef GLU_CULL_DISTANCE_FAR
#define GLU_CULL_DISTANCE_FAR 1000.0 // Default far cull distance
#endif

// Texture parameters (equivalent to OpenGL defines)
#define GLU_TEXTURE_WRAP_S 0x2802     // GL_TEXTURE_WRAP_S
#define GLU_TEXTURE_WRAP_T 0x2803     // GL_TEXTURE_WRAP_T
#define GLU_TEXTURE_MAG_FILTER 0x2800 // GL_TEXTURE_MAG_FILTER
#define GLU_TEXTURE_MIN_FILTER 0x2801 // GL_TEXTURE_MIN_FILTER

#define GLU_TEXTURE_FILTER_NEAREST 0x2600            // GL_NEAREST
#define GLU_TEXTURE_FILTER_LINEAR 0x2601             // GL_LINEAR
#define GLU_TEXTURE_FILTER_MIP_NEAREST 0x2700        // GL_NEAREST_MIPMAP_NEAREST
#define GLU_TEXTURE_FILTER_NEAREST_MIP_LINEAR 0x2702 // GL_NEAREST_MIPMAP_LINEAR
#define GLU_TEXTURE_FILTER_LINEAR_MIP_NEAREST 0x2701 // GL_LINEAR_MIPMAP_NEAREST
#define GLU_TEXTURE_FILTER_MIP_LINEAR 0x2703         // GL_LINEAR_MIPMAP_LINEAR
#define GLU_TEXTURE_FILTER_ANISOTROPIC 0x3000        // Anisotropic filter (custom identifier)
#define GLU_TEXTURE_MIPMAP_BIAS_RATIO 0x4000         // Texture mipmap bias, percentage ratio (custom identifier)

#define GLU_TEXTURE_WRAP_REPEAT 0x2901        // GL_REPEAT
#define GLU_TEXTURE_WRAP_CLAMP 0x812F         // GL_CLAMP_TO_EDGE
#define GLU_TEXTURE_WRAP_MIRROR_REPEAT 0x8370 // GL_MIRRORED_REPEAT
#define GLU_TEXTURE_WRAP_MIRROR_CLAMP 0x8742  // GL_MIRROR_CLAMP_EXT

// Matrix modes (equivalent to OpenGL)
#define GLU_MODELVIEW 0x1700  // GL_MODELVIEW
#define GLU_PROJECTION 0x1701 // GL_PROJECTION
#define GLU_TEXTURE 0x1702    // GL_TEXTURE

// Primitive assembly draw modes
#define GLU_LINES 0x0001     // GL_LINES
#define GLU_TRIANGLES 0x0004 // GL_TRIANGLES
#define GLU_QUADS 0x0007     // GL_QUADS

// GL equivalent data types
#define GLU_UNSIGNED_BYTE 0x1401 // GL_UNSIGNED_BYTE
#define GLU_FLOAT 0x1406         // GL_FLOAT

// GL buffer usage hint
#define GLU_STREAM_DRAW 0x88E0  // GL_STREAM_DRAW
#define GLU_STREAM_READ 0x88E1  // GL_STREAM_READ
#define GLU_STREAM_COPY 0x88E2  // GL_STREAM_COPY
#define GLU_STATIC_DRAW 0x88E4  // GL_STATIC_DRAW
#define GLU_STATIC_READ 0x88E5  // GL_STATIC_READ
#define GLU_STATIC_COPY 0x88E6  // GL_STATIC_COPY
#define GLU_DYNAMIC_DRAW 0x88E8 // GL_DYNAMIC_DRAW
#define GLU_DYNAMIC_READ 0x88E9 // GL_DYNAMIC_READ
#define GLU_DYNAMIC_COPY 0x88EA // GL_DYNAMIC_COPY

// GL Shader type
#define GLU_FRAGMENT_SHADER 0x8B30 // GL_FRAGMENT_SHADER
#define GLU_VERTEX_SHADER 0x8B31   // GL_VERTEX_SHADER
#define GLU_COMPUTE_SHADER 0x91B9  // GL_COMPUTE_SHADER

// GL blending factors
#define GLU_ZERO 0                          // GL_ZERO
#define GLU_ONE 1                           // GL_ONE
#define GLU_SRC_COLOR 0x0300                // GL_SRC_COLOR
#define GLU_ONE_MINUS_SRC_COLOR 0x0301      // GL_ONE_MINUS_SRC_COLOR
#define GLU_SRC_ALPHA 0x0302                // GL_SRC_ALPHA
#define GLU_ONE_MINUS_SRC_ALPHA 0x0303      // GL_ONE_MINUS_SRC_ALPHA
#define GLU_DST_ALPHA 0x0304                // GL_DST_ALPHA
#define GLU_ONE_MINUS_DST_ALPHA 0x0305      // GL_ONE_MINUS_DST_ALPHA
#define GLU_DST_COLOR 0x0306                // GL_DST_COLOR
#define GLU_ONE_MINUS_DST_COLOR 0x0307      // GL_ONE_MINUS_DST_COLOR
#define GLU_SRC_ALPHA_SATURATE 0x0308       // GL_SRC_ALPHA_SATURATE
#define GLU_CONSTANT_COLOR 0x8001           // GL_CONSTANT_COLOR
#define GLU_ONE_MINUS_CONSTANT_COLOR 0x8002 // GL_ONE_MINUS_CONSTANT_COLOR
#define GLU_CONSTANT_ALPHA 0x8003           // GL_CONSTANT_ALPHA
#define GLU_ONE_MINUS_CONSTANT_ALPHA 0x8004 // GL_ONE_MINUS_CONSTANT_ALPHA

// GL blending functions/equations
#define GLU_FUNC_ADD 0x8006              // GL_FUNC_ADD
#define GLU_MIN 0x8007                   // GL_MIN
#define GLU_MAX 0x8008                   // GL_MAX
#define GLU_FUNC_SUBTRACT 0x800A         // GL_FUNC_SUBTRACT
#define GLU_FUNC_REVERSE_SUBTRACT 0x800B // GL_FUNC_REVERSE_SUBTRACT
#define GLU_BLEND_EQUATION 0x8009        // GL_BLEND_EQUATION
#define GLU_BLEND_EQUATION_RGB 0x8009    // GL_BLEND_EQUATION_RGB   // (Same as BLEND_EQUATION)
#define GLU_BLEND_EQUATION_ALPHA 0x883D  // GL_BLEND_EQUATION_ALPHA
#define GLU_BLEND_DST_RGB 0x80C8         // GL_BLEND_DST_RGB
#define GLU_BLEND_SRC_RGB 0x80C9         // GL_BLEND_SRC_RGB
#define GLU_BLEND_DST_ALPHA 0x80CA       // GL_BLEND_DST_ALPHA
#define GLU_BLEND_SRC_ALPHA 0x80CB       // GL_BLEND_SRC_ALPHA
#define GLU_BLEND_COLOR 0x8005           // GL_BLEND_COLOR

#define GLU_READ_FRAMEBUFFER 0x8CA8 // GL_READ_FRAMEBUFFER
#define GLU_DRAW_FRAMEBUFFER 0x8CA9 // GL_DRAW_FRAMEBUFFER

// Default shader vertex attribute locations
#ifndef GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION
#define GLU_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION 0
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD
#define GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD 1
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL
#define GLU_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL 2
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR
#define GLU_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR 3
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT
#define GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT 4
#endif
#ifndef GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2
#define GLU_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2 5
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
#include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool) && !defined(GLU_BOOL_TYPE)
// Boolean type
typedef enum bool {
    false = 0,
    true = !false
} bool;
#endif

#if !defined(GLU_MATRIX_TYPE)
// Matrix, 4x4 components, column major, OpenGL style, right handed
typedef struct Matrix {
    float m0, m4, m8, m12;  // Matrix first row (4 components)
    float m1, m5, m9, m13;  // Matrix second row (4 components)
    float m2, m6, m10, m14; // Matrix third row (4 components)
    float m3, m7, m11, m15; // Matrix fourth row (4 components)
} Matrix;
#define GLU_MATRIX_TYPE
#endif

// Dynamic vertex buffers (position + texcoords + colors + indices arrays)
typedef struct gluVertexBuffer {
    int elementCount; // Number of elements in the buffer (QUADS)
    float *vertices;       // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;      // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *normals;        // Vertex normal (XYZ - 3 components per vertex) (shader-location = 2)
    unsigned char *colors; // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned int *indices; // Vertex indices (in case vertex data comes indexed) (6 indices per quad)
    unsigned int vaoId;    // OpenGL Vertex Array Object id
    unsigned int vboId[5]; // OpenGL Vertex Buffer Objects id (5 types of vertex data)
} gluVertexBuffer;

// Draw call type
// NOTE: Only texture changes register a new draw, other state-change-related elements are not
// used at this moment (vaoId, shaderId, matrices), raylib just forces a batch draw call if any
// of those state-change happens (this is done in core module)
typedef struct gluDrawCall {
    int mode;            // Drawing mode: LINES, TRIANGLES, QUADS
    int vertexCount;     // Number of vertex of the draw
    int vertexAlignment; // Number of vertex required for index alignment (LINES, TRIANGLES)
    // unsigned int vaoId;       // Vertex array id to be used on the draw -> Using glu.currentBatch->vertexBuffer.vaoId
    // unsigned int shaderId;    // Shader id to be used on the draw -> Using glu.currentShaderId
    unsigned int textureId; // Texture id to be used on the draw -> Use to create new draw call if changes

    // Matrix projection;        // Projection matrix for this draw -> Using glu.projection by default
    // Matrix modelview;         // Modelview matrix for this draw -> Using glu.modelview by default
} gluDrawCall;

// gluRenderBatch type
typedef struct gluRenderBatch {
    int bufferCount;              // Number of vertex buffers (multi-buffering support)
    int currentBuffer;            // Current buffer tracking in case of multi-buffering
    gluVertexBuffer *vertexBuffer; // Dynamic buffer(s) for vertex data

    gluDrawCall *draws;  // Draw calls array, depends on textureId
    int drawCounter;    // Draw calls counter
    float currentDepth; // Current depth value for next draw
} gluRenderBatch;

// OpenGL version
typedef enum {
    GLU_OPENGL_11 = 1, // OpenGL 1.1
    GLU_OPENGL_21,     // OpenGL 2.1 (GLSL 120)
    GLU_OPENGL_33,     // OpenGL 3.3 (GLSL 330)
    GLU_OPENGL_43,     // OpenGL 4.3 (using GLSL 330)
    GLU_OPENGL_ES_20,  // OpenGL ES 2.0 (GLSL 100)
    GLU_OPENGL_ES_30   // OpenGL ES 3.0 (GLSL 300 es)
} gluGlVersion;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    GLU_LOG_ALL = 0, // Display all logs
    GLU_LOG_TRACE,   // Trace logging, intended for internal use only
    GLU_LOG_DEBUG,   // Debug logging, used for internal debugging, it should be disabled on release builds
    GLU_LOG_INFO,    // Info logging, used for program execution info
    GLU_LOG_WARNING, // Warning logging, used on recoverable failures
    GLU_LOG_ERROR,   // Error logging, used on unrecoverable failures
    GLU_LOG_FATAL,   // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    GLU_LOG_NONE     // Disable logging
} gluTraceLogLevel;

// Texture pixel formats
// NOTE: Support depends on OpenGL version
typedef enum {
    GLU_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    GLU_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    GLU_PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    GLU_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    GLU_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    GLU_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    GLU_PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    GLU_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    GLU_PIXELFORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    GLU_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    GLU_PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    GLU_PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    GLU_PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    GLU_PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    GLU_PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    GLU_PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    GLU_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    GLU_PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    GLU_PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    GLU_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    GLU_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} gluPixelFormat;

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    GLU_TEXTURE_FILTER_POINT = 0,       // No filter, just pixel approximation
    GLU_TEXTURE_FILTER_BILINEAR,        // Linear filtering
    GLU_TEXTURE_FILTER_TRILINEAR,       // Trilinear filtering (linear with mipmaps)
    GLU_TEXTURE_FILTER_ANISOTROPIC_4X,  // Anisotropic filtering 4x
    GLU_TEXTURE_FILTER_ANISOTROPIC_8X,  // Anisotropic filtering 8x
    GLU_TEXTURE_FILTER_ANISOTROPIC_16X, // Anisotropic filtering 16x
} gluTextureFilter;

// Color blending modes (pre-defined)
typedef enum {
    GLU_BLEND_ALPHA = 0,         // Blend textures considering alpha (default)
    GLU_BLEND_ADDITIVE,          // Blend textures adding colors
    GLU_BLEND_MULTIPLIED,        // Blend textures multiplying colors
    GLU_BLEND_ADD_COLORS,        // Blend textures adding colors (alternative)
    GLU_BLEND_SUBTRACT_COLORS,   // Blend textures subtracting colors (alternative)
    GLU_BLEND_ALPHA_PREMULTIPLY, // Blend premultiplied textures considering alpha
    GLU_BLEND_CUSTOM,            // Blend textures using custom src/dst factors (use gluSetBlendFactors())
    GLU_BLEND_CUSTOM_SEPARATE    // Blend textures using custom src/dst factors (use gluSetBlendFactorsSeparate())
} gluBlendMode;

// Shader location point type
typedef enum {
    GLU_SHADER_LOC_VERTEX_POSITION = 0, // Shader location: vertex attribute: position
    GLU_SHADER_LOC_VERTEX_TEXCOORD01,   // Shader location: vertex attribute: texcoord01
    GLU_SHADER_LOC_VERTEX_TEXCOORD02,   // Shader location: vertex attribute: texcoord02
    GLU_SHADER_LOC_VERTEX_NORMAL,       // Shader location: vertex attribute: normal
    GLU_SHADER_LOC_VERTEX_TANGENT,      // Shader location: vertex attribute: tangent
    GLU_SHADER_LOC_VERTEX_COLOR,        // Shader location: vertex attribute: color
    GLU_SHADER_LOC_MATRIX_MVP,          // Shader location: matrix uniform: model-view-projection
    GLU_SHADER_LOC_MATRIX_VIEW,         // Shader location: matrix uniform: view (camera transform)
    GLU_SHADER_LOC_MATRIX_PROJECTION,   // Shader location: matrix uniform: projection
    GLU_SHADER_LOC_MATRIX_MODEL,        // Shader location: matrix uniform: model (transform)
    GLU_SHADER_LOC_MATRIX_NORMAL,       // Shader location: matrix uniform: normal
    GLU_SHADER_LOC_VECTOR_VIEW,         // Shader location: vector uniform: view
    GLU_SHADER_LOC_COLOR_DIFFUSE,       // Shader location: vector uniform: diffuse color
    GLU_SHADER_LOC_COLOR_SPECULAR,      // Shader location: vector uniform: specular color
    GLU_SHADER_LOC_COLOR_AMBIENT,       // Shader location: vector uniform: ambient color
    GLU_SHADER_LOC_MAP_ALBEDO,          // Shader location: sampler2d texture: albedo (same as: GLU_SHADER_LOC_MAP_DIFFUSE)
    GLU_SHADER_LOC_MAP_METALNESS,       // Shader location: sampler2d texture: metalness (same as: GLU_SHADER_LOC_MAP_SPECULAR)
    GLU_SHADER_LOC_MAP_NORMAL,          // Shader location: sampler2d texture: normal
    GLU_SHADER_LOC_MAP_ROUGHNESS,       // Shader location: sampler2d texture: roughness
    GLU_SHADER_LOC_MAP_OCCLUSION,       // Shader location: sampler2d texture: occlusion
    GLU_SHADER_LOC_MAP_EMISSION,        // Shader location: sampler2d texture: emission
    GLU_SHADER_LOC_MAP_HEIGHT,          // Shader location: sampler2d texture: height
    GLU_SHADER_LOC_MAP_CUBEMAP,         // Shader location: samplerCube texture: cubemap
    GLU_SHADER_LOC_MAP_IRRADIANCE,      // Shader location: samplerCube texture: irradiance
    GLU_SHADER_LOC_MAP_PREFILTER,       // Shader location: samplerCube texture: prefilter
    GLU_SHADER_LOC_MAP_BRDF             // Shader location: sampler2d texture: brdf
} gluShaderLocationIndex;

#define GLU_SHADER_LOC_MAP_DIFFUSE GLU_SHADER_LOC_MAP_ALBEDO
#define GLU_SHADER_LOC_MAP_SPECULAR GLU_SHADER_LOC_MAP_METALNESS

// Shader uniform data type
typedef enum {
    GLU_SHADER_UNIFORM_FLOAT = 0, // Shader uniform type: float
    GLU_SHADER_UNIFORM_VEC2,      // Shader uniform type: vec2 (2 float)
    GLU_SHADER_UNIFORM_VEC3,      // Shader uniform type: vec3 (3 float)
    GLU_SHADER_UNIFORM_VEC4,      // Shader uniform type: vec4 (4 float)
    GLU_SHADER_UNIFORM_INT,       // Shader uniform type: int
    GLU_SHADER_UNIFORM_IVEC2,     // Shader uniform type: ivec2 (2 int)
    GLU_SHADER_UNIFORM_IVEC3,     // Shader uniform type: ivec3 (3 int)
    GLU_SHADER_UNIFORM_IVEC4,     // Shader uniform type: ivec4 (4 int)
    GLU_SHADER_UNIFORM_UINT,      // Shader uniform type: unsigned int
    GLU_SHADER_UNIFORM_UIVEC2,    // Shader uniform type: uivec2 (2 unsigned int)
    GLU_SHADER_UNIFORM_UIVEC3,    // Shader uniform type: uivec3 (3 unsigned int)
    GLU_SHADER_UNIFORM_UIVEC4,    // Shader uniform type: uivec4 (4 unsigned int)
    GLU_SHADER_UNIFORM_SAMPLER2D  // Shader uniform type: sampler2d
} gluShaderUniformDataType;

// Shader attribute data types
typedef enum {
    GLU_SHADER_ATTRIB_FLOAT = 0, // Shader attribute type: float
    GLU_SHADER_ATTRIB_VEC2,      // Shader attribute type: vec2 (2 float)
    GLU_SHADER_ATTRIB_VEC3,      // Shader attribute type: vec3 (3 float)
    GLU_SHADER_ATTRIB_VEC4       // Shader attribute type: vec4 (4 float)
} gluShaderAttributeDataType;

// Framebuffer attachment type
// NOTE: By default up to 8 color channels defined, but it can be more
typedef enum {
    GLU_ATTACHMENT_COLOR_CHANNEL0 = 0, // Framebuffer attachment type: color 0
    GLU_ATTACHMENT_COLOR_CHANNEL1 = 1, // Framebuffer attachment type: color 1
    GLU_ATTACHMENT_COLOR_CHANNEL2 = 2, // Framebuffer attachment type: color 2
    GLU_ATTACHMENT_COLOR_CHANNEL3 = 3, // Framebuffer attachment type: color 3
    GLU_ATTACHMENT_COLOR_CHANNEL4 = 4, // Framebuffer attachment type: color 4
    GLU_ATTACHMENT_COLOR_CHANNEL5 = 5, // Framebuffer attachment type: color 5
    GLU_ATTACHMENT_COLOR_CHANNEL6 = 6, // Framebuffer attachment type: color 6
    GLU_ATTACHMENT_COLOR_CHANNEL7 = 7, // Framebuffer attachment type: color 7
    GLU_ATTACHMENT_DEPTH = 100,        // Framebuffer attachment type: depth
    GLU_ATTACHMENT_STENCIL = 200,      // Framebuffer attachment type: stencil
} gluFramebufferAttachType;

// Framebuffer texture attachment type
typedef enum {
    GLU_ATTACHMENT_CUBEMAP_POSITIVE_X = 0, // Framebuffer texture attachment type: cubemap, +X side
    GLU_ATTACHMENT_CUBEMAP_NEGATIVE_X = 1, // Framebuffer texture attachment type: cubemap, -X side
    GLU_ATTACHMENT_CUBEMAP_POSITIVE_Y = 2, // Framebuffer texture attachment type: cubemap, +Y side
    GLU_ATTACHMENT_CUBEMAP_NEGATIVE_Y = 3, // Framebuffer texture attachment type: cubemap, -Y side
    GLU_ATTACHMENT_CUBEMAP_POSITIVE_Z = 4, // Framebuffer texture attachment type: cubemap, +Z side
    GLU_ATTACHMENT_CUBEMAP_NEGATIVE_Z = 5, // Framebuffer texture attachment type: cubemap, -Z side
    GLU_ATTACHMENT_TEXTURE2D = 100,        // Framebuffer texture attachment type: texture2d
    GLU_ATTACHMENT_RENDERBUFFER = 200,     // Framebuffer texture attachment type: renderbuffer
} gluFramebufferAttachTextureType;

// Face culling mode
typedef enum {
    GLU_CULL_FACE_FRONT = 0,
    GLU_CULL_FACE_BACK
} gluCullMode;

//------------------------------------------------------------------------------------
// Functions Declaration - Matrix operations
    //------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" { // Prevents name mangling of functions
#endif

void gluMatrixMode(int mode);                            // Choose the current matrix to be transformed
void gluPushMatrix(void);                                // Push the current matrix to stack
void gluPopMatrix(void);                                 // Pop latest inserted matrix from stack
void gluLoadIdentity(void);                              // Reset current matrix to identity matrix
void gluTranslatef(float x, float y, float z);           // Multiply the current matrix by a translation matrix
void gluRotatef(float angle, float x, float y, float z); // Multiply the current matrix by a rotation matrix
void gluScalef(float x, float y, float z);               // Multiply the current matrix by a scaling matrix
void gluMultMatrixf(const float *matf);                  // Multiply the current matrix by another matrix
void gluFrustum(double left, double right, double bottom, double top, double znear, double zfar);
void gluOrtho(double left, double right, double bottom, double top, double znear, double zfar);
void gluViewport(int x, int y, int width, int height);    // Set the viewport area
void gluSetClipPlanes(double nearPlane, double farPlane); // Set clip planes distances
double gluGetCullDistanceNear(void);                      // Get cull plane distance near
double gluGetCullDistanceFar(void);                       // Get cull plane distance far

//------------------------------------------------------------------------------------
// Functions Declaration - Vertex level operations
//------------------------------------------------------------------------------------
void gluBegin(int mode);                                                              // Initialize drawing mode (how to organize vertex)
void gluEnd(void);                                                                    // Finish vertex providing
void gluVertex2i(int x, int y);                                                       // Define one vertex (position) - 2 int
void gluVertex2f(float x, float y);                                                   // Define one vertex (position) - 2 float
void gluVertex3f(float x, float y, float z);                                          // Define one vertex (position) - 3 float
void gluTexCoord2f(float x, float y);                                                 // Define one vertex (texture coordinate) - 2 float
void gluNormal3f(float x, float y, float z);                                          // Define one vertex (normal) - 3 float
void gluColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // Define one vertex (color) - 4 byte
void gluColor3f(float x, float y, float z);                                           // Define one vertex (color) - 3 float
void gluColor4f(float x, float y, float z, float w);                                  // Define one vertex (color) - 4 float

//------------------------------------------------------------------------------------
// Functions Declaration - OpenGL style functions (common to 1.1, 3.3+, ES2)
// NOTE: This functions are used to completely abstract raylib code from OpenGL layer,
// some of them are direct wrappers over OpenGL calls, some others are custom
//------------------------------------------------------------------------------------

// Vertex buffers state
bool gluEnableVertexArray(unsigned int vaoId);      // Enable vertex array (VAO, if supported)
void gluDisableVertexArray(void);                   // Disable vertex array (VAO, if supported)
void gluEnableVertexBuffer(unsigned int id);        // Enable vertex buffer (VBO)
void gluDisableVertexBuffer(void);                  // Disable vertex buffer (VBO)
void gluEnableVertexBufferElement(unsigned int id); // Enable vertex buffer element (VBO element)
void gluDisableVertexBufferElement(void);           // Disable vertex buffer element (VBO element)
void gluEnableVertexAttribute(unsigned int index);  // Enable vertex attribute index
void gluDisableVertexAttribute(unsigned int index); // Disable vertex attribute index
#if defined(GRAPHICS_API_OPENGL_11)
void gluEnableStatePointer(int vertexAttribType, void *buffer); // Enable attribute state pointer
void gluDisableStatePointer(int vertexAttribType);              // Disable attribute state pointer
#endif

// Textures state
void gluActiveTextureSlot(int slot);                              // Select and active a texture slot
void gluEnableTexture(unsigned int id);                           // Enable texture
void gluDisableTexture(void);                                     // Disable texture
void gluEnableTextureCubemap(unsigned int id);                    // Enable texture cubemap
void gluDisableTextureCubemap(void);                              // Disable texture cubemap
void gluTextureParameters(unsigned int id, int param, int value); // Set texture parameters (filter, wrap)
void gluCubemapParameters(unsigned int id, int param, int value); // Set cubemap parameters (filter, wrap)

// Shader state
void gluEnableShader(unsigned int id); // Enable shader program
void gluDisableShader(void);           // Disable shader program

// Framebuffer state
void gluEnableFramebuffer(unsigned int id);                                                                                                // Enable render texture (fbo)
void gluDisableFramebuffer(void);                                                                                                          // Disable render texture (fbo), return to default framebuffer
unsigned int gluGetActiveFramebuffer(void);                                                                                                // Get the currently active render texture (fbo), 0 for default framebuffer
void gluActiveDrawBuffers(int count);                                                                                                      // Activate multiple draw color buffers
void gluBlitFramebuffer(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, int bufferMask); // Blit active framebuffer to main framebuffer
void gluBindFramebuffer(unsigned int target, unsigned int framebuffer);                                                                    // Bind framebuffer (FBO)

// General render state
void gluEnableColorBlend(void);                       // Enable color blending
void gluDisableColorBlend(void);                      // Disable color blending
void gluEnableDepthTest(void);                        // Enable depth test
void gluDisableDepthTest(void);                       // Disable depth test
void gluEnableDepthMask(void);                        // Enable depth write
void gluDisableDepthMask(void);                       // Disable depth write
void gluEnableBackfaceCulling(void);                  // Enable backface culling
void gluDisableBackfaceCulling(void);                 // Disable backface culling
void gluColorMask(bool r, bool g, bool b, bool a);    // Color mask control
void gluSetCullFace(int mode);                        // Set face culling mode
void gluEnableScissorTest(void);                      // Enable scissor test
void gluDisableScissorTest(void);                     // Disable scissor test
void gluScissor(int x, int y, int width, int height); // Scissor test
void gluEnableWireMode(void);                         // Enable wire mode
void gluEnablePointMode(void);                        // Enable point mode
void gluDisableWireMode(void);                        // Disable wire (and point) mode
void gluSetLineWidth(float width);                    // Set the line drawing width
float gluGetLineWidth(void);                          // Get the line drawing width
void gluEnableSmoothLines(void);                      // Enable line aliasing
void gluDisableSmoothLines(void);                     // Disable line aliasing
void gluEnableStereoRender(void);                     // Enable stereo rendering
void gluDisableStereoRender(void);                    // Disable stereo rendering
bool gluIsStereoRenderEnabled(void);                  // Check if stereo render is enabled

void gluClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);                                  // Clear color buffer with color
void gluClearScreenBuffers(void);                                                                                        // Clear used screen buffers (color and depth)
void gluCheckErrors(void);                                                                                               // Check and log OpenGL error codes
void gluSetBlendMode(int mode);                                                                                          // Set blending mode
void gluSetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation);                                               // Set blending mode factor and equation (using OpenGL factors)
void gluSetBlendFactorsSeparate(int glSrcRGB, int glDstRGB, int glSrcAlpha, int glDstAlpha, int glEqRGB, int glEqAlpha); // Set blending mode factors and equations separately (using OpenGL factors)

//------------------------------------------------------------------------------------
// Functions Declaration - glu functionality
//------------------------------------------------------------------------------------
// glu initialization functions
void gluInit(int width, int height);    // Initialize glu (buffers, shaders, textures, states)
void gluClose(void);                    // De-initialize glu (buffers, shaders, textures)
void gluLoadExtensions(void *loader);     // Load OpenGL extensions (loader function required)
int gluGetVersion(void);                  // Get current OpenGL version
void gluSetFramebufferWidth(int width);   // Set current framebuffer width
int gluGetFramebufferWidth(void);         // Get default framebuffer width
void gluSetFramebufferHeight(int height); // Set current framebuffer height
int gluGetFramebufferHeight(void);        // Get default framebuffer height

unsigned int gluGetTextureIdDefault(void); // Get default texture id
unsigned int gluGetShaderIdDefault(void);  // Get default shader id
int *rlGetShaderLocsDefault(void);        // Get default shader locations

// Render batch management
// NOTE: glu provides a default render batch to behave like OpenGL 1.1 immediate mode
// but this render batch API is exposed in case of custom batches are required
 gluRenderBatch gluLoadRenderBatch(int numBuffers, int bufferElements); // Load a render batch system
void gluUnloadRenderBatch( gluRenderBatch batch);                       // Unload render batch system
void gluDrawRenderBatch( gluRenderBatch *batch);                        // Draw render batch data (Update->Draw->Reset)
void gluSetRenderBatchActive( gluRenderBatch *batch);                   // Set the active render batch for glu (NULL for default internal)
void gluDrawRenderBatchActive(void);                                  // Update and draw internal render batch
bool gluCheckRenderBatchLimit(int vCount);                            // Check internal buffer overflow for a given number of vertex

void gluSetTexture(unsigned int id); // Set current texture for render batch and check buffers limits

// Vertex buffers management
unsigned int gluLoadVertexArray(void);                                                                           // Load vertex array (vao) if supported
unsigned int gluLoadVertexBuffer(const void *buffer, int size, bool dynamic);                                    // Load a vertex buffer object
unsigned int gluLoadVertexBufferElement(const void *buffer, int size, bool dynamic);                             // Load vertex buffer elements object
void gluUpdateVertexBuffer(unsigned int bufferId, const void *data, int dataSize, int offset);                   // Update vertex buffer object data on GPU buffer
void gluUpdateVertexBufferElements(unsigned int id, const void *data, int dataSize, int offset);                 // Update vertex buffer elements data on GPU buffer
void gluUnloadVertexArray(unsigned int vaoId);                                                                   // Unload vertex array (vao)
void gluUnloadVertexBuffer(unsigned int vboId);                                                                  // Unload vertex buffer object
void gluSetVertexAttribute(unsigned int index, int compSize, int type, bool normalized, int stride, int offset); // Set vertex attribute data configuration
void gluSetVertexAttributeDivisor(unsigned int index, int divisor);                                              // Set vertex attribute data divisor
void gluSetVertexAttributeDefault(int locIndex, const void *value, int attribType, int count);                   // Set vertex attribute default value, when attribute to provided
void gluDrawVertexArray(int offset, int count);                                                                  // Draw vertex array (currently active vao)
void gluDrawVertexArrayElements(int offset, int count, const void *buffer);                                      // Draw vertex array elements
void gluDrawVertexArrayInstanced(int offset, int count, int instances);                                          // Draw vertex array (currently active vao) with instancing
void gluDrawVertexArrayElementsInstanced(int offset, int count, const void *buffer, int instances);              // Draw vertex array elements with instancing

// Textures management
unsigned int gluLoadTexture(const void *data, int width, int height, int format, int mipmapCount);                     // Load texture data
unsigned int gluLoadTextureDepth(int width, int height, bool useRenderBuffer);                                         // Load depth texture/renderbuffer (to be attached to fbo)
unsigned int gluLoadTextureCubemap(const void *data, int size, int format);                                            // Load texture cubemap data
void gluUpdateTexture(unsigned int id, int offsetX, int offsetY, int width, int height, int format, const void *data); // Update texture with new data on GPU
void gluGetGlTextureFormats(int format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType); // Get OpenGL internal formats
const char *rlGetPixelFormatName(unsigned int format);                                                                // Get name string for pixel format
void gluUnloadTexture(unsigned int id);                                                                                // Unload texture from GPU memory
void gluGenTextureMipmaps(unsigned int id, int width, int height, int format, int *mipmaps);                           // Generate mipmap data for selected texture
void *rlReadTexturePixels(unsigned int id, int width, int height, int format);                                        // Read texture pixel data
unsigned char *rlReadScreenPixels(int width, int height);                                                             // Read screen pixel data (color buffer)

// Framebuffer management (fbo)
unsigned int gluLoadFramebuffer(void);                                                                        // Load an empty framebuffer
void gluFramebufferAttach(unsigned int fboId, unsigned int texId, int attachType, int texType, int mipLevel); // Attach texture/renderbuffer to a framebuffer
bool gluFramebufferComplete(unsigned int id);                                                                 // Verify framebuffer is complete
void gluUnloadFramebuffer(unsigned int id);                                                                   // Delete framebuffer from GPU

// Shaders management
unsigned int gluLoadShaderCode(const char *vsCode, const char *fsCode);            // Load shader from code strings
unsigned int gluCompileShader(const char *shaderCode, int type);                   // Compile custom shader and return shader id (type: GLU_VERTEX_SHADER, GLU_FRAGMENT_SHADER, GLU_COMPUTE_SHADER)
unsigned int gluLoadShaderProgram(unsigned int vShaderId, unsigned int fShaderId); // Load custom shader program
void gluUnloadShaderProgram(unsigned int id);                                      // Unload shader program
int gluGetLocationUniform(unsigned int shaderId, const char *uniformName);         // Get shader location uniform
int gluGetLocationAttrib(unsigned int shaderId, const char *attribName);           // Get shader location attribute
void gluSetUniform(int locIndex, const void *value, int uniformType, int count);   // Set shader value uniform
void gluSetUniformMatrix(int locIndex, Matrix mat);                                // Set shader value matrix
void gluSetUniformSampler(int locIndex, unsigned int textureId);                   // Set shader value sampler
void gluSetShader(unsigned int id, int *locs);                                     // Set shader currently active (id and locations)

// Compute shader management
unsigned int gluLoadComputeShaderProgram(unsigned int shaderId);                              // Load compute shader program
void gluComputeShaderDispatch(unsigned int groupX, unsigned int groupY, unsigned int groupZ); // Dispatch compute shader (equivalent to *draw* for graphics pipeline)

// Shader buffer storage object management (ssbo)
unsigned int gluLoadShaderBuffer(unsigned int size, const void *data, int usageHint);                                                   // Load shader storage buffer object (SSBO)
void gluUnloadShaderBuffer(unsigned int ssboId);                                                                                        // Unload shader storage buffer object (SSBO)
void gluUpdateShaderBuffer(unsigned int id, const void *data, unsigned int dataSize, unsigned int offset);                              // Update SSBO buffer data
void gluBindShaderBuffer(unsigned int id, unsigned int index);                                                                          // Bind SSBO buffer
void gluReadShaderBuffer(unsigned int id, void *dest, unsigned int count, unsigned int offset);                                         // Read SSBO buffer data (GPU->CPU)
void gluCopyShaderBuffer(unsigned int destId, unsigned int srcId, unsigned int destOffset, unsigned int srcOffset, unsigned int count); // Copy SSBO data between buffers
unsigned int gluGetShaderBufferSize(unsigned int id);                                                                                   // Get SSBO buffer size

// Buffer management
void gluBindImageTexture(unsigned int id, unsigned int index, int format, bool readonly); // Bind image texture

// Matrix state management
Matrix gluGetMatrixModelview(void);                           // Get internal modelview matrix
Matrix gluGetMatrixProjection(void);                          // Get internal projection matrix
Matrix gluGetMatrixTransform(void);                           // Get internal accumulated transform matrix
Matrix gluGetMatrixProjectionStereo(int eye);                 // Get internal projection matrix for stereo render (selected eye)
Matrix gluGetMatrixViewOffsetStereo(int eye);                 // Get internal view offset matrix for stereo render (selected eye)
void gluSetMatrixProjection(Matrix proj);                     // Set a custom projection matrix (replaces internal projection matrix)
void gluSetMatrixModelview(Matrix view);                      // Set a custom modelview matrix (replaces internal modelview matrix)
void gluSetMatrixProjectionStereo(Matrix right, Matrix left); // Set eyes projection matrices for stereo rendering
void gluSetMatrixViewOffsetStereo(Matrix right, Matrix left); // Set eyes view offsets matrices for stereo rendering

// Quick and dirty cube/quad buffers load->draw->unload
void gluLoadDrawCube(void); // Load and draw a cube
void gluLoadDrawQuad(void); // Load and draw a quad

#if defined(__cplusplus)
    }
#endif
#endif /* glu_h */
