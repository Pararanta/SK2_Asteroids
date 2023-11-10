#include <gui.h>
#include <glfw.h>
#include <loop.h>
#include <shader_src.h>
#include <shader.h>
#include <entity.h>
#include <room.h>
#include <stdio.h>
#include <loop.h>
#include <stdlib.h>
#include <sleep.h>

GLuint program;
GLuint coord_vbo, color_vbo;
uint8_t up, down, left, right;
double x, y;
uint8_t shoot;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP || key == GLFW_KEY_K)
        up = action == GLFW_PRESS || action == GLFW_REPEAT;
    if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN || key == GLFW_KEY_J)
        down = action == GLFW_PRESS || action == GLFW_REPEAT;
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT || key == GLFW_KEY_H)
        left = action == GLFW_PRESS || action == GLFW_REPEAT;
    if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT || key == GLFW_KEY_L)
        right = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    shoot = action == GLFW_PRESS;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    x = xpos / width;
    y = 1 - ypos / height;
}

void cursorCallback(GLFWwindow* window, double _x, double _y)
{
    x = _x/WINDOW_SIZE;
    y = _y/WINDOW_SIZE;
    printf("%f %f\n", x, y);
}

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
    if(room->player.connection_error)
    {
       
    }
    return 0;
}

double checkDelta(Room* room)
{
    double now = getTime();
    return now - room->time;
}

double updateDelta(Room * room)
{
    room->frame++;

    double now = getTime();
    room->delta = now - room->time;
    room->time = now;

    return room->delta;
}

int clientLoop()
{
    Room* room = getThreadRoom();
    updateDelta(room);

    clientBeforeGameStep();
    gameStep(room->delta);
    clientAfterGameStep(!(room->frame%120), right - left, up - down, shoot, x, y);
    shoot = 0;
    
    if(!room->player.connection_error)
    {
        guiRender();
        room->player.finish_time = room->time;
    }

    return room->player.finish_time + 5.0 < room->time;
}
int serverLoop()
{
    Room* room = getThreadRoom();

    if (checkDelta(room) < 0.005)
        sleep_ms(1);
    updateDelta(room);

    serverBeforeGameStep(room->time, &room->last_asteroid);
    gameStep(room->delta);
    serverAfterGameStep();

    return 0;
}

int guiRun(uint8_t client)
{
    srand(time(NULL));
    
    if(client)
        glfwRun(guiInit, clientLoop, NULL, keyCallback, mouseCallback, NULL, "ASTEROIDS! CLIENT");
    else
        while(!serverLoop());
    return 0;
}
