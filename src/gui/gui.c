#include <gui.h>
#include <glfw.h>

Room * room;

int guiRender()
{
    return 0;
}

int guiRun(void * room_v)
{
    room = (Room*)room_v;
    glfwRun(guiRender);
    return 0;
}
