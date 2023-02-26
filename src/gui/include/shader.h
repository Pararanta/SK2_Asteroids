#ifndef H_ASTEROIDS_SHADER
#define H_ASTEROIDS_SHADER
    #include <glad/gl.h>
    #include <GLFW/glfw3.h>

    GLuint createShader(const char * vertex_src, const char * geometry_src, const char * fragment_src);
    int useProgram(GLuint program);
    int setAttrib(GLuint program, const char * name, void * data);
    int useAttrib(GLuint program, const char * name);
    int disableAttrib(GLuint program, const char * name);
#endif