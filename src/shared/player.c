#include <player.h>
#include <extensions.h>

/**
* @brief A function to start loop reciving requests from the player
* @param player Pointer to player for which we start listening for (need to be cast to void pointer to be compatible with threads api)
*/
int listenRequests(void * player_v);

/**
* @brief A function to start loop reciving responses for the player
* @param player Pointer to player for which we start listening for (need to be cast to void pointer to be compatible with threads api)
*/
int listenResponses(void * player_v);

/**
* @brief A function to start loop waiting for send signal and sending requests on it
* @param player Pointer to player for which we start sending for (need to be cast to void pointer to be compatible with threads api)
*/
int sendRequests(void * player_v);

/**
* @brief A function to start loop waiting for send signal and sending responses on it
* @param player Pointer to player for which we start sending for (need to be cast to void pointer to be compatible with threads api)
*/
int sendResponses(void * player_v);

int initPlayer(Player * player, sock_t fd, MessageType listen_type, MessageType send_type)
{
    if(player->status)
        return 1;

    player->fd = fd;
    player->connection_error = 0;
    player->request_cnt = 0;
    player->response_cnt = 0;

    mtx_init(&player->request_mtx, mtx_plain);
    mtx_init(&player->response_mtx, mtx_plain);
    mtx_init(&player->send_mtx, mtx_plain);
    cnd_init(&player->send_cnd);

    player->listen = 1;
    player->listening_type = listen_type;
    player->sending_type = send_type;

    thrd_start_t listen_fn;
    thrd_start_t send_fn;

    if(listen_type == REQUEST)
        listen_fn = &listenRequests;
    if(listen_type == RESPONSE)
        listen_fn = &listenResponses;

    if(send_type == REQUEST)
        send_fn = &sendRequests;
    if(send_type == RESPONSE)
        send_fn = &sendResponses;

    thrd_create(&player->listening_thrd, listen_fn, (void*)player);
    thrd_create(&player->sending_thrd, send_fn, (void*)player);

    player->status = 1;

    return 0;
}

int destroyPlayer(Player * player)
{
    if(!player->status)
        return 1;

    player->listen = 0;
    player->connection_error = 0;

    cnd_broadcast(&player->send_cnd);
    thrd_yield();

    socketClose(player->fd);

    thrd_join(player->listening_thrd, NULL);
    thrd_join(player->sending_thrd, NULL);

    cnd_destroy(&player->send_cnd);
    mtx_destroy(&player->request_mtx);
    mtx_destroy(&player->response_mtx);
    mtx_destroy(&player->send_mtx);

    player->status = 0;

    return 0;
}

void appendRequests(Player * player, Request * requests, uint16_t * count)
{
    mtx_lock(&player->request_mtx);

    for(int i = 0; i < *count; i++)
    {
        player->requests[player->request_cnt%MAX_STORED_MESSAGES] = requests[i];
        player->request_cnt++;
    }

    mtx_unlock(&player->request_mtx);
}

void appendResponses(Player * player, Response * responses, uint16_t * count)
{
    mtx_lock(&player->response_mtx);

    for(int i = 0; i < *count; i++)
    {
        player->responses[player->response_cnt%MAX_STORED_MESSAGES] = responses[i];
        player->response_cnt++;
    }
    
    mtx_unlock(&player->response_mtx);
}

void consumeRequests(Player * player, Request * requests, uint16_t * count)
{
    mtx_lock(&player->request_mtx);

    *count = player->request_cnt;
    if(*count > MAX_STORED_MESSAGES)
        *count = MAX_STORED_MESSAGES;
    player->request_cnt = 0;

    for(int i = 0; i < *count; i++)
        requests[i] = player->requests[i];

    mtx_unlock(&player->request_mtx);
}

void consumeResponses(Player * player, Response * responses, uint16_t * count)
{
    mtx_lock(&player->response_mtx);

    *count = player->response_cnt;
    if(*count > MAX_STORED_MESSAGES)
        *count = MAX_STORED_MESSAGES;
    player->response_cnt = 0;

    for(int i = 0; i < *count; i++)
        responses[i] = player->responses[i];

    mtx_unlock(&player->response_mtx);
}

int listenRequests(void * player_v)
{
    Player * player = player_v;

    uint16_t one = 1;
    while(player->listen && !player->connection_error)
    {
        int result;
        Request request;

        result = recv(player->fd, &request, sizeof(Request), MSG_WAITALL);
        // end listening if encountered end of input or error
        if(result < 0 || result == 0)
        {
            player->connection_error = 1;
            break;
        }

        request = ntoh_req(request);
        appendRequests(player, &request, &one);
    }
    return !player->listen;
}

int listenResponses(void * player_v)
{
    Player * player = player_v;

    uint16_t one = 1;
    while(player->listen && !player->connection_error)
    {
        int result;
        Response response;

        result = recv(player->fd, &response, sizeof(Response), MSG_WAITALL);

        // end listening if encountered end of input or error
        if(result < 0 || result == 0)
        {
            player->connection_error = 1;
            break;
        }

        response = ntoh_res(response);
        appendResponses(player, &response, &one);
    }
    
    return !player->listen;
}

int sendRequests(void * player_v)
{
    Player * player = player_v;

    Request requests[MAX_STORED_MESSAGES];
    uint16_t count;

    while(player->listen && !player->connection_error)
    {
        consumeRequests(player, requests, &count);

        int result = 1;
        for(int i = 0; i < count; i++)
        {
            Request request = hton_req(requests[i]);
            int subresult = send(player->fd, &request, sizeof(Request), 0);
            if(subresult < result)
                result = subresult;
        }

        if(result < 0 || result == 0)
        {
            player->connection_error = 1;
            break;
        }

        cnd_waitonce(&player->send_cnd, &player->send_mtx);
    }

    return !player->listen;
}

int sendResponses(void * player_v)
{
    Player * player = player_v;

    Response responses[MAX_STORED_MESSAGES];
    uint16_t count;

    while(player->listen && !player->connection_error)
    {
        consumeResponses(player, responses, &count);
        cnd_signal(&player->send_cnd);

        int result = 1;
        for(int i = 0; i < count; i++)
        {
            Response response = hton_res(responses[i]);
            int subresult = send(player->fd, &response, sizeof(Response), 0);
            if(subresult < result)
                result = subresult;
        }

        if(result < 0 || result == 0)
        {
            player->connection_error = 1;
            break;
        }

        cnd_waitonce(&player->send_cnd, &player->send_mtx);
    }

    return !player->listen;
}

void startSending(Player * player)
{
    cnd_signal(&player->send_cnd);
}
