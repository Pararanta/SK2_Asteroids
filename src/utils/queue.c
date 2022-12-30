#include "queue.h"
#include <string.h>

void queueInit(Queue * queue, uint32_t element_size_bytes, QueueMode mode)
{
    queue->push_pop_balance = 0;
    queue->push_cursor = 0;
    queue->pop_cursor = 0;
    queue->element_size_bytes = element_size_bytes;
    queue->max_elements = (uint32_t)(MAX_QUEUE_SIZE_BYTES/element_size_bytes);
    queue->mode = mode;
}

void * queuePush(Queue * queue, void * element)
{

    if(queue->push_pop_balance == queue->max_elements && queue->mode != ROTATE)
        return NULL;

    memcpy_s(
        &queue->buffer[queue->push_cursor],
        MAX_QUEUE_SIZE_BYTES - queue->push_cursor,
        element,
        queue->element_size_bytes
    );

    void* pushed = &queue->buffer[queue->push_cursor];

    if(queue->push_pop_balance < queue->max_elements)
        queue->push_pop_balance++;

    queue->push_cursor += queue->element_size_bytes;
    if(queue->push_cursor+queue->element_size_bytes > MAX_QUEUE_SIZE_BYTES)
        queue->push_cursor = 0;

    if(queue->push_pop_balance == queue->max_elements)
        queue->pop_cursor = queue->push_cursor;

    return pushed;
}

void * queuePeek(Queue * queue)
{
    if(!queue->push_pop_balance)
        return NULL;
    return &queue->buffer[queue->pop_cursor];
}

void * queuePop(Queue * queue)
{
    if(!queue->push_pop_balance)
        return NULL;
    
    void * popped = &queue->buffer[queue->pop_cursor];

    queue->push_pop_balance--;

    if(queue->pop_cursor >= queue->element_size_bytes)
        queue->pop_cursor -= queue->element_size_bytes;
    else
        queue->pop_cursor = (queue->max_elements-1) * queue->element_size_bytes;

    if (!queue->push_pop_balance)
        queue->pop_cursor = queue->push_cursor;

    return popped;
}
