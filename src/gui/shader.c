#include <stdio.h>
#include <shader.h>

GLuint compileShader(const char * source, GLuint type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }

    return shader;
}

GLuint createShader(const char * vertex_src, const char * geometry_src, const char * fragment_src)
{
    printf("Compiling vertex");
    GLuint vertex_shader = compileShader(vertex_src, GL_VERTEX_SHADER);

    GLuint geometry_shader;
    if(geometry_src)
    {
        printf("Compiling geometry");
        geometry_shader = compileShader(geometry_src, GL_GEOMETRY_SHADER);
    }

    printf("Compiling fragment");
    GLuint fragment_shader = compileShader(fragment_src, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    if(geometry_src)
        glAttachShader(program, geometry_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        printf("Shader linking failed: %s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    if(geometry_src)
        glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    return program;
}