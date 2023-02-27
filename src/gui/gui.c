#include <gui.h>
#include <glfw.h>
#include <loop.h>
#include <shader_src.h>
#include <shader.h>
#include <entity.h>
#include <room.h>
#include <stdio.h>
#include <loop.h>

GLuint program;
GLuint coord_vbo, color_vbo;

int guiInit()
{
    glGenBuffers(1, &coord_vbo);
    glGenBuffers(1, &color_vbo);

    program = createShader(vertex_circle_src, geometry_circle_src, fragment_circle_src);
    useProgram(program);

    return 0;
}

int guiRender()
{

    Room * room = getThreadRoom();
    float coords[MAX_ENTITY_COUNT*3] = { 0 };
    float colors[MAX_ENTITY_COUNT*3] = { 0 };
    uint16_t draw_cnt = 0;
    for(int i = 0; i < MAX_ENTITY_COUNT; i++)
        addToAttribute(&room->entities[i], &draw_cnt, coords, colors);

    setAttrib(program, "coord", coords, draw_cnt, coord_vbo);
    setAttrib(program, "color", colors, draw_cnt, color_vbo);
    glDrawArrays(GL_POINTS, 0, draw_cnt);

    return 0;
}

double last = 0.0;

double updateDelta()
{
    double now = glfwGetTime();
    double delta = now - last;
    last = now;

    return delta;
}

int clientLoop()
{

    gameStep(updateDelta());
    guiRender();

    return 0;
}

int guiRun()
{
    glfwRun(guiInit, clientLoop, NULL);
    return 0;
}
