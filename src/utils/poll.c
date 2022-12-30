#include "poll.h"

void swap(Poll * poll, uint32_t a, uint32_t b);
int bubble(Poll * poll, uint32_t index);

int pollRequest(Poll * poll, uint32_t * index)
{
    if(poll->used_indexes >= poll->count)
        return 1;

    *index = poll->indexes[poll->used_indexes];
    poll->used_indexes++;
    if(*index > poll->max_index)
        poll->max_index = *index;

    return 0;
}


int pollReturn(Poll * poll, uint32_t index)
{
    poll->used_indexes--;
    return bubbleUp(poll, index);
}

int bubbleUp(Poll * poll, uint32_t index)
{
    uint32_t removed_index = poll->indexes[poll->used_indexes];
    poll->indexes[poll->used_indexes] = index;

    uint32_t final_index = poll->used_indexes;

    int result;
    while((result = bubble(poll, final_index, &final_index)) < 0);

    if(poll->max_index == index)
        poll->max_index = poll->indexes[final_index]-1;

    if(poll->max_index == index)
        poll->max_index--;

    if(result > 0)
        bubbleUp(poll, removed_index);

    return result;
}

int bubble(Poll * poll, uint32_t * index)
{
    *index++;

    if(poll->indexes[*index-1] > poll->indexes[*index])
        return 0;

    if(poll->indexes[*index-1] == poll->indexes[*index])
        return 1;

    swap(poll, *index-1, *index);
    return -1;
}

void swap(Poll * poll, uint32_t a, uint32_t b)
{
    uint32_t temp = poll->indexes[a];
    poll->indexes[a] = poll->indexes[b];
    poll->indexes[b] = temp;
}