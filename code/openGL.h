#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\glfw3.h>

#include "clag.h"
#include "stb_image.h"

/*** OpenGL 1st Abstraction Interface
 * Vertex Buffer : Interface to VRAM vertex buffers
 * Index Buffer : Interface to VRAM index buffers
 * Vertex Array Object : Interface to OpenGL vertex array object
 * VideoShader : Interface to OpenGL video shaders (vertex/fragment)
 * ImageTexture : Interface to OpenGL image shaders
 * Window : GLFW window controller
 */

enum imgType {jpg= 0, png= 1};
extern FILE* stream;                                                    // All error messages that occur in OpenGL are sent to this file (often stdout/stderr)

/* Vertex Buffer ******************************************************/
typedef struct {
      GLuint vertexBufferRef;
      int size;
} VertexBuffer;
void vertexBuffer_init(VertexBuffer* self, const float* vertices, int numVerts, int sizeOfVerts);
void vertexBuffer_bind(VertexBuffer* self);
void vertexBuffer_delete(VertexBuffer* self);
void vertexBuffer_rebufferData(VertexBuffer* self, float* vertices, int numVerts, int sizeOfVerts);
/* Vertex Buffer ******************************************************/

/* Index Buffer *******************************************************/
typedef struct {
      GLuint indexBufferRef;
      int length;
} IndexBuffer;
void indexBuffer_init(IndexBuffer* self, const unsigned int* indices, int numIndices);
void indexBuffer_bind(IndexBuffer* self);
void indexBuffer_delete(IndexBuffer* self);
void indexBuffer_rebufferData(IndexBuffer* self, unsigned int* indices);
/* Index Buffer *******************************************************/

/* Vertex Array Object ************************************************/
typedef struct {
      GLuint vaoRef;
      int locations;
      int vertexLength;
      int offset;
} VertexArrayObject;
void vertexArrayObject_init(VertexArrayObject* self, int vertexLength);
void vertexArrayObject_bind(VertexArrayObject* self);
void vertexArrayObject_delete(VertexArrayObject* self);
void vertexArrayObject_bindVertexBuffer(VertexArrayObject* self, VertexBuffer* vb);
void vertexArrayObject_addElement(VertexArrayObject* self, int length);
/* Vertex Array Object ************************************************/

/* VideoShader ********************************************************/
typedef struct {
      const char* name;
      GLuint programID;
      const char* vertexShaderPath;
      const char* fragmentShaderPath;
} VideoShader;
bool videoShader_load(VideoShader* self, const char* vertexShaderPath, const char* fragmentShaderPath, const char* name);
void videoShader_bind(VideoShader* self);
void videoShader_delete(VideoShader* self);
bool videoShader_setUniform_int(VideoShader* self, const char* name, int i);
bool videoShader_setUniform_vec2(VideoShader* self, const char* name, vec2* vec);
bool videoShader_setUniform_vec3(VideoShader* self, const char* name, vec3* vec);
bool videoShader_setUniform_vec4(VideoShader* self, const char* name, vec4* vec);
bool videoShader_setUniform_mat4(VideoShader* self, const char* name, mat4* mat);
/* VideoShader ********************************************************/

/* ImageTexture *******************************************************/
typedef struct {
      const char* name;
      GLuint textureID;
      const char* texturePath;
      int width;
      int height;
      int nrChannels;
} ImageTexture;
bool imageTexture_loadimage(ImageTexture* self, const char* texturePath, const char* name, unsigned int slot, int type);
void imageTexture_bind(ImageTexture* self, unsigned int slot);
void imageTexture_delete(ImageTexture* self);
/* ImageTexture *******************************************************/

/* Window *************************************************************/
typedef struct {
      const char* name;
      GLFWwindow* window;
      unsigned int width;
      unsigned int height;
      float aspect;
      int mode;
} Window;
bool window_open(Window* self, const char* name, unsigned int width, unsigned int height);
void window_bind(Window* self);
void window_delete(Window* self);
bool window_setIcon(Window* self, const char* path);
bool window_update(Window* self);
/* Window *************************************************************/
