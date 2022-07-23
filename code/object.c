#include "object.h"

/* Camera *************************************************************/
void camera_start(Camera* self, Window* w) {
      self->width= &w->width;
      self->height= &w->height;
      self->aspect= &w->aspect;
      self->winMode= &w->mode;
      self->windowRef= &w;
}
void camera_init(Camera* self, float sensitivity, float speed, float nearZ, float farZ, float fov, vec3* position, vec3* direction, const char* ID) {
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
      glfwSetCursorPos(self->windowRef->window, *self->width / 2.0, *self->height / 2.0);
      camera_update(self, 0);
      return;
}
void camera_update(Camera* self, double deltaTime) {
      if (*self->winMode == 0) {
            // Get cursor position and recenter cursor
            double xpos, ypos;
            glfwGetCursorPos(self->windowRef->window, &xpos, &ypos);
            glfwSetCursorPos(self->windowRef->window, *self->width / 2.0, *self->height / 2.0);
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
            if (glfwGetKey(self->windowRef->window, GLFW_KEY_W) == GLFW_PRESS) {
                  vec3_add(&self->position, &velocity, &self->position);
            }
            if (glfwGetKey(self->windowRef->window, GLFW_KEY_S) == GLFW_PRESS) {
                  vec3_sub(&self->position, &velocity, &self->position);
            }
            if (glfwGetKey(self->windowRef->window, GLFW_KEY_A) == GLFW_PRESS) {
                  vec3_sub(&self->position, &right, &self->position);
            }
            if (glfwGetKey(self->windowRef->window, GLFW_KEY_D) == GLFW_PRESS) {
                  vec3_add(&self->position, &right, &self->position);
            }
            if (glfwGetKey(self->windowRef->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                  vec3_add(&self->position, &up, &self->position);
            }
            if (glfwGetKey(self->windowRef->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
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

/* Vertex *************************************************************/
/* Vertex *************************************************************/

/* Object *************************************************************/
bool object_load(Object* self, const char* fileName) {
      self->objFile= fileName;
      // Define lists to store the obj data
      List vertexPos;                                                   // Stores the vertex positions (x, y, z)
      initList(&vertexPos, 3 * sizeof(float), 0);
      List uv_coords;                                                   // Stores each uv coordinate (for textures)
      initList(&uv_coords, 2 * sizeof(float), 0);
      List normals;                                                     // Stores each normal coordinate (for lighting)
      initList(&normals, 3 * sizeof(float), 0);
      List indices;                                                     // Stores the indices for each set of vertex attributes
      initList(&indices, 3 * sizeof(int), 0);
      // Open the object file to load
      FILE* objFile= fopen(objFilePath, "r");
      if (objFile == NULL) {
            fprintf(stderr, "Error: Failed to open Object file! Are you sure you have the correct path?");
            return 0;
      }
      // Find the length of the file so the program can allocate enough space for it
      int fLen= 0;
      while (fgetc(objFile) != EOF) {
            fLen++;
      }
      // Return file pointer to the beginning of the file
      fseek(objFile, 0, SEEK_SET);
      // Allocate space for the file contents (characters)
      char* fileBuf= malloc(fLen + 1);
      if (fileBuf == NULL) {
            return 0;
      }
      // Fill the file buffer with the data from the file
      for (int i= 0; i < fLen; i++) {
            fileBuf[i]= fgetc(objFile);
      }
      // Add a null termination character at the end
      fileBuf[fLen]= '\0';
      // Close the obj file
      fclose(objFile);
      // For each line
      int beginLine= 0;
      int lineNum= 0;
      while (fileBuf[beginLine] != '\0') {
            int endOfLine= beginLine;
            // Find the end of the line
            while (fileBuf[endOfLine] != '\n' && fileBuf[endOfLine] != '\0') {
                  endOfLine++;
            }
            // '\0' indicates the end of a file
            if (fileBuf[endOfLine] == '\0') {
                  break;
            }
            // Allocate space for the chars in the line
            char* line= (char*) malloc(sizeof(char) * (endOfLine - beginLine) + 2);
            // Strings in C are null terminated
            line[sizeof(char) * (endOfLine - beginLine) + 1]= '\0';
            // Copy data from the file buffer to the line buffer
            for (int i= beginLine; i <= sizeof(char) * endOfLine; i++) {
                  line[i - beginLine]= fileBuf[i];
            }
            // At this point we have the current line in 'line'
            //printf("%s", line);
            if (line[0] == 'v' && line[1] == ' ') {
                  float x, y, z;
                  // Find the vertex in the line string and push it to the vertex list
                  sscanf(line, "v %f %f %f\n", &x, &y, &z);
                  vec3 vert= list_push(&vertexPos, NULL);
                  vert[0]= x;
                  vert[1]= y;
                  vert[2]= z;
            }
            if (line[0] == 'v' && line[1] == 't') {
                  float u, v;
                  // Find the uv coordinates in the line and push them to the uv_coords list
                  sscanf(line, "vt %f %f\n", &u, &v);
                  vec2 coords= list_push(&uv_coords, NULL);
                  coords[0]= u;
                  coords[1]= v;
            }
            if (line[0] == 'v' && line[1] == 'n') {
                  float x, y, z;
                  // Find the normal vectors in the line and push them to the normals list
                  sscanf(line, "vn %f %f %f\n", &x, &y, &z);
                  vec3 vert= list_push(&normals, NULL);
                  vert[0]= x;
                  vert[1]= y;
                  vert[2]= z;
            }
            if (line[0] == 'f' && line[1] == ' ') {
                  int vti[3];
                  int uvi[3];
                  int nmi[3];
                  sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vti[0], &uvi[0], &nmi[0], &vti[1], &uvi[1], &nmi[1], &vti[2], &uvi[2], &nmi[2]);
                  int* index;
                  index= list_push(&indices, NULL);
                  index[0]= vti[0];
                  index[1]= uvi[0];
                  index[2]= nmi[0];
                  index= list_push(&indices, NULL);
                  index[0]= vti[1];
                  index[1]= uvi[1];
                  index[2]= nmi[1];
                  index= list_push(&indices, NULL);
                  index[0]= vti[2];
                  index[1]= uvi[2];
                  index[2]= nmi[2];
            }
            /**********************************************************/
            free(line);
            beginLine= endOfLine + 1;
            lineNum++;
      }
      free(fileBuf);
      // Finally compile all the data into the vertex and index array
      // f 5/1/1 1/2/1 4/3/1 <-- each line describes a triangle, each x/y/z describes a single vertex
      List storedIndices;                                               // Define a list for already processed vertex indices
      initList(&storedIndices, 3 * sizeof(int), 0);
      // Loop through all vertex indices
      for (int i= 0; i < list_len(&indices); i++) {
            int* currentIndex= list_get(&indices, i);
            // Find whether the vertex index has already been stored
            // Were really looking for vertices here. The indices stored in the obj file each refer to a set of vertex attributes that will make up a vertex
            bool found= false;
            int indexLoc= 0;                                            // indexLoc indicates the location of the index (only if it has been found)
            for (int j= 0; j < list_len(&storedIndices); j++) {
                  int* storedIndex= list_get(&storedIndices, j);
                  indexLoc= j;
                  // if the index is equal to one stored in 'storedIndices'
                  if (currentIndex[0] == storedIndex[0] && currentIndex[1] == storedIndex[1] && currentIndex[2] == storedIndex[2]) {
                        found= true;
                        break;
                  }
            }
            if (found == true) {
                  // If the index was found just push the location of the index/vertex onto the indexBuffer to output
                  list_push(indexBuf, &indexLoc);
            } else {
                  // If the index was not found process the vertex data referenced and push it onto the vertexBuffer then add the new index to the indexBuffer
                  int vertexLoc= currentIndex[0] - 1;
                  int uvLoc= currentIndex[1] - 1;
                  int normalLoc= currentIndex[2] - 1;
                  // Add the current index to the list of stored indices
                  list_push(&storedIndices, currentIndex);
                  // Compile the vertex data into the actual vertex
                  float* newVertex= list_push(vertexBuf, NULL);
                  newVertex[0]= ((float*) list_get(&vertexPos, vertexLoc))[0];
                  newVertex[1]= ((float*) list_get(&vertexPos, vertexLoc))[1];
                  newVertex[2]= ((float*) list_get(&vertexPos, vertexLoc))[2];
                  newVertex[3]= ((float*) list_get(&uv_coords, uvLoc))[0];
                  newVertex[4]= ((float*) list_get(&uv_coords, uvLoc))[1];
                  newVertex[5]= ((float*) list_get(&normals, normalLoc))[0];
                  newVertex[6]= ((float*) list_get(&normals, normalLoc))[1];
                  newVertex[7]= ((float*) list_get(&normals, normalLoc))[2];
                  int newIndex= list_len(vertexBuf) - 1;                // This index points to the last vertex that was just added
                  list_push(indexBuf, &newIndex);
            }
      }
      /****************************************************************/
      deleteList(&vertexPos);
      deleteList(&uv_coords);
      deleteList(&normals);
      deleteList(&indices);
      return 1;
}
/* Object *************************************************************/

/* Renderer ***********************************************************/
void renderer_setup(Renderer* self, const char* name, unsigned int width, unsigned int height, const char* vShaderPath, const char* fShaderPath) {
      self->ID= name;
      self->window= (Window*) malloc(sizeof(Window));
      self->camera= (Camera*) malloc(sizeof(Camera));
      self->shader= (VideoShader*) malloc(sizeof(VideoShader));
      initList(&self->objPtrs, sizeof(Object*), 0);
      // Init the Window
      window_open(self->window, self->ID, width, height);
      // Init Camera
      vec3 pos= vec3_getVec(0, 0, 1);
      vec3 dir= vec3_getVec(0, 0, 0);
      // &width, &height, &aspect, &windowMode
      camera_start(self->camera, self->window);
      //sensitivity, speed, nearZ, farZ, fov
      camera_init(self->camera, 1, .1, .001, 1000, 90, &pos, &dir, self->ID);
      // Init VideoShader
      videoShader_load(self->shader, vShaderPath, fShaderPath, self->ID);
}
/* Renderer ***********************************************************/
