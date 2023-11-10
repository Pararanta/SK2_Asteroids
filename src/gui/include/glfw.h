#ifndef H_ASTEROIDS_GLFW
    #define H_ASTEROIDS_GLFW

    #ifndef WINDOW_SIZE
        #define WINDOW_SIZE 768
    #endif

    #include <stdio.h>
    #include <glad/gl.h>
    #include <GLFW/glfw3.h>
    
    int glfwRun(int (*initCode)(), int (*runCode)(), int (*finCode)(),
    void (*keyCallback)(GLFWwindow*, int, int, int, int),
    void (*mouseCallback)(GLFWwindow*, int, int, int),
    void (*positionCallback)(GLFWwindow*, double, double),
    const char* name);
#endif