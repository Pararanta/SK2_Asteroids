#ifndef H_ASTEROIDS_RADIX
#define H_ASTEROIDS_RADIX
    #include <stdint.h>

    #define MAX_ENTITY_COUNT 1000000

    typedef struct {
        uint32_t index;
        float x;
        float y;
        uint8_t keys[4];
    } SortItem;

    typedef struct {
        uint32_t array_count;
        SortItem array[MAX_ENTITY_COUNT*2];
        uint32_t bucket_counts[256];
        SortItem buckets[256][MAX_ENTITY_COUNT*2];
    } RadixArray;

    int radixHashKey(float key, uint8_t * result);
    int radixSort(SortItem * array, uint32_t count);

#endif