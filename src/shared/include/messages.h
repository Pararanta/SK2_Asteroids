#ifndef H_ASTEROIDS_MESSAGES
#define H_ASTEROIDS_MESSAGES

    #include <inttypes.h>
    #include <socket.h>
    #include <extensions.h>
    #include <entity.h>

    typedef enum {
        REQUEST,
        RESPONSE
    } MessageType;

    // server -> client
    typedef struct {
        uint16_t index;
        Entity entity;
    } Response;

    // client -> server
    typedef enum : uint16_t {
        REQUEST_ENTITY = 0,
        MOVE = 1,
        SHOOT = 2
    } RequestType;

    typedef struct {
        RequestType type;
        union {
            uint16_t index; // used for REQUEST_ENTITY type
            Point point;
        };
    } Request;

    Request ntoh_req(Request request);
    Request hton_req(Request request);

    Response ntoh_res(Response response);
    Response hton_res(Response response);

#endif