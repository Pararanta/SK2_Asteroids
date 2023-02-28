#include <room.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int appendToAllPlayers(Room * room, Response * response)
{
    uint16_t _one = 1;
    for(int i = 0; i < MAX_PLAYER_COUNT; i++) 
    {
        if(!room->players[i].status)
            continue;
        appendResponses(&room->players[i], response, &_one);
    }

    return 0;
}

uint16_t instantiate(Room * room, Entity prefab)
{
    uint16_t result = MAX_ENTITY_COUNT;
    mtx_lock(&room->edit);

    for(uint16_t i = 0; i < MAX_ENTITY_COUNT; i++)
        if(!room->entities[i].type)
        {
            result = i;
            room->entities[i] = prefab;
            break;
        }

    mtx_unlock(&room->edit);

    if(result == MAX_ENTITY_COUNT)
        return result;

    Response response = {0};

    response.index = result;
    response.entity = room->entities[result];
    appendToAllPlayers(room, &response);
    return result;
}

void update(Room * room, uint16_t index)
{
    mtx_lock(&room->edit);

    Response response = {0};

    response.index = index;
    response.entity = room->entities[index];

    appendToAllPlayers(room, &response);

    mtx_unlock(&room->edit);
}

void destroy(Room * room, uint16_t index)
{
    mtx_lock(&room->edit);

    room->to_destroy[room->to_destroy_cnt] = index;
    room->to_destroy_cnt++;

    mtx_unlock(&room->edit);
}

void applyDestroy(Room * room)
{
    for(int i = 0; i < room->to_destroy_cnt; i++)
    {
        room->entities[room->to_destroy[i]].type = 0;
        Response response = {0};

        response.index = room->to_destroy[i];

        appendToAllPlayers(room, &response);
    }

    room->to_destroy_cnt = 0;
}


int initRoom(Room * room, const char * name, size_t name_len)
{
    if(room->status)
        return 1;

    room->status = 1;
    room->frame = -1;

    snprintf(room->name, name_len+1, "%s", name);
    room->name[name_len] = '\0';

    mtx_init(&room->edit, mtx_plain);

    return 0;
}

int destroyRoom(Room * room)
{
    if(!room->status)
        return 1;

    room->status = 0;
    mtx_destroy(&room->edit);
    for(int i = 0; i < MAX_PLAYER_COUNT; i++)
        destroyPlayer(&room->players[i]);

    return 0;
}