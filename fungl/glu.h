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
// TODO: Instead of #error, enable pass-thru mode for older versions of OpenGL
#if FUNGL_VERSION < GL_VERSION_3_2
#error "glu.h requires FUNGL_VERSION >= GL_VERSION_3_2"
#endif
#include "gl.h"
#include "bla/bla/bla.h"

// Texture parameters (equivalent to OpenGL defines)
#define GLU_TEXTURE_WRAP_S                       0x2802                         // GL_TEXTURE_WRAP_S
#define GLU_TEXTURE_WRAP_T                       0x2803                         // GL_TEXTURE_WRAP_T
#define GLU_TEXTURE_MAG_FILTER                   0x2800                         // GL_TEXTURE_MAG_FILTER
#define GLU_TEXTURE_MIN_FILTER                   0x2801                         // GL_TEXTURE_MIN_FILTER

#define GLU_TEXTURE_FILTER_NEAREST               0x2600                         // GL_NEAREST
#define GLU_TEXTURE_FILTER_LINEAR                0x2601                         // GL_LINEAR
#define GLU_TEXTURE_FILTER_MIP_NEAREST           0x2700                         // GL_NEAREST_MIPMAP_NEAREST
#define GLU_TEXTURE_FILTER_NEAREST_MIP_LINEAR    0x2702                         // GL_NEAREST_MIPMAP_LINEAR
#define GLU_TEXTURE_FILTER_LINEAR_MIP_NEAREST    0x2701                         // GL_LINEAR_MIPMAP_NEAREST
#define GLU_TEXTURE_FILTER_MIP_LINEAR            0x2703                         // GL_LINEAR_MIPMAP_LINEAR
#define GLU_TEXTURE_FILTER_ANISOTROPIC           0x3000                         // Anisotropic filter (custom identifier)
#define GLU_TEXTURE_MIPMAP_BIAS_RATIO            0x4000                         // Texture mipmap bias, percentage ratio (custom identifier)

#define GLU_TEXTURE_WRAP_REPEAT                  0x2901                         // GL_REPEAT
#define GLU_TEXTURE_WRAP_CLAMP                   0x812F                         // GL_CLAMP_TO_EDGE
#define GLU_TEXTURE_WRAP_MIRROR_REPEAT           0x8370                         // GL_MIRRORED_REPEAT
#define GLU_TEXTURE_WRAP_MIRROR_CLAMP            0x8742                         // GL_MIRROR_CLAMP_EXT

// mat4 modes (equivalent to OpenGL)
#define GLU_MODELVIEW                            0x1700                         // GL_MODELVIEW
#define GLU_PROJECTION                           0x1701                         // GL_PROJECTION
#define GLU_TEXTURE                              0x1702                         // GL_TEXTURE

// Primitive assembly draw modes
#define GLU_LINES                                0x0001                         // GL_LINES
#define GLU_TRIANGLES                            0x0004                         // GL_TRIANGLES
#define GLU_QUADS                                0x0007                         // GL_QUADS

// GL equivalent data types
#define GLU_UNSIGNED_BYTE                        0x1401                         // GL_UNSIGNED_BYTE
#define GLU_FLOAT                                0x1406                         // GL_FLOAT

// GL buffer usage hint
#define GLU_STREAM_DRAW                          0x88E0                         // GL_STREAM_DRAW
#define GLU_STREAM_READ                          0x88E1                         // GL_STREAM_READ
#define GLU_STREAM_COPY                          0x88E2                         // GL_STREAM_COPY
#define GLU_STATIC_DRAW                          0x88E4                         // GL_STATIC_DRAW
#define GLU_STATIC_READ                          0x88E5                         // GL_STATIC_READ
#define GLU_STATIC_COPY                          0x88E6                         // GL_STATIC_COPY
#define GLU_DYNAMIC_DRAW                         0x88E8                         // GL_DYNAMIC_DRAW
#define GLU_DYNAMIC_READ                         0x88E9                         // GL_DYNAMIC_READ
#define GLU_DYNAMIC_COPY                         0x88EA                         // GL_DYNAMIC_COPY

// GL Shader type
#define GLU_FRAGMENT_SHADER                      0x8B30                         // GL_FRAGMENT_SHADER
#define GLU_VERTEX_SHADER                        0x8B31                         // GL_VERTEX_SHADER
#define GLU_COMPUTE_SHADER                       0x91B9                         // GL_COMPUTE_SHADER

