#ifndef H_ASTEROIDS_PLAYER
#define H_ASTEROIDS_PLAYER

    #include <inttypes.h>
    #include <socket.h>
    #include <messages.h>
    #include <tinycthread.h>

    typedef struct {
        SOCKET fd;
        
        uint16_t request_cnt;
        uint16_t response_cnt;

        Request requests[MAX_STORED_MESSAGES];
        Response responses[MAX_STORED_MESSAGES];

        mtx_t request_mtx;
        mtx_t response_mtx;

        cnd_t request_cnd;
        cnd_t response_cnd;

        uint16_t entity;
    } Player;

#endif