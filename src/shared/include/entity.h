#ifndef H_ASTEROIDS_ENTITY
#define H_ASTEROIDS_ENTITY

    #include <inttypes.h>

    typedef struct {
        uint16_t type;
        float x;
        float y;
        float dx;
        float dy;
    } Entity;

#endif