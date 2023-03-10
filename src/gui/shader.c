#include <stdio.h>
#include <shader.h>

GLuint compileShader(const char * source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    printf("Shader created\n");
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    printf("Compilation finished\n");
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
    printf("Compiling vertex\n");
    GLuint vertex_shader = compileShader(vertex_src, GL_VERTEX_SHADER);

    GLuint geometry_shader;
    if(geometry_src)
    {
        printf("Compiling geometry\n");
        geometry_shader = compileShader(geometry_src, GL_GEOMETRY_SHADER);
    }

    printf("Compiling fragment\n");
    GLuint fragment_shader = compileShader(fragment_src, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    if(geometry_src)
        glAttachShader(program, geometry_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Shader linking failed: %s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    if(geometry_src)
        glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    return program;
}

int useProgram(GLuint program)
{
    glUseProgram(program);
}

int setAttrib(GLuint program, const char * name, void * data, uint16_t count, GLuint vbo)
{
    GLint location = glGetAttribLocation(program, name);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(location);
}

int useAttrib(GLuint program, const char * name)
{
    GLint location = glGetAttribLocation(program, name);
    glEnableVertexAttribArray(location);
}

int disableAttrib(GLuint program, const char * name)
{
    GLint location = glGetAttribLocation(program, name);
    glDisableVertexAttribArray(location);
}
