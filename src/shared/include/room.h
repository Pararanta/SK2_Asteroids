#ifndef H_ASTEROIDS_ROOM
#define H_ASTEROIDS_ROOM

    #ifndef MAX_ROOM_NAME
        #define MAX_ROOM_NAME 255
    #endif

    #ifndef MAX_ENTITY_COUNT
        #define MAX_ENTITY_COUNT 127
    #endif

    #ifndef MAX_ENTITY_QUEUED
        #define MAX_ENTITY_QUEUED 15
    #endif

    #ifndef MAX_PLAYER_COUNT
        #define MAX_PLAYER_COUNT 63
    #endif

    #ifndef MAX_COLLISIONS_PER_STEP
        #define MAX_COLLISIONS_PER_STEP 15
    #endif

    #ifndef ASTEROID_SPEED
        #define ASTEROID_SPEED 0.125
    #endif

    #ifndef PLAYER_SPEED
        #define PLAYER_SPEED 1
    #endif

    #ifndef BULLET_SPEED
        #define BULLET_SPEED 2
    #endif

    #include <inttypes.h>
    #include <entity.h>
    #include <player.h>
    #include <tinycthread.h>

    typedef struct {
        uint8_t status;
        mtx_t edit;
        thrd_t room_thrd;
        char name[MAX_ROOM_NAME+1];
        Entity entities[MAX_ENTITY_COUNT];
        uint16_t to_destroy_cnt;
        uint16_t to_destroy[MAX_ENTITY_QUEUED];
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

        double time;
        double delta;
        double last_asteroid;
        uint16_t frame;
    } Room;

    uint16_t instantiate(Room * room, Entity prefab);
    void destroy(Room * room, uint16_t index);
    void applyDestroy(Room * room);
    void update(Room * room, uint16_t index);
    int initRoom(Room * room, const char * name, size_t name_len);
    int destroyRoom(Room * room);

    #endif