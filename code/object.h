#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "list.h"
#include "map.h"
#include "clag.h"

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
} Camera;
void camera_init(Camera* self, float sensitivity, float speed, float nearZ, float farZ, float fov, vec3* position, vec3* direction, const char* ID);
/* Camera *************************************************************/

/* Object *************************************************************/
typedef struct {
} Object;
/* Object *************************************************************/
