#ifndef H_ASTEROIDS_ROOM
#define H_ASTEROIDS_ROOM

    #ifndef MAX_ROOM_NAME
        #define MAX_ROOM_NAME 255
    #endif

    #ifndef MAX_ENTITY_COUNT
        #define MAX_ENTITY_COUNT 255
    #endif

    #ifndef MAX_PLAYER_COUNT
        #define MAX_PLAYER_COUNT 127
    #endif

    #ifndef MAX_COLLISIONS_PER_STEP
        #define MAX_COLLISIONS_PER_STEP 15
    #endif

    #include <inttypes.h>
    #include <entity.h>
    #include <player.h>
    #include <tinycthread.h>

    typedef struct {
        uint8_t status;
        mtx_t edit;
        char name[MAX_ROOM_NAME];
        Entity entities[MAX_ENTITY_COUNT];
        union
        {
            Player players[MAX_PLAYER_COUNT];
            Player player;
        };
        uint16_t collision_cnt;
        struct {
            uint16_t a;
            uint16_t b;
        } collisions[MAX_COLLISIONS_PER_STEP];
    } Room;

#endif