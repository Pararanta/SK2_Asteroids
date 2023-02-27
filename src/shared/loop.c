#include <loop.h>
#include <extensions.h>
#include <stdlib.h>

tss_t room_key;
static Request all_entities_requests[MAX_ENTITY_COUNT];
static uint16_t all_entities_count = MAX_ENTITY_COUNT;

void initRooms()
{

    tss_create(&room_key, NULL);
    for(uint16_t i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        all_entities_requests[i].type = REQUEST_ENTITY;
        all_entities_requests[i].index = i;
    }
}

Room * getThreadRoom()
{
    return (Room*)tss_get(room_key);
}

int initRoomThread(void * room_v)
{
    tss_set(room_key, room_v);

    return 0;
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

        if (entity->x < 0 && entity->dx < 0)
        {
            if(entity->type == 1)
                entity->x = 0;
            entity->dx = -entity->dx;
        }

        if (entity->x > 1 && entity->dx > 0)
        {
            if (entity->type == 1)
                entity->x = 1;
            entity->dx = -entity->dx;
        }

        if (entity->y < 0 && entity->dy < 0)
        {
            if (entity->type == 1)
                entity->y = 0;
            entity->dy = -entity->dy;
        }

        if (entity->y > 1 && entity->dy > 0)
        {
            if (entity->type == 1)
                entity->y = 1;
            entity->dy = -entity->dy;
        }
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
    printf("%d\n", response_cnt);
    for(int i = 0; i < response_cnt; i++)
        room->entities[responses[i].index] = responses[i].entity;

    return 0;
}

int clientAfterGameStep(uint8_t requestAll, float dx, float dy, uint8_t shootState, float tx, float ty)
{
    static uint16_t two = 2;
    static uint16_t one = 1;

    Room * room = getThreadRoom();

    if(!requestAll)
        for(int i = 0; i < room->collision_cnt; i++)
        {
            Request collided[2];
            collided[0].type = REQUEST_ENTITY;
            collided[0].index = room->collisions[i].a;
            collided[1].type = REQUEST_ENTITY;
            collided[1].index = room->collisions[i].b;
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
        move.point.x = dx;
        move.point.y = dy;
        appendRequests(&room->player, &move, &one);
    }

    if(shootState)
    {
        Request shoot;
        shoot.type = SHOOT;
        shoot.point.x = tx;
        shoot.point.y = ty;
        appendRequests(&room->player, &shoot, &one);
    }

    startSending(&room->player);

    return 0;
}

float frandom()
{
    return (float)rand() / RAND_MAX;
}

int serverBeforeGameStep(double time, double* lastAsteroid)
{
    Room* room = getThreadRoom();
    uint16_t _one = 1;

    if ((*lastAsteroid) + SHOOT_COOLDOWN * 4 < time)
    {
        float axis = frandom();
        float position = frandom();
        Point normalized = normalize(frandom() - 0.5f, frandom() - 0.5f);

        Entity asteroid = {3, 0, 0, normalized.x*ASTEROID_SPEED, normalized.y*ASTEROID_SPEED};
        
        if(axis < 0.25f)
        {
            asteroid.x = position;
            asteroid.y = -1.0f;
        }
        else if(axis < 0.5f)
        {
            asteroid.x = position;
            asteroid.y = 2.0f;
        }
        else if(axis < 0.75)
        {
            asteroid.y = position;
            asteroid.x = -1.0f;
        }
        else
        {
            asteroid.y = position;
            asteroid.x = 2.0f;
        }

        instantiate(room, asteroid);

        (*lastAsteroid) = time;
    }

    for(int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        if(!room->players[i].status)
            continue;

        Request requests[MAX_STORED_MESSAGES];
        uint16_t request_cnt;
        consumeRequests(&room->players[i], requests, &request_cnt);

        for(uint16_t j = 0; j < request_cnt; j++)
        {
            if(requests[j].type == REQUEST_ENTITY)
            {
                Response response = {0};
                response.index = requests[j].index;
                appendResponses(&room->players[i], &response, &_one);
            }

            if(requests[j].type == MOVE)
            {
                Point normalized = normalize(requests[j].point.x, requests[j].point.y);
                room->entities[room->players[i].entity].dx = normalized.x*PLAYER_SPEED;
                room->entities[room->players[i].entity].dy = normalized.y*PLAYER_SPEED;

                update(room, room->players[i].entity);
            }

            if(requests[j].type == SHOOT && room->players[i].last_shot + SHOOT_COOLDOWN < time)
            {
                room->players[i].last_shot = time;

                float sx = room->entities[room->players[i].entity].x;
                float sy = room->entities[room->players[i].entity].y;
                printf("%f %f, %f %f, %f %f\n", sx, sy, requests[j].point.x, requests[j].point.y, requests[j].point.x - sx, requests[j].point.y - sy);
                Point normalized = normalize(requests[j].point.x - sx, requests[j].point.y - sy);

                Entity projectile = {2, sx, sy, normalized.x*BULLET_SPEED, normalized.y*BULLET_SPEED};

                instantiate(room, projectile);
            }
        }
    }
}

