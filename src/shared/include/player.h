#ifndef H_ASTEROIDS_PLAYER
#define H_ASTEROIDS_PLAYER

    #include <inttypes.h>
    #include <socket.h>
    #include <messages.h>
    #include <tinycthread.h>

    #ifndef MAX_STORED_MESSAGES
        #define MAX_STORED_MESSAGES 255
    #endif

    #ifndef SHOOT_COOLDOWN
        #define SHOOT_COOLDOWN 1
    #endif

    typedef struct {
        uint16_t status; // 0 - uninitialize, 1 - connected
        sock_t fd;

        uint16_t request_cnt;
        uint16_t response_cnt;

        Request requests[MAX_STORED_MESSAGES];
        Response responses[MAX_STORED_MESSAGES];

        mtx_t request_mtx;
        mtx_t response_mtx;

        uint8_t listen; // 0 - do not listen, 1 - continue listening
        uint8_t connection_error; // 0 - no error, 1 - error

        thrd_t listening_thrd;
        thrd_t sending_thrd;
        cnd_t send_cnd;

        MessageType listening_type;
        MessageType sending_type;

        uint16_t entity;
        double last_shot;
    } Player;

    /**
    * @brief A function to atomically append array of requests to player
    * @param player Pointer to player which we want to initialize.
    * @param fd Pointer socket on which connection to player is
    */
    int initPlayer(Player * player, SOCKET fd, MessageType listen_type, MessageType send_type);

    /**
    * @brief A function to atomically append array of requests to player
    * @param player Pointer to player which we want to destroy.
    */
    int destroyPlayer(Player * player);

    /**
    * @brief A function to atomically append array of requests to player
    * @param player Pointer to player to which we append request.
    * @param request Pointer to array of requests which we add to player.
    * @param count Pointer to variable with size of request array.
    */
    void appendRequests(Player * player, Request * requests, uint16_t * count);

    /**
    * @brief A function to atomically append array of responses to player
    * @param player Pointer to player to which we append request.
    * @param request Pointer to array of responses which we add to player.
    * @param count Pointer to variable with size of response array.
    */
    void appendResponses(Player * player, Response * responses, uint16_t * count);


    /**
    * @brief A function to atomically gather current requests of a player, and clear them
    * @param player Pointer to player to which we gather requests from.
    * @param request [out] Pointer to array of requests which we put our requests to.
    * @param count [out] Pointer to variable to which we put count of consumed requests.
    */
    void consumeRequests(Player * player, Request * requests, uint16_t * count);

    /**
    * @brief A function to atomically gather current responses for a player, and clear them
    * @param player Pointer to player to which we gather requests from.
    * @param request [out] Pointer to array of responses which we put our responses to.
    * @param count [out] Pointer to variable to which we put count of consumed responses.
    */
    void consumeResponses(Player * player, Response * responses, uint16_t * count);

    /**
    * @brief A function that signals that we are ready to send data
    * @param player Pointer to player for which we start sending for
    */
    void startSending(Player * player);

#endif