#include <gui.h>
#include <glfw.h>
#include <loop.h>
#include <shader_src.h>
#include <shader.h>

int guiInit()
{
    createShader(vertex_circle_src, geometry_circle_src, fragment_circle_src);

    return 0;
}

int guiRender()
{
    Room * room = getThreadRoom();
    return 0;
}

int guiRun(void * room_v)
{
    Room * room = (Room*)room_v;
    glfwRun(guiInit, NULL, NULL);
    return 0;
}
