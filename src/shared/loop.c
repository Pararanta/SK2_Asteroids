#include <loop.h>

tss_t room_key;
static Request all_entities_requests[MAX_ENTITY_COUNT];
static const uint16_t all_entities_count = MAX_ENTITY_COUNT;

void initRooms()
{

    tss_create(&room_key, NULL);
    for(uint16_t i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        all_entities_requests[i].type = REQUEST_ENTITY;
        all_entities_requests[i].entity = i;
    }
}

Room * getThreadRoom()
{
    return (Room*)tss_get(room_key);
}

int initRoomThread(void * room_v)
{
    tss_set(room_key, room_v);
}

int gameStep(float delta)
{
    Room * room = getThreadRoom();

    for(uint16_t i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        Entity * entity = &room->entities[i];
        if(!entity->type)
            continue;

        entity->x += entity->dx*delta;
        entity->y += entity->dy*delta;

        if(entity->x < 0 && entity->dx < 0)
            entity->dx = -entity->dx;

        if(entity->x > 1 && entity->dx > 0)
            entity->dx = -entity->dx;

        if(entity->y < 0 && entity->dy < 0)
            entity->dy = -entity->dy;

        if(entity->y > 1 && entity->dy > 0)
            entity->dy = -entity->dy;
    }

    room->collision_cnt = 0;
    for(uint16_t i = 0; i < MAX_ENTITY_COUNT; i++)
        for(uint16_t j = i+1; j < MAX_ENTITY_COUNT; j++)
        {
            Entity * a = &room->entities[i];
            Entity * b = &room->entities[j];

            if(!a->type || !b->type)
                continue;

            if(!checkCollision(a, b))
                continue;

            room->collisions[room->collision_cnt].a = i;
            room->collisions[room->collision_cnt].b = j;
            room->collision_cnt++;
        }

    return 0;
}

int clientBeforeGameStep()
{
    Room * room = getThreadRoom();

    Response responses[MAX_STORED_MESSAGES];
    uint16_t response_cnt;
    consumeResponses(&room->player, responses, &response_cnt);

    for(int i = 0; i < response_cnt; i++)
    {
        Entity * entity = &room->entities[responses[i].entity];
        entity->type = responses[i].type;
        entity->x = responses[i].x;
        entity->y = responses[i].y;
        entity->dx = responses[i].dx;
        entity->dy = responses[i].dy;
    }
}

int clientAfterGameStep(uint8_t requestAll, float dx, float dy, uint8_t shootState, float tx, float ty)
{
    static const uint16_t two = 2;
    static const uint16_t one = 1;

    Room * room = getThreadRoom();

    if(!requestAll)
        for(int i = 0; i < room->collision_cnt; i++)
        {
            Request collided[2];
            collided[0].type = REQUEST_ENTITY;
            collided[0].entity = room->collisions[i].a;
            collided[1].type = REQUEST_ENTITY;
            collided[1].entity = room->collisions[i].b;
            appendRequests(&room->player, collided, &two);
        }
    else
        {
            appendRequests(&room->player, all_entities_requests, &all_entities_count);
        }
    
    if(dx != room->entities[room->player.entity].dx || dy != room->entities[room->player.entity].dy)
    {
        Request move;
        move.type = MOVE;
        move.vector.x = dx;
        move.vector.y = dy;
        appendRequests(&room->player, &move, &one);
    }

    if(shootState)
    {
        Request shoot;
        shoot.type = SHOOT;
        shoot.vector.x = tx;
        shoot.vector.y = ty;
        appendRequests(&room->player, &shoot, &one);
    }

    startSending(&room->player);
}