// GL blending factors
#define GLU_ZERO                                 0                              // GL_ZERO
#define GLU_ONE                                  1                              // GL_ONE
#define GLU_SRC_COLOR                            0x0300                         // GL_SRC_COLOR
#define GLU_ONE_MINUS_SRC_COLOR                  0x0301                         // GL_ONE_MINUS_SRC_COLOR
#define GLU_SRC_ALPHA                            0x0302                         // GL_SRC_ALPHA
#define GLU_ONE_MINUS_SRC_ALPHA                  0x0303                         // GL_ONE_MINUS_SRC_ALPHA
#define GLU_DST_ALPHA                            0x0304                         // GL_DST_ALPHA
#define GLU_ONE_MINUS_DST_ALPHA                  0x0305                         // GL_ONE_MINUS_DST_ALPHA
#define GLU_DST_COLOR                            0x0306                         // GL_DST_COLOR
#define GLU_ONE_MINUS_DST_COLOR                  0x0307                         // GL_ONE_MINUS_DST_COLOR
#define GLU_SRC_ALPHA_SATURATE                   0x0308                         // GL_SRC_ALPHA_SATURATE
#define GLU_CONSTANT_COLOR                       0x8001                         // GL_CONSTANT_COLOR
#define GLU_ONE_MINUS_CONSTANT_COLOR             0x8002                         // GL_ONE_MINUS_CONSTANT_COLOR
#define GLU_CONSTANT_ALPHA                       0x8003                         // GL_CONSTANT_ALPHA
#define GLU_ONE_MINUS_CONSTANT_ALPHA             0x8004                         // GL_ONE_MINUS_CONSTANT_ALPHA

// GL blending functions/equations
#define GLU_FUNC_ADD                             0x8006                         // GL_FUNC_ADD
#define GLU_MIN                                  0x8007                         // GL_MIN
#define GLU_MAX                                  0x8008                         // GL_MAX
#define GLU_FUNC_SUBTRACT                        0x800A                         // GL_FUNC_SUBTRACT
#define GLU_FUNC_REVERSE_SUBTRACT                0x800B                         // GL_FUNC_REVERSE_SUBTRACT
#define GLU_BLEND_EQUATION                       0x8009                         // GL_BLEND_EQUATION
#define GLU_BLEND_EQUATION_RGB                   0x8009                         // GL_BLEND_EQUATION_RGB   // (Same as BLEND_EQUATION)
#define GLU_BLEND_EQUATION_ALPHA                 0x883D                         // GL_BLEND_EQUATION_ALPHA
#define GLU_BLEND_DST_RGB                        0x80C8                         // GL_BLEND_DST_RGB
#define GLU_BLEND_SRC_RGB                        0x80C9                         // GL_BLEND_SRC_RGB
#define GLU_BLEND_DST_ALPHA                      0x80CA                         // GL_BLEND_DST_ALPHA
#define GLU_BLEND_SRC_ALPHA                      0x80CB                         // GL_BLEND_SRC_ALPHA
#define GLU_BLEND_COLOR                          0x8005                         // GL_BLEND_COLOR

#define GLU_READ_FRAMEBUFFER                     0x8CA8                         // GL_READ_FRAMEBUFFER
#define GLU_DRAW_FRAMEBUFFER                     0x8CA9                         // GL_DRAW_FRAMEBUFFER

typedef struct gluRenderBatch gluRenderBatch;

void gluInit(int width, int height);                                            // Initialize glu (buffers, shaders, textures, states)
void gluClose(void);                                                            // De-initialize glu (buffers, shaders, textures)
void gluLoadExtensions(void *loader);                                           // Load OpenGL extensions (loader function required)
int gluGetVersion(void);                                                        // Get current OpenGL version
void gluSetFramebufferWidth(int width);                                         // Set current framebuffer width
int gluGetFramebufferWidth(void);                                               // Get default framebuffer width
void gluSetFramebufferHeight(int height);                                       // Set current framebuffer height
int gluGetFramebufferHeight(void);                                              // Get default framebuffer height

void gluMatrixMode(int mode);                                                   // Choose the current mat4 to be transformed
void gluPushMatrix(void);                                                       // Push the current mat4 to stack
void gluPopMatrix(void);                                                        // Pop latest inserted mat4 from stack
void gluLoadIdentity(void);                                                     // Reset current mat4 to identity mat4
void gluTranslatef(float x, float y, float z);                                  // Multiply the current mat4 by a translation mat4
void gluRotatef(float angle, float x, float y, float z);                        // Multiply the current mat4 by a rotation mat4
void gluScalef(float x, float y, float z);                                      // Multiply the current mat4 by a scaling mat4
void gluMultMatrix(const float *matf);                                          // Multiply the current mat4 by another mat4
void gluFrustum(double left, double right, double bottom, double top, double znear, double zfar);
void gluOrtho(double left, double right, double bottom, double top, double znear, double zfar);
void gluViewport(int x, int y, int width, int height);                          // Set the viewport area
void gluSetClipPlanes(double nearPlane, double farPlane);                       // Set clip planes distances
double gluGetCullDistanceNear(void);                                            // Get cull plane distance near
double gluGetCullDistanceFar(void);                                             // Get cull plane distance far

