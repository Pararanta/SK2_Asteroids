#ifndef H_ASTEROIDS_SHADER
#define H_ASTEROIDS_SHADER
    #include <glad/gl.h>
    #include <GLFW/glfw3.h>

    GLuint createShader(const char * vertex_src, const char * geometry_src, const char * fragment_src);
#endif