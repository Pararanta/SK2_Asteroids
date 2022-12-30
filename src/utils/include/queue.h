#ifndef H_ASTEROIDS_UTILS_QUEUE
#define H_ASTEROIDS_UTILS_QUEUE
    #include <stdint.h>

    #ifndef MAX_QUEUE_SIZE_BYTES
        #define MAX_QUEUE_SIZE_BYTES 1024
    #endif
    
    typedef enum QueueMode {LIMIT, ROTATE} QueueMode;

    typedef struct Queue
    {
        uint8_t buffer[MAX_QUEUE_SIZE_BYTES];
        uint32_t element_size_bytes;
        uint32_t max_elements;
        uint32_t push_cursor;
        uint32_t pop_cursor;
        uint32_t push_pop_balance;
        QueueMode mode;
    }
    Queue;

    void queueInit(Queue * queue, uint32_t element_size_bytes, QueueMode mode);
    void * queuePush(Queue * queue, void * element);
    void * queuePeek(Queue * queue);
    void * queuePop(Queue * queue);
#endif