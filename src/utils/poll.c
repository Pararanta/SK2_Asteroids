#include "poll.h"

int pollInitIndexes(uint32_t * indexes, uint32_t count);
void swap(Poll * poll, uint32_t a, uint32_t b);
int bubbleUp(Poll * poll, uint32_t index);
int bubble(Poll * poll, uint32_t * index);

int pollInit(Poll * poll, uint32_t * indexes, uint32_t count)
{
    pollInitIndexes(indexes, count);
    poll->indexes = indexes;
    poll->count = count;
    poll->max_index = 0;
    poll->used_indexes = 0;

    return 0;
}

int pollRequest(Poll * poll, uint32_t * index)
{
    if(poll->used_indexes >= poll->count)
        return 1;

    uint32_t requested = poll->indexes[poll->used_indexes];
    poll->used_indexes++;
    if(requested > poll->max_index)
        poll->max_index = requested;

    if(index == NULL)
        return 0;

    *index = requested;

    return 0;
}

int pollReturn(Poll * poll, uint32_t index)
{
    int error = bubbleUp(poll, index);
    if(!error)
        poll->used_indexes--;
    return error;
}

int pollInitIndexes(uint32_t * indexes, uint32_t count)
{
    for(uint32_t i = 0; i < count; i++)
        indexes[i] = i;
    return 0;
}

int bubbleUp(Poll * poll, uint32_t index)
{
    if (!poll->used_indexes)
        return 2;

    if (index > poll->max_index)
        return 3;

    if (index >= poll->count)
        return 4;

    uint32_t removed_index = poll->indexes[poll->used_indexes-1];
    poll->indexes[poll->used_indexes-1] = index;

    uint32_t final_index = poll->used_indexes-1;

    int result = -1;
    while(result < 0)
    {
        result = bubble(poll, &final_index);
    }

    if (poll->used_indexes - 1 > 0)
    {
        if (poll->max_index == index)
            poll->max_index = poll->indexes[poll->used_indexes - 1] - 1;

        if (poll->max_index == index)
            poll->max_index--;
    }
    else
        poll->max_index = 0;

    if(result == 1)
        bubbleUp(poll, removed_index);

    return result;
}

int bubble(Poll * poll, uint32_t * index)
{
    (*index)++;

    if (!poll->used_indexes)
        return 2;

    if (*index > poll->max_index)
        return 0;

    if (*index >= poll->count)
        return 0;

    uint32_t next_value = poll->indexes[*index];
    uint32_t current_value = poll->indexes[(*index)-1];

    if(current_value < next_value)
        return 0;

    if(current_value == next_value)
        return 1;

    swap(poll, (*index)-1, *index);
    return -1;
}

void swap(Poll * poll, uint32_t a, uint32_t b)
{
    uint32_t temp = poll->indexes[a];
    poll->indexes[a] = poll->indexes[b];
    poll->indexes[b] = temp;
}