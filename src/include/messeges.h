#ifndef H_ASTEROIDS_MESSAGES
#define H_ASTEROIDS_MESSAGES

    #include <inttypes.h>

    // server -> client
    typedef struct {
        uint16_t entity;
        uint16_t type;
        float x;
        float y;
        float dx;
        float dy;
    } Response;

    // client->server
    typedef enum : uint16_t {
        REQUEST_ENTITY,
        MOVE,
        SHOOT
    } RequestType;

    typedef struct {
        RequestType type;
        union {
            uint16_t type; // used for REQUEST_ENTITY type
            struct point { // used for MOVE and SHOOT types
                float x;
                float y;
            }
        }
    } Request;
#endif