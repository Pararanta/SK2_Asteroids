#ifndef H_ASTEROIDS_GLFW
    #define H_ASTEROIDS_GLFW

    #ifndef WINDOW_SIZE
        #define WINDOW_SIZE 512
    #endif
    
    int glfwRun(int (*initCode)(), int (*runCode)(), int (*finCode)());
#endif