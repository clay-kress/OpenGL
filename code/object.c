#include "object.h"

/* Camera *************************************************************/
void camera_init(Camera* self, float sensitivity, float speed, float nearZ, float farZ, float fov, vec3* position, vec3* direction, const char* ID) {
      self->ID= ID;
      // Init all variables to sane values.
      self->sensitivity= sensitivity;
      self->speed= speed;
      self->fov= fov;
      self->nearZ= nearZ;
      self->farZ= farZ;
      self->horizontalAngle= 0;
      self->verticalAngle= 0;
      memcpy(&self->position, position, sizeof(vec3));
      memcpy(&self->direction, direction, sizeof(vec3));
      camera_update(self);
      return;
}
void camera_update(Camera* self) {
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
      horizontalAngle= fabs(horizontalAngle);
      horizontalAngle= fmod(horizontalAngle, PI * 2.0);
      verticalAngle= fmod(verticalAngle, PI / 2.0);
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
mat4 camera_getView(Camera* self) {
      return self->view;
}
mat4 camera_getProjection(Camera* self) {
      return self->projection;
}
/* Camera *************************************************************/