typedef struct {
    uint8_t success;
    uint16_t a;
    uint16_t b;
} Matched;

Matched match(Room * room, uint16_t a, uint16_t b, uint16_t type_a, uint16_t type_b)
{
    Matched result = {0};

    if(room->entities[a].type == type_a && room->entities[b].type == type_b)
    {
        result.success = 1;
        result.a = a;
        result.b = b;
    }

    if(room->entities[b].type == type_a && room->entities[a].type == type_b)
    {
        result.success = 1;
        result.a = b;
        result.b = a;
    }

    return result;
}

void entityLost(Room * room, uint16_t index)
{
    for(int i = 0; i < MAX_PLAYER_COUNT; i++)
        if(room->players[i].entity == index)
            destroyPlayer(&room->players[i]);

    destroy(room, index);
}

void entityHit(Room * room, uint16_t bullet, uint16_t asteroid)
{
    uint8_t createSmaller = room->entities[asteroid].type < 5;

    if(createSmaller)
    {
        Entity smallerAsteroid = {0};
        smallerAsteroid.type = room->entities[asteroid].type + 1;
        smallerAsteroid.x = room->entities[asteroid].x;
        smallerAsteroid.y = room->entities[asteroid].y;

        smallerAsteroid.dx = -room->entities[asteroid].dy;
        smallerAsteroid.dy = room->entities[asteroid].dx;
        instantiate(room, smallerAsteroid);

        smallerAsteroid.dx = -smallerAsteroid.dx;
        smallerAsteroid.dy = -smallerAsteroid.dy;
        instantiate(room, smallerAsteroid);
    }

    destroy(room, bullet);
    destroy(room, asteroid);
}

int serverAfterGameStep()
{
    Room * room = getThreadRoom();

    for(int i = 0; i < room->collision_cnt; i++)
        {
            Matched test = {0};

            Matched playerLostT1 = match(room, room->collisions[i].a, room->collisions[i].b, 1, 3);
            Matched playerLostT2 = match(room, room->collisions[i].a, room->collisions[i].b, 1, 4);
            Matched playerLostT3 = match(room, room->collisions[i].a, room->collisions[i].b, 1, 5);

            if(playerLostT1.success)
                entityLost(room, playerLostT1.a);

            if(playerLostT2.success)
                entityLost(room, playerLostT2.a);

            if(playerLostT3.success)
                entityLost(room, playerLostT3.a);

            Matched bulletHitT1 = match(room, room->collisions[i].a, room->collisions[i].b, 2, 3);
            Matched bulletHitT2 = match(room, room->collisions[i].a, room->collisions[i].b, 2, 4);
            Matched bulletHitT3 = match(room, room->collisions[i].a, room->collisions[i].b, 2, 5);

            if(bulletHitT1.success)
                entityHit(room, bulletHitT1.a, bulletHitT1.b);

            if(bulletHitT2.success)
                entityHit(room, bulletHitT2.a, bulletHitT2.b);

            if(bulletHitT3.success)
                entityHit(room, bulletHitT3.a, bulletHitT3.b);
        }

    applyDestroy(room);

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        if (!room->players[i].status)
            continue;

        Response responses[MAX_STORED_MESSAGES];
        uint16_t response_cnt;
        consumeResponses(&room->players[i], responses, &response_cnt);
        for (int i = 0; i < response_cnt; i++)
            update(room, responses[i].index);

        startSending(&room->players[i]);
    }
}