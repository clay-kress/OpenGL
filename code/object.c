#include "object.h"

/* Camera *************************************************************/
void camera_start(Camera* self, Window* w) {
      self->width= &w->width;
      self->height= &w->height;
      self->aspect= &w->aspect;
      self->winMode= &w->mode;
      self->windowRef= &w->window;
}
void camera_init(Camera* self, float sensitivity, float speed, float nearZ, float farZ, float fov, vec3* position, vec3* direction, char* ID) {
      self->ID= ID;
      // Init all variables to sane values.
      self->sensitivity= sensitivity;
      self->speed= speed;
      self->fov= rad(fov);
      self->nearZ= nearZ;
      self->farZ= farZ;
      self->horizontalAngle= 0;
      self->verticalAngle= 0;
      memcpy(&self->position, position, sizeof(vec3));
      memcpy(&self->direction, direction, sizeof(vec3));
      vec3_add(&self->position, &self->direction, &self->target);
      vec3_init(&self->up, 0, 1, 0);
      mat4_identity(&self->view);
      mat4_identity(&self->projection);
      glfwSetCursorPos(*self->windowRef, *self->width / 2.0, *self->height / 2.0);
      camera_update(self, 0);
      return;
}
void camera_update(Camera* self, double deltaTime) {
      if (*self->winMode == 0) {
            // Get cursor position and recenter cursor
            double xpos, ypos;
            glfwGetCursorPos(*self->windowRef, &xpos, &ypos);
            glfwSetCursorPos(*self->windowRef, *self->width / 2.0, *self->height / 2.0);
            // Compute new orientation
            self->horizontalAngle += rad(self->sensitivity * (*self->width / 2.0 - xpos) * deltaTime);
            self->verticalAngle += rad(self->sensitivity * (*self->height / 2.0 - ypos) * deltaTime);
            // Bind angles to sane values
            if (fabs(self->verticalAngle) >= PI / 2.0 - rad(1)) {
                  self->verticalAngle= (PI / 2.0  - rad(1)) * (self->verticalAngle > 0 ? 1 : -1);
            }
            self->horizontalAngle= fmod(self->horizontalAngle, 2.0 * PI);
            if (self->horizontalAngle < 0.0) {
                  self->horizontalAngle= (2.0 * PI) - self->horizontalAngle;
            }
            // Set the direction vector
            self->direction.x= cos(self->verticalAngle) * sin(self->horizontalAngle);
            self->direction.y= sin(self->verticalAngle);
            self->direction.z= cos(self->verticalAngle) * cos(self->horizontalAngle);
            vec3_normalize(&self->direction, &self->direction);
            // Init and scale velocity, up, and right
            vec3 velocity, right, up;
            vec3_init(&velocity, sin(self->horizontalAngle), 0, cos(self->horizontalAngle));
            memcpy(&up, &self->up, sizeof(vec3));
            vec3_cross(&self->direction, &self->up, &right);
            vec3_normalize(&right, &right);
            vec3_scale(&velocity, self->speed, &velocity);
            vec3_scale(&right, self->speed, &right);
            vec3_scale(&up, self->speed, &up);
            // TODO: Set the position vector
            if (glfwGetKey(*self->windowRef, GLFW_KEY_W) == GLFW_PRESS) {
                  vec3_add(&self->position, &velocity, &self->position);
            }
            if (glfwGetKey(*self->windowRef, GLFW_KEY_S) == GLFW_PRESS) {
                  vec3_sub(&self->position, &velocity, &self->position);
            }
            if (glfwGetKey(*self->windowRef, GLFW_KEY_A) == GLFW_PRESS) {
                  vec3_sub(&self->position, &right, &self->position);
            }
            if (glfwGetKey(*self->windowRef, GLFW_KEY_D) == GLFW_PRESS) {
                  vec3_add(&self->position, &right, &self->position);
            }
            if (glfwGetKey(*self->windowRef, GLFW_KEY_SPACE) == GLFW_PRESS) {
                  vec3_add(&self->position, &up, &self->position);
            }
            if (glfwGetKey(*self->windowRef, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                  vec3_sub(&self->position, &up, &self->position);
            }
            // Sets the View and Projection matricies
            vec3_add(&self->position, &self->direction, &self->target);
            glm_lookAt(&self->position, &self->target, &self->up, &self->view);
            glm_perspective(self->fov, *self->aspect, self->nearZ, self->farZ, &self->projection);
      }
      return;
}
void camera_setSensitivity(Camera* self, float sensitivity) {
      self->sensitivity= sensitivity;
      return;
}
void camera_setSpeed(Camera* self, float speed) {
      self->speed= speed;
      return;
}
void camera_setZ(Camera* self, float nearZ, float farZ) {
      if (nearZ >= farZ) {
            return;
      }
      self->nearZ= nearZ;
      self->farZ= farZ;
      return;
}
void camera_setFov(Camera* self, float fov) {
      self->fov= fov;
      return;
}
void camera_setAngle(Camera* self, float horizontalAngle, float verticalAngle) {
      if (fabs(verticalAngle) >= PI / 2.0 - rad(1)) {
            verticalAngle= (PI / 2.0  - rad(1)) * (verticalAngle > 0 ? 1 : -1);
      }
      horizontalAngle= fmod(horizontalAngle, 2.0 * PI);
      if (horizontalAngle < 0.0) {
            horizontalAngle= (2.0 * PI) - horizontalAngle;
      }
      self->verticalAngle= verticalAngle;
      self->horizontalAngle= horizontalAngle;
      return;
}
void camera_setPosition(Camera* self, vec3* position) {
      memcpy(&self->position, position, sizeof(vec3));
      return;
}
void camera_setDirection(Camera* self, vec3* direction) {
      memcpy(&self->direction, direction, sizeof(vec3));
      return;
}
void camera_setTarget(Camera* self, vec3* target) {
      memcpy(&self->target, target, sizeof(vec3));
      return;
}
void camera_setUp(Camera* self, vec3* up) {
      memcpy(&self->up, up, sizeof(vec3));
      return;
}
mat4* camera_getView(Camera* self) {
      return &self->view;
}
mat4* camera_getProjection(Camera* self) {
      return &self->projection;
}
/* Camera *************************************************************/

/* Renderer ***********************************************************/
void renderer_setup() {
      
}
/* Renderer ***********************************************************/
