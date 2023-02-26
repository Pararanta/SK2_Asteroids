#include <gui.h>
#include <glfw.h>
#include <loop.h>
#include <shader_src.h>
#include <shader.h>
#include <entity.h>
#include <room.h>
#include <stdio.h>

GLuint program;

int guiInit()
{
    program = createShader(vertex_circle_src, geometry_circle_src, fragment_circle_src);
    useProgram(program);
    useAttrib(program, "coord");
    useAttrib(program, "color");

    //glGenBuffers(1, &coords_buf);
    //glGenBuffers(1, &colors_buf);
    return 0;
}

int guiRender()
{
    useProgram(program);
    useAttrib(program, "coord");
    useAttrib(program, "color");

    Room * room = getThreadRoom();
    float coords[MAX_ENTITY_COUNT*3];
    float colors[MAX_ENTITY_COUNT*3];
    uint16_t draw_cnt = 0;
    for(int i = 0; i < MAX_ENTITY_COUNT; i++)
        addToAttribute(&room->entities[i], &draw_cnt, coords, colors);

    setAttrib(program, "coord", coords);
    setAttrib(program, "color", colors);
    useProgram(program);
    glDrawArrays(GL_POINTS, 0, 3);

    disableAttrib(program, "coord");
    disableAttrib(program, "color");

    return 0;
}

int guiRun()
{
    glfwRun(guiInit, guiRender, NULL);
    return 0;
}
