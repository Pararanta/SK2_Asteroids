#ifndef H_ASTEROIDS_SEARCH
#define H_ASTEROIDS_SEARCH
    #include <stdint.h>

    typedef struct {
        uint32_t index;
        float coords[4];
        int coord;
        int checked;
        //uint8_t keys[4];
    } Item;

    Item createItem(uint32_t index);
    int setKeys(Item * item, float key);
    int binarySearch(Item array_sorted[], uint32_t array_count, Item item_searched, uint32_t * result);
    int quickSort(Item * arr);
#endif