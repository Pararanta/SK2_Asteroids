#include <stdio.h>
#include <signal.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define WINDOW_SIZE 512

int error(int occured, static char * error)
{
   if(occured)
      fprintf(stderr, "Application error occured (%d): %s\n", occured, error);
   return occured;
}

int glfwStart(int window_size, static char * window_title, GLFWwindow ** out_window)
{
   if (error(!glfwInit(), "GLFW init failed!"))
      return 1;

   glfwSetErrorCallback(error);

   GLFWwindow * window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Hello World", NULL, NULL);
   if (error(!window, "Window creation error"))
      return 1;

   (*out_window) = window;

   glfwMakeContextCurrent(window);
   gladLoadGL(glfwGetProcAddress);
   glfwSwapInterval(1);
    
   return 0;
}

int glfwFinish(GLFWwindow * window){
   glfwDestroyWindow(window);
   glfwTerminate();
}

static volatile int run = 1;
void intHandler(int dummy) {
    run = 0;
}

int main() {
   printf("Hello, World!");

   GLFWwindow * window;
   if(glfwStart(WINDOW_SIZE, "Hello World!", &window))
      return 1;

   signal(SIGINT, intHandler);

   while(!glfwWindowShouldClose(window))
   {
      if(!run)
         glfwSetWindowShouldClose(window, GLFW_TRUE);

      glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwFinish(window);

   return 0;
}