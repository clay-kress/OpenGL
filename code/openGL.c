#include "openGL.h"

unsigned int CURRENT_WIDTH= 1440;
unsigned int CURRENT_HEIGHT= 810;

FILE* stream;                                                           // All error messages that occur in OpenGL are sent to this file (often stdout/stderr)

/* GLFW Callbacks *****************************************************/
void windowResizeCallback(GLFWwindow* window, int width, int height) {
      glfwMakeContextCurrent(window);
      CURRENT_WIDTH= width;
      CURRENT_HEIGHT= height;
      glViewport(0, 0, CURRENT_WIDTH, CURRENT_HEIGHT);
      return;
}
/* GLFW Callbacks *****************************************************/

/* Vertex Buffer ******************************************************/
void vertexBuffer_init(VertexBuffer* self, const float* vertices, int numVerts, int sizeOfVerts) {
      glGenBuffers(1, &self->vertexBufferRef);
      glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferRef);
      self->size= numVerts * sizeOfVerts * sizeof(float);
      glBufferData(GL_ARRAY_BUFFER, self->size, vertices, GL_DYNAMIC_DRAW);
      return;
}
void vertexBuffer_bind(VertexBuffer* self) {
      glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferRef);
      return;
}
void vertexBuffer_delete(VertexBuffer* self) {
      glDeleteBuffers(1, &self->vertexBufferRef);
      return;
}
void vertexBuffer_rebufferData(VertexBuffer* self, float* vertices, int numVerts, int sizeOfVerts) {
      glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferRef);
      self->size= numVerts * sizeOfVerts * sizeof(float);
      glBufferSubData(GL_ARRAY_BUFFER, 0, self->size, vertices);
      return;
}
/* Vertex Buffer ******************************************************/

/* Index Buffer *******************************************************/
void indexBuffer_init(IndexBuffer* self, const unsigned int* indices, int numIndices) {
      glGenBuffers(1, &self->indexBufferRef);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferRef);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
      self->length= numIndices;
      return;
}
void indexBuffer_bind(IndexBuffer* self) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferRef);
      return;
}
void indexBuffer_delete(IndexBuffer* self) {
      glDeleteBuffers(1, &self->indexBufferRef);
      return;
}
void indexBuffer_rebufferData(IndexBuffer* self, unsigned int* indices) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferRef);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, self->length * sizeof(unsigned int), indices);
      return;
}
/* Index Buffer *******************************************************/

/* Vertex Array Object ************************************************/
void vertexArrayObject_init(VertexArrayObject* self, int vertexLength) {
      self->locations= 0;
      self->offset= 0;
      self->vertexLength= vertexLength;
      glGenVertexArrays(1, &self->vaoRef);
      glBindVertexArray(self->vaoRef);
      return;
}
void vertexArrayObject_bind(VertexArrayObject* self) {
      glBindVertexArray(self->vaoRef);
      return;
}
void vertexArrayObject_delete(VertexArrayObject* self) {
      glDeleteVertexArrays(1, &self->vaoRef);
      return;
}
void vertexArrayObject_bindVertexBuffer(VertexArrayObject* self, VertexBuffer* vb) {
      glBindBuffer(GL_ARRAY_BUFFER, vb->vertexBufferRef);
      glBindVertexArray(self->vaoRef);
      return;
}
void vertexArrayObject_addElement(VertexArrayObject* self, int len) {
      glBindVertexArray(self->vaoRef);
      glVertexAttribPointer(self->locations, len, GL_FLOAT, GL_FALSE, self->vertexLength * sizeof(float), (const void*) (self->offset * sizeof(float)));
      glEnableVertexAttribArray(self->locations);
      self->offset += len;
      self->locations++;
      return;
}
/* Vertex Array Object ************************************************/