void gluBegin(int mode);                                                        // Initialize drawing mode (how to organize vertex)
void gluEnd(void);                                                              // Finish vertex providing
void gluVertex2i(int x, int y);                                                 // Define one vertex (position) - 2 int
void gluVertex2f(float x, float y);                                             // Define one vertex (position) - 2 float
void gluVertex3f(float x, float y, float z);                                    // Define one vertex (position) - 3 float
void gluTexCoord2f(float x, float y);                                           // Define one vertex (texture coordinate) - 2 float
void gluNormal3f(float x, float y, float z);                                    // Define one vertex (normal) - 3 float
void gluColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // Define one vertex (color) - 4 byte
void gluColor3f(float x, float y, float z);                                     // Define one vertex (color) - 3 float
void gluColor4f(float x, float y, float z, float w);                            // Define one vertex (color) - 4 float

int gluEnableVertexArray(unsigned int vaoId);                                   // Enable vertex array (VAO, if supported)
void gluDisableVertexArray(void);                                               // Disable vertex array (VAO, if supported)
void gluEnableVertexBuffer(unsigned int id);                                    // Enable vertex buffer (VBO)
void gluDisableVertexBuffer(void);                                              // Disable vertex buffer (VBO)
void gluEnableVertexBufferElement(unsigned int id);                             // Enable vertex buffer element (VBO element)
void gluDisableVertexBufferElement(void);                                       // Disable vertex buffer element (VBO element)
void gluEnableVertexAttribute(unsigned int index);                              // Enable vertex attribute index
void gluDisableVertexAttribute(unsigned int index);                             // Disable vertex attribute index

void gluActiveTextureSlot(int slot);                                            // Select and active a texture slot
void gluEnableTexture(unsigned int id);                                         // Enable texture
void gluDisableTexture(void);                                                   // Disable texture
void gluEnableTextureCubemap(unsigned int id);                                  // Enable texture cubemap
void gluDisableTextureCubemap(void);                                            // Disable texture cubemap
void gluTextureParameters(unsigned int id, int param, int value);               // Set texture parameters (filter, wrap)
void gluCubemapParameters(unsigned int id, int param, int value);               // Set cubemap parameters (filter, wrap)

void gluEnableShader(unsigned int id);                                          // Enable shader program
void gluDisableShader(void);                                                    // Disable shader program

void gluEnableFramebuffer(unsigned int id);                                     // Enable render texture (fbo)
void gluDisableFramebuffer(void);                                               // Disable render texture (fbo), return to default framebuffer
unsigned int gluGetActiveFramebuffer(void);                                     // Get the currently active render texture (fbo), 0 for default framebuffer
void gluActiveDrawBuffers(int count);                                           // Activate multiple draw color buffers
void gluBlitFramebuffer(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, int bufferMask); // Blit active framebuffer to main framebuffer
void gluBindFramebuffer(unsigned int target, unsigned int framebuffer);         // Bind framebuffer (FBO)

void gluEnableColorBlend(void);                                                 // Enable color blending
void gluDisableColorBlend(void);                                                // Disable color blending
void gluEnableDepthTest(void);                                                  // Enable depth test
void gluDisableDepthTest(void);                                                 // Disable depth test
void gluEnableDepthMask(void);                                                  // Enable depth write
void gluDisableDepthMask(void);                                                 // Disable depth write
void gluEnableBackfaceCulling(void);                                            // Enable backface culling
void gluDisableBackfaceCulling(void);                                           // Disable backface culling
void gluColorMask(int r, int g, int b, int a);                                  // Color mask control
void gluSetCullFace(int mode);                                                  // Set face culling mode
void gluEnableScissorTest(void);                                                // Enable scissor test
void gluDisableScissorTest(void);                                               // Disable scissor test
void gluScissor(int x, int y, int width, int height);                           // Scissor test
void gluEnableWireMode(void);                                                   // Enable wire mode
void gluEnablePointMode(void);                                                  // Enable point mode
void gluDisableWireMode(void);                                                  // Disable wire (and point) mode
void gluSetLineWidth(float width);                                              // Set the line drawing width
float gluGetLineWidth(void);                                                    // Get the line drawing width
void gluEnableSmoothLines(void);                                                // Enable line aliasing
void gluDisableSmoothLines(void);                                               // Disable line aliasing
void gluEnableStereoRender(void);                                               // Enable stereo rendering
void gluDisableStereoRender(void);                                              // Disable stereo rendering
int gluIsStereoRenderEnabled(void);                                             // Check if stereo render is enabled

void gluClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // Clear color buffer with color
void gluClearScreenBuffers(void);                                               // Clear used screen buffers (color and depth)
void gluCheckErrors(void);                                                      // Check and log OpenGL error codes
void gluSetBlendMode(int mode);                                                 // Set blending mode
void gluSetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation);      // Set blending mode factor and equation (using OpenGL factors)
void gluSetBlendFactorsSeparate(int glSrcRGB, int glDstRGB, int glSrcAlpha, int glDstAlpha, int glEqRGB, int glEqAlpha); // Set blending mode factors and equations separately (using OpenGL factors)

unsigned int gluGetTextureIdDefault(void);                                      // Get default texture id
unsigned int gluGetShaderIdDefault(void);                                       // Get default shader id
int *gluGetShaderLocsDefault(void);                                             // Get default shader locations

// Render batch management
// NOTE: glu provides a default render batch to behave like OpenGL 1.1 immediate mode
// but this render batch API is exposed in case of custom batches are required
gluRenderBatch gluLoadRenderBatch(int numBuffers, int bufferElements);          // Load a render batch system
void gluUnloadRenderBatch(gluRenderBatch batch);                                // Unload render batch system
void gluDrawRenderBatch(gluRenderBatch *batch);                                 // Draw render batch data (Update->Draw->Reset)
void gluSetRenderBatchActive(gluRenderBatch *batch);                            // Set the active render batch for glu (NULL for default internal)
void gluDrawRenderBatchActive(void);                                            // Update and draw internal render batch
int gluCheckRenderBatchLimit(int vCount);                                       // Check internal buffer overflow for a given number of vertex

void gluSetTexture(unsigned int id);                                            // Set current texture for render batch and check buffers limits

// Vertex buffers management
unsigned int gluLoadVertexArray(void);                                          // Load vertex array (vao) if supported
unsigned int gluLoadVertexBuffer(const void *buffer, int size, int dynamic);    // Load a vertex buffer object
unsigned int gluLoadVertexBufferElement(const void *buffer, int size, int dynamic); // Load vertex buffer elements object
void gluUpdateVertexBuffer(unsigned int bufferId, const void *data, int dataSize, int offset); // Update vertex buffer object data on GPU buffer
void gluUpdateVertexBufferElements(unsigned int id, const void *data, int dataSize, int offset); // Update vertex buffer elements data on GPU buffer
void gluUnloadVertexArray(unsigned int vaoId);                                  // Unload vertex array (vao)
void gluUnloadVertexBuffer(unsigned int vboId);                                 // Unload vertex buffer object
void gluSetVertexAttribute(unsigned int index, int compSize, int type, int normalized, int stride, int offset); // Set vertex attribute data configuration
void gluSetVertexAttributeDivisor(unsigned int index, int divisor);             // Set vertex attribute data divisor
void gluSetVertexAttributeDefault(int locIndex, const void *value, int attribType, int count); // Set vertex attribute default value, when attribute to provided
void gluDrawVertexArray(int offset, int count);                                 // Draw vertex array (currently active vao)
void gluDrawVertexArrayElements(int offset, int count, const void *buffer);     // Draw vertex array elements
void gluDrawVertexArrayInstanced(int offset, int count, int instances);         // Draw vertex array (currently active vao) with instancing
void gluDrawVertexArrayElementsInstanced(int offset, int count, const void *buffer, int instances); // Draw vertex array elements with instancing

// Textures management
unsigned int gluLoadTexture(const void *data, int width, int height, int format, int mipmapCount); // Load texture data
unsigned int gluLoadTextureDepth(int width, int height, int useRenderBuffer);   // Load depth texture/renderbuffer (to be attached to fbo)
unsigned int gluLoadTextureCubemap(const void *data, int size, int format);     // Load texture cubemap data
void gluUpdateTexture(unsigned int id, int offsetX, int offsetY, int width, int height, int format, const void *data); // Update texture with new data on GPU
void gluGetGlTextureFormats(int format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType); // Get OpenGL internal formats
const char *gluGetPixelFormatName(unsigned int format);                         // Get name string for pixel format
void gluUnloadTexture(unsigned int id);                                         // Unload texture from GPU memory
void gluGenTextureMipmaps(unsigned int id, int width, int height, int format, int *mipmaps); // Generate mipmap data for selected texture
void *gluReadTexturePixels(unsigned int id, int width, int height, int format); // Read texture pixel data
unsigned char *gluReadScreenPixels(int width, int height);                      // Read screen pixel data (color buffer)

