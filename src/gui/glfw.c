#include <glfw.h>

int glfwStart(int window_size, char * window_title, GLFWwindow ** out_window);
int glfwFinish(GLFWwindow * window);
int error(int occured, const char * error);
void GLFW_error(int occured, const char* error);

int glfwRun(int (*initCode)(), int (*runCode)(), int (*finCode)(),
void (*keyCallback)(GLFWwindow*, int, int, int, int),
void (*mouseCallback)(GLFWwindow*, int, int, int),
void (*positionCallback)(GLFWwindow*, double, double),
const char * name)
{
   GLFWwindow * window;
   if(glfwStart(WINDOW_SIZE, name, &window))
      return 1;
      
   if(initCode)
      initCode();

   if(keyCallback)
      glfwSetKeyCallback(window, keyCallback);
   if(mouseCallback)
      glfwSetMouseButtonCallback(window, mouseCallback);
   if(positionCallback)
      glfwSetCursorPosCallback(window, positionCallback);

   GLuint vao;
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   while(!glfwWindowShouldClose(window))
   {
      glClear(GL_COLOR_BUFFER_BIT);
      if(runCode)
         if(runCode())
            glfwSetWindowShouldClose(window, GLFW_TRUE);
      glfwSwapBuffers(window);
      glfwPollEvents();

      GLenum error;
      while ((error = glGetError()) != GL_NO_ERROR) {
         printf("OpenGL error: %d\n", error);
      }
   }

   if(finCode)
      finCode();

   glfwFinish(window);

   return 0;
}

int glfwStart(int window_size, char * window_title, GLFWwindow ** out_window)
{
   if (error(!glfwInit(), "GLFW init failed!"))
      return 1;

   glfwSetErrorCallback(GLFW_error);

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

   #ifdef __APPLE__
      printf("GLFW_OPENGL_FORWARD_COMPAT\n");
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   #endif

   GLFWwindow * window = glfwCreateWindow(window_size, window_size, window_title, NULL, NULL);
   if (error(!window, "Window creation error"))
      return 1;

   (*out_window) = window;

   glfwMakeContextCurrent(window);
   gladLoadGL(glfwGetProcAddress);

   glClearColor(0, 0, 0, 1);
   glfwSwapInterval(1);

   int fbWidth, fbHeight;
   glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
   glViewport(0, 0, fbWidth, fbHeight);

   return 0;
}

int glfwFinish(GLFWwindow * window)
{
   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}

int error(int occured, const char * error)
{
   if (occured)
      fprintf(stderr, "Application error occured (%d): %s\n", occured, error);
   return occured;
}

void GLFW_error(int occured, const char* error)
{
    if (occured)
        fprintf(stderr, "GLFW error occured (%d): %s\n", occured, error);
}