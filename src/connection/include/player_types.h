#ifndef H_ASTEROIDS_PLAYER_TYPES
#define H_ASTEROIDS_PLAYER_TYPES
    
    #include <tinycthread.h>
    #include <stdint.h>
    #include <player_consts.h>

    /*
        commands player can execute are:
        * start game/join to room
        * exit game
        * request entity data
        * accelerate in direction
        * shoot at point
    */

    typedef struct {
        uint16_t command; // we can decipher size from command
    } NetworkRequestHeader;

    typedef struct {
        char room[ASTEROIDS_MAX_ROOM_NAME];
    } NetworkRequestBodyJoin;

    // checking which entity is player does not require parameter

    typedef struct {
        uint16_t entity;
    } NetworkRequestEntity;

    typedef struct {
        float dx;
        float dy;
    } NetworkRequestAccelerate;

    typedef struct {
        float x;
        float y;
    } NetworkRequestShoot;

    /*
        for server single response is enough, we assume that most of it will be updating enities, so we will use this request for other purpuses too:
        command:
            * 0 - repond with entity data
            * 1 - successfully joined room, entity field contains players entity
            * 2 - player lost
    */

    typedef struct {
        uint16_t command;
        uint16_t entity;
        float x;
        float y;
        float dx;
        float dy;
    } NetworkResponse;

    typedef struct {
        int socket;
        uint8_t playing;
        uint16_t room;
        uint16_t entity;

        struct {
            // we need just the latest room name
            char join[ASTEROIDS_MAX_ROOM_NAME];
            // we need just the latest acceleration received
            uint8_t d; // treated as boolean if new acceleration was received
            float dx;
            float dy;
            // we need just the latest shot location
            uint8_t shoot; // treated as boolean if new shot was received
            float shoot_x;
            float shoot_y;
            // queue with requests for entity data
            uint16_t entity_count;
            uint16_t entity[ASTEROIDS_MAX_ENTITIES_QUEUED];
        } requests;

        uint16_t reponse_count;
        NetworkResponse responses[ASTEROIDS_MAX_RESPONSES_QUEUED];

        mtx_t request_mtx;
        mtx_t reponse_mtx;
        cnd_t reponse_cnd; // not used by client as responses are read in game loop
        cnd_t request_cnd; // not used by server as requests are read in game loop
    } Player;

#endif