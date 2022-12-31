#ifndef H_ASTEROIDS_UTILS_INTERPOLATE
#define H_ASTEROIDS_UTILS_INTERPOLATE
    #include <stdint.h>

    typedef struct Poll
    {
        uint32_t * indexes;
        uint32_t count;
        uint32_t max_index;
        uint32_t used_indexes;
    } Poll;

    int pollInit(Poll * poll, uint32_t * indexes, uint32_t count);
    int pollRequest(Poll * poll, uint32_t * index);
    int pollReturn(Poll * poll, uint32_t index);

#endif