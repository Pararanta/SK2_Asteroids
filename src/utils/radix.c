#include "include/radix.h"

int radixHashKey(float key, uint8_t * result)
{
    uint32_t cast = *(uint32_t *)&key;
    if(key < 0.0f)
        cast = ~cast;
    else
        cast ^= 1<<31;

    uint8_t * divide = &cast;

    for(int i = 0; i < 4; i++)
        result[i] = divide[i];
}