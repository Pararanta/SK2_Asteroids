#include <gui.h>
#include <glfw.h>

int guiRender()
{
    return 0;
}

int guiRun(int room_index)
{
    (void)room_index;
    glfwRun(guiRender);
    return 0;
}
