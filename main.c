/*****************************************************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\glfw3.h>

#include "code/list.h"
#include "code/map.h"
#include "code/clag.h"
#include "code/openGL.h"
#include "code/object.h"

#define WIDTH 1440
#define HEIGHT 810

float buf[9]= {
     -0.5, -0.5, 0.0,
      0.5, -0.5, 0.0,
      0.0,  0.5, 0.0
};
unsigned int index[3]= {
      0, 1, 2
};

int main(int argc, char** argv) {
      printf("Hello, World!\n");
      stream= stdout;
      Window w;
      window_open(&w, "Minecraft", WIDTH, HEIGHT);
      window_setIcon(&w, "icon.png");
      Camera c;
      vec3 pos= vec3_getVec(0, 0, 1);
      vec3 dir= vec3_getVec(0, -1, 0);
      // &width, &height, &aspect, &windowMode
      camera_start(&c, &w);
      //sensitivity, speed, nearZ, farZ
      camera_init(&c, 1, .1, .001, 1000, 100, &pos, &dir, "Player Camera");
      VideoShader s;
      videoShader_load(&s, "shaders/defaultShader.vs.glsl", "shaders/defaultShader.fs.glsl", "Basic Shader");
      videoShader_bind(&s);
      VertexBuffer vb;
      IndexBuffer ib;
      VertexArrayObject vao;
      vertexBuffer_init(&vb, buf, 3, 3);
      vertexArrayObject_init(&vao, 3);
      vertexArrayObject_addElement(&vao, 3);
      indexBuffer_init(&ib, index, 3);
      double currentTime= 0;
      double deltaTime= 0;
      double lastTime= glfwGetTime();
      double timeElapsed= 0;
      double fps= 0;
      do {
            currentTime= glfwGetTime();
            deltaTime= currentTime - lastTime;
            lastTime= currentTime;
            timeElapsed += deltaTime;
            fps= 1.0 / deltaTime;
            videoShader_setUniform_mat4(&s, "VM", camera_getView(&c));
            videoShader_setUniform_mat4(&s, "PM", camera_getProjection(&c));
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
            camera_update(&c, deltaTime);
      } while (window_update(&w));
      window_delete(&w);
      glfwTerminate();
      return EXIT_SUCCESS;
}