/* VideoShader ********************************************************/
bool videoShader_load(VideoShader* self, const char* vertexShaderPath, const char* fragmentShaderPath, const char* name) {
      self->name= name;
      self->vertexShaderPath= vertexShaderPath;
      self->fragmentShaderPath= fragmentShaderPath;
      // Fetch the Vertex Shader source file
      FILE* vertexShaderFile= fopen(self->vertexShaderPath, "r");       // Open the vertex shader file
      if (vertexShaderFile == NULL) {
            fprintf(stream, "Error: Vertex shader file not found!\n");
            return false;
      }
      int vLen= 0;
      while (fgetc(vertexShaderFile) != EOF) {
            vLen++;
      }
      fseek(vertexShaderFile, 0, SEEK_SET);
      char* vBuf= malloc(vLen + 1);
      for (int i= 0; i < vLen; i++) {
            vBuf[i]= fgetc(vertexShaderFile);
      }
      vBuf[vLen]= '\0';
      fclose(vertexShaderFile);                                         // Close the file
      // Fetch the Fragment Shader source file
      FILE* fragmentShaderFile= fopen(self->fragmentShaderPath, "r");   // Open the fragment shader file
      if (fragmentShaderFile == NULL) {
            fprintf(stream, "Error: Fragment shader file not found!\n");
            return false;
      }
      int fLen= 0;
      while (fgetc(vertexShaderFile) != EOF) {
            fLen++;
      }
      fseek(vertexShaderFile, 0, SEEK_SET);
      char* fBuf= malloc(fLen + 1);
      for (int i= 0; i < fLen; i++) {
            fBuf[i]= fgetc(vertexShaderFile);
      }
      fBuf[fLen]= '\0';
      fclose(fragmentShaderFile);                                       // Close the file
      // Create Shader Program
      self->programID= glCreateProgram();                               // Create a new shader program instance in OpenGL
      // Create new Vertex Shader and compile the source code
      GLuint vs= glCreateShader(GL_VERTEX_SHADER);                      // Create the vertex shader instance in OpenGL
      glShaderSource(vs, 1, (const GLchar* const*) &vBuf, NULL);        // Point the vertex shader instance to the vertex shader source code
      glCompileShader(vs);                                              // Compile the vertex shader source code
      // Do error catching
      GLint result;
      glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
      if (result == GL_FALSE) {
            int length;
            glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
            char* message= malloc(length);
            glGetShaderInfoLog(vs, length, &length, message);
            fprintf(stream, "Error: Failed to compile Vertex Shader!\n");
            fprintf(stream, "   %s", message);
            free(message);
            return false;
      }
      // Create new Fragment Shader and compile the source code
      GLuint fs= glCreateShader(GL_FRAGMENT_SHADER);                    // Create the fragment shader instance in OpenGL
      glShaderSource(fs, 1, (const GLchar* const*) &fBuf, NULL);        // Point the fragment shader instance to the fragment shader source code
      glCompileShader(fs);                                              // Compile the fragment shader source code
      // Do error catching
      glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
      if (result == GL_FALSE) {
            int length;
            glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
            char* message= malloc(length);
            glGetShaderInfoLog(fs, length, &length, message);
            fprintf(stream, "Error: Failed to compile Fragment Shader!\n");
            fprintf(stream, "   %s", message);
            free(message);
            return false;
      }
      // Combine the shaders into one program
      glAttachShader(self->programID, vs);                              // Attach the vertex shader to the shader program
      glAttachShader(self->programID, fs);                              // Attach the fragment shader to the shader program
      glLinkProgram(self->programID);                                   // Link the vertex and the fragment shaders
      glValidateProgram(self->programID);                               // Validate?
      // Delete the seperate shaders
      glDetachShader(self->programID, vs);
      glDetachShader(self->programID, fs);
      glDeleteShader(vs);
      glDeleteShader(fs);
      // Free shader code buffers
      free(vBuf);
      free(fBuf);
      return true;
}
void videoShader_bind(VideoShader* self) {
      glUseProgram(self->programID);
      return;
}
void videoShader_delete(VideoShader* self) {
      glDeleteShader(self->programID);
      return;
}
bool videoShader_setUniform_int(VideoShader* self, const char* name, int i) {
      GLint loc= glGetUniformLocation(self->programID, name);
      if (loc == -1) {
            fprintf(stream, "Error: Uniform name not found!\n");
            return false;
      }
      glUniform1i(loc, i);
      return true;
}
bool videoShader_setUniform_vec2(VideoShader* self, const char* name, vec2* vec) {
      GLint loc= glGetUniformLocation(self->programID, name);
      if (loc == -1) {
            fprintf(stream, "Error: Uniform name not found!\n");
            return false;
      }
      glUniform2fv(loc, 1, (const GLfloat*) vec->v);
      return true;
}
bool videoShader_setUniform_vec3(VideoShader* self, const char* name, vec3* vec) {
      GLint loc= glGetUniformLocation(self->programID, name);
      if (loc == -1) {
            fprintf(stream, "Error: Uniform name not found!\n");
            return false;
      }
      glUniform3fv(loc, 1, (const GLfloat*) vec->v);
      return true;
}
bool videoShader_setUniform_vec4(VideoShader* self, const char* name, vec4* vec) {
      GLint loc= glGetUniformLocation(self->programID, name);
      if (loc == -1) {
            fprintf(stream, "Error: Uniform name not found!\n");
            return false;
      }
      glUniform4fv(loc, 1, (const GLfloat*) vec->v);
      return true;
}
bool videoShader_setUniform_mat4(VideoShader* self, const char* name, mat4* mat) {
      GLint loc= glGetUniformLocation(self->programID, name);
      if (loc == -1) {
            fprintf(stream, "Error: Uniform name not found!\n");
            return false;
      }
      glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*) mat->m);
      return true;
}
/* VideoShader ********************************************************/

