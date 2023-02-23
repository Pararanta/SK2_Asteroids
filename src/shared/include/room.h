#ifndef H_ASTEROIDS_ENTITY
#define H_ASTEROIDS_ENTITY

    #ifndef MAX_ROOM_NAME
        #define MAX_ROOM_NAME 255
    #endif

    #ifndef MAX_ENTITY_COUNT
        #define MAX_ENTITY_COUNT 2047
    #endif

    #ifndef MAX_PLAYER_COUNT
        #define MAX_PLAYER_COUNT 127
    #endif

    #include <inttypes.h>
    #include <entity.h>
    #include <tinycthread.h>

    typedef struct {
        uint8_t status;
        mtx_t edit;
        char name[MAX_ROOM_NAME];
        Entity entities[MAX_ENTITY_COUNT];
        uint8_t player_status[MAX_PLAYER_COUNT];
        uint16_t players[MAX_PLAYER_COUNT];
    } Room;

#endif