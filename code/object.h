#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "list.h"
#include "map.h"
#include "clag.h"
#include "openGL.h"

/* Camera *************************************************************/
typedef struct {
      const char* ID;
      float sensitivity;
      float speed;
      float nearZ;
      float farZ;
      float fov;
      float horizontalAngle;
      float verticalAngle;
      vec3 position;
      vec3 direction;
      vec3 target;
      vec3 up;
      mat4 view;
      mat4 projection;
      // Contains a bunch of references to window information
      Window* windowRef;
      unsigned int* width;
      unsigned int* height;
      float* aspect;
      int* winMode;
} Camera;
void camera_start(Camera* self, Window* w);
void camera_init(Camera* self, float sensitivity, float speed, float nearZ, float farZ, float fov, vec3* position, vec3* direction, const char* ID);
void camera_update(Camera* self, double deltaTime);                     // Basically constitutes the camera controller
void camera_setSensitivity(Camera* self, float sensitivity);
void camera_setSpeed(Camera* self, float speed);
void camera_setZ(Camera* self, float nearZ, float farZ);
void camera_setFov(Camera* self, float fov);
void camera_setAngle(Camera* self, float horizontalAngle, float verticalAngle);
void camera_setPosition(Camera* self, vec3* position);
void camera_setDirection(Camera* self, vec3* direction);
void camera_setTarget(Camera* self, vec3* target);
void camera_setUp(Camera* self, vec3* up);
mat4* camera_getView(Camera* self);
mat4* camera_getProjection(Camera* self);
/* Camera *************************************************************/

/* Vertex *************************************************************/
typedef struct {
      vec3 pos;
      vec2 uv;
      vec3 normal;
} Vertex;
/* Vertex *************************************************************/

/* Object *************************************************************/
typedef struct {
      const char* objFile;
      vec3 globalPosition;
      vec3 globalScale;
      mat4 globalRotation;
      VertexBuffer vb;
      IndexBuffer ib;
      Vertex* vertices;
      ImageTexture* texture;
} Object;
bool object_load(const char* fileName);
/* Object *************************************************************/

/* Renderer ***********************************************************/
typedef struct {
      const char* ID;
      Window* window;
      Camera* camera;
      VideoShader* shader;
      List objPtrs;
} Renderer;
void renderer_setup(Renderer* self, const char* windowName, unsigned int width, unsigned int height, const char* vShaderPath, const char* fShaderPath);
/* Renderer ***********************************************************/
