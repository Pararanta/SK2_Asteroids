#include <gui.h>
#include <glfw.h>
#include <loop.h>

int guiRender()
{
    Room * room = getThreadRoom();
    return 0;
}

int guiRun(void * room_v)
{
    Room * room = (Room*)room_v;
    glfwRun(guiRender);
    return 0;
}