/* ImageTexture *******************************************************/
bool imageTexture_loadimage(ImageTexture* self, const char* texturePath, const char* name, unsigned int slot, int type) {
      self->name= name;
      stbi_set_flip_vertically_on_load(1);
      unsigned char* textureImg= stbi_load(texturePath, &self->width, &self->height, &self->nrChannels, 0);
      if (textureImg == NULL) {
            fprintf(stream, "Error: Texture file not found!\n");
            return false;
      }
      // Generate Texture
      glGenTextures(1, &self->textureID);
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, self->textureID);
      if (type == jpg) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, self->width, self->height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg);
      } else if (type == png) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImg);
      }
      // Set parameters for filtering
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glGenerateMipmap(GL_TEXTURE_2D);
      // Free Image RAM
      stbi_image_free(textureImg);
      return true;
}
void imageTexture_bind(ImageTexture* self, unsigned int slot) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, self->textureID);
      return;
}
void imageTexture_delete(ImageTexture* self) {
      glDeleteTextures(1, &self->textureID);
      return;
}
/* ImageTexture *******************************************************/

/* Window *************************************************************/
bool window_open(Window* self, const char* name, unsigned int width, unsigned int height) {
      CURRENT_WIDTH= width;
      CURRENT_HEIGHT= height;
      self->name= name;
      self->width= width;
      self->height= height;
      self->aspect= (float) self->width / (float) self->height;
      glewExperimental= true;
      // Open the window
      if (!glfwInit()) {
            fprintf(stream, "Error: GLFW failed to initialize!\n");
            return false;
      }
      // Set default GLFW parameters
      glfwWindowHint(GLFW_SAMPLES, 4);                                  // 4x antialiasing
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                    // Use OpenGL 3.3
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // Use new OpenGL
      self->window= glfwCreateWindow(self->width, self->height, self->name, NULL, NULL);
      if (self->window == NULL) {
            fprintf(stream, "Error: GLFW failed to create window!\n");
            return false;
      }
      // Bind this window
      glfwMakeContextCurrent(self->window);
      glewExperimental= true;
      if (glewInit() != GLEW_OK) {
            fprintf(stream, "Error: GLEW failed to initialize!\n");
            return false;
      }
      // Bind all keyboard input to this window
      glfwSetInputMode(self->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetInputMode(self->window, GLFW_STICKY_KEYS, GL_TRUE);
      self->mode= 0;
      // Set resize calback Function
      glfwSetWindowSizeCallback(self->window, windowResizeCallback);
      glViewport(0, 0, self->width, self->height);
      // Set certain OpenGL directives
      //glEnable(GL_DEPTH_TEST);                                          // Enable depth test
      //glDepthFunc(GL_LESS);                                             // Accept fragment if it closer to the camera than the former one
      //glEnable(GL_CULL_FACE);                                           // Enable face culling
      //glFrontFace(GL_CCW);                                              // Define the front face as one that is wrapped counter clockwise
      //glCullFace(GL_BACK);                                              // Tell OpenGL to cull the back face
      return true;
}
void window_bind(Window* self) {
      // Bind this window
      glfwMakeContextCurrent(self->window);
      // Bind all keyboard input to this window
      glfwSetInputMode(self->window, GLFW_STICKY_KEYS, GL_TRUE);
      return;
}
void window_delete(Window* self) {
      glfwDestroyWindow(self->window);
      return;
}
bool window_setIcon(Window* self, const char* path) {
      GLFWimage icon[1];
      stbi_set_flip_vertically_on_load(0);
      icon[0].pixels = stbi_load(path, &icon[0].width, &icon[0].height, 0, 4);
      if (icon[0].pixels == NULL) {
            fprintf(stream, "Error: Icon image file not found!\n");
            return false;
      }
      glfwSetWindowIcon(self->window, 1, icon);
      stbi_image_free(icon[0].pixels);
      return true;
}
bool window_update(Window* self) {
      self->width= CURRENT_WIDTH;
      self->height= CURRENT_HEIGHT;
      self->aspect= (float) self->width / (float) self->height;
      // Swap buffers
      glfwSwapBuffers(self->window);
      glfwPollEvents();
      // Return false if window should close
      if (glfwWindowShouldClose(self->window)) {
            return false;
      }
      if (glfwGetKey(self->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetInputMode(self->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            self->mode= 1;
      }
      if (glfwGetMouseButton(self->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwSetInputMode(self->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(self->window, self->width / 2.0, self->height / 2.0);
            self->mode= 0;
      }
      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      return true;
}
/* Window *************************************************************/

/* Camera *************************************************************/

/* Camera *************************************************************/