// Framebuffer management (fbo)
unsigned int gluLoadFramebuffer(void);                                          // Load an empty framebuffer
void gluFramebufferAttach(unsigned int fboId, unsigned int texId, int attachType, int texType, int mipLevel); // Attach texture/renderbuffer to a framebuffer
int gluFramebufferComplete(unsigned int id);                                    // Verify framebuffer is complete
void gluUnloadFramebuffer(unsigned int id);                                     // Delete framebuffer from GPU

// Shaders management
unsigned int gluLoadShaderCode(const char *vsCode, const char *fsCode);         // Load shader from code strings
unsigned int gluCompileShader(const char *shaderCode, int type);                // Compile custom shader and return shader id (type: RL_VERTEX_SHADER, RL_FRAGMENT_SHADER, RL_COMPUTE_SHADER)
unsigned int gluLoadShaderProgram(unsigned int vShaderId, unsigned int fShaderId); // Load custom shader program
void gluUnloadShaderProgram(unsigned int id);                                   // Unload shader program
int gluGetLocationUniform(unsigned int shaderId, const char *uniformName);      // Get shader location uniform
int gluGetLocationAttrib(unsigned int shaderId, const char *attribName);        // Get shader location attribute
void gluSetUniform(int locIndex, const void *value, int uniformType, int count); // Set shader value uniform
void gluSetUniformMatrix(int locIndex, mat4 mat);                               // Set shader value mat4
void gluSetUniformSampler(int locIndex, unsigned int textureId);                // Set shader value sampler
void gluSetShader(unsigned int id, int *locs);                                  // Set shader currently active (id and locations)

// Compute shader management
unsigned int gluLoadComputeShaderProgram(unsigned int shaderId);                // Load compute shader program
void gluComputeShaderDispatch(unsigned int groupX, unsigned int groupY, unsigned int groupZ); // Dispatch compute shader (equivalent to *draw* for graphics pipeline)

// Shader buffer storage object management (ssbo)
unsigned int gluLoadShaderBuffer(unsigned int size, const void *data, int usageHint); // Load shader storage buffer object (SSBO)
void gluUnloadShaderBuffer(unsigned int ssboId);                                // Unload shader storage buffer object (SSBO)
void gluUpdateShaderBuffer(unsigned int id, const void *data, unsigned int dataSize, unsigned int offset); // Update SSBO buffer data
void gluBindShaderBuffer(unsigned int id, unsigned int index);                  // Bind SSBO buffer
void gluReadShaderBuffer(unsigned int id, void *dest, unsigned int count, unsigned int offset); // Read SSBO buffer data (GPU->CPU)
void gluCopyShaderBuffer(unsigned int destId, unsigned int srcId, unsigned int destOffset, unsigned int srcOffset, unsigned int count); // Copy SSBO data between buffers
unsigned int gluGetShaderBufferSize(unsigned int id);                           // Get SSBO buffer size

// Buffer management
void gluBindImageTexture(unsigned int id, unsigned int index, int format, int readonly); // Bind image texture

// Matrix state management
mat4 gluGetMatrixModelview(void);                                               // Get internal modelview mat4
mat4 gluGetMatrixProjection(void);                                              // Get internal projection mat4
mat4 gluGetMatrixTransform(void);                                               // Get internal accumulated transform mat4
mat4 gluGetMatrixProjectionStereo(int eye);                                     // Get internal projection mat4 for stereo render (selected eye)
mat4 gluGetMatrixViewOffsetStereo(int eye);                                     // Get internal view offset mat4 for stereo render (selected eye)
void gluSetMatrixProjection(mat4 proj);                                         // Set a custom projection mat4 (replaces internal projection mat4)
void gluSetMatrixModelview(mat4 view);                                          // Set a custom modelview mat4 (replaces internal modelview mat4)
void gluSetMatrixProjectionStereo(mat4 right, mat4 left);                       // Set eyes projection matrices for stereo rendering
void gluSetMatrixViewOffsetStereo(mat4 right, mat4 left);                       // Set eyes view offsets matrices for stereo rendering

#if defined(__cplusplus)
}
#endif
#endif /* glu_h */
