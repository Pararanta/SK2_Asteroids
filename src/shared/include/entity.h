#ifndef H_ASTEROIDS_ENTITY
#define H_ASTEROIDS_ENTITY

    #include <inttypes.h>

    #define ENTITY_TYPE_CNT 6

    typedef struct {
        uint16_t type;
        float x;
        float y;
        float dx;
        float dy;
    } Entity;

    uint8_t checkCollision(Entity * a, Entity * b);
    int addToAttribute(Entity * entity, uint16_t * pointer, float * position, float * color);
#endif