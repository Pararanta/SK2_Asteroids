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

GLuint program;
GLuint coord_vbo, color_vbo;
uint8_t up, down, left, right;
double x, y;
uint8_t shoot;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP || key == GLFW_KEY_K)
        up = action == GLFW_PRESS;
    if (key == GLFW_KEY_S || key == GLFW_KEY_LEFT || key == GLFW_KEY_J)
        down = action == GLFW_PRESS;
    if (key == GLFW_KEY_A || key == GLFW_KEY_RIGHT || key == GLFW_KEY_H)
        left = action == GLFW_PRESS;
    if (key == GLFW_KEY_D || key == GLFW_KEY_DOWN || key == GLFW_KEY_L)
        right = action == GLFW_PRESS;
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

    return 0;
}

double last = 0.0;
double lastAsteroid = -60.0f;
uint32_t frame = 0;

double updateDelta()
{
    frame++;

    double now = glfwGetTime();
    double delta = now - last;
    last = now;

    return delta;
}

int clientLoop()
{
    clientBeforeGameStep();
    gameStep(updateDelta());
    clientAfterGameStep(!(frame%120), right - left, up - down, shoot, x, y);
    guiRender();
    shoot = 0;
    return 0;
}

int serverLoop()
{
    serverBeforeGameStep(last, &lastAsteroid);
    gameStep(updateDelta());
    serverAfterGameStep();
    guiRender();

    return 0;
}

int guiRun(uint8_t client)
{
    srand(time(NULL));
    
    if(client)
        glfwRun(guiInit, clientLoop, NULL, keyCallback, mouseCallback, NULL, "ASTEROIDS! CLIENT");
    else
        glfwRun(guiInit, serverLoop, NULL, NULL, NULL, NULL, "ASTEROIDS! SERVER");
    return 0;
}
