#ifndef H_ASTEROIDS_AUTO
#define H_ASTEROIDS_AUTO
    #include "./search.h"

    #define MAX_ENTITY_COUNT 1000000
    
    typedef struct {
        uint32_t count;
        Item array[MAX_ENTITY_COUNT*2];
    } AutoArray;

int add(AutoArray * array, Item item);
int remove(AutoArray * array, Item item);
int toggle(AutoArray * array, Item item);
int push(AutoArray * array, Item item);
int has(AutoArray * array, Item item);
int pop(AutoArray * array);
int sort(AutoArray * array);
#endif