#include <entity.h>
#include <messages.h>

float ENTITY_RADIUS[ENTITY_TYPE_CNT] = {0.0f, 0.03125f, 0.015625f, 0.125f, 0.0625f, 0.03125f};
float ENTITY_COLOR[ENTITY_TYPE_CNT][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
};

uint8_t checkCollision(Entity * a, Entity * b)
{
    float xd = a->x - b->x;
    float yd = a->y - b->y;
    float rs = ENTITY_RADIUS[a->type] + ENTITY_RADIUS[b->type];
    return xd*xd + yd*yd < rs*rs;
}

int addToAttribute(Entity * entity, uint16_t * pointer, float * position, float * color)
{
    if(!entity->type)
        return 1;

    uint16_t current = (*pointer)*3;

    position[current]   = entity->x;
    position[current+1] = entity->y;
    position[current+2] = ENTITY_RADIUS[entity->type];

    color[current]      = ENTITY_COLOR[entity->type][0];
    color[current+1]    = ENTITY_COLOR[entity->type][1];
    color[current+2]    = ENTITY_COLOR[entity->type][2];

    (*pointer)++;

    return 0;
}