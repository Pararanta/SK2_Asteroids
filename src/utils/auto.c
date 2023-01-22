#include "include/auto.h"

int addAt(AutoArray * array, Item item, int index)
{
    for(int i = array->count; i > index; i--)
        array->array[i] = array->array[i-1];

    array->array[index] = item;
    array->count++;
    return 0;
}

int removeAt(AutoArray * array, Item item, int index)
{
    array->count--;
    for(int i = index; i < array->count; i++)
        array->array[i] = array->array[i+1];

    return 0;
}

int add(AutoArray * array, Item item)
{
    uint32_t index;
    binarySearch(array->array, array->count, item, &index);
    return addAt(array, item, index);
}
int remove(AutoArray * array, Item item)
{
    uint32_t index;
    binarySearch(array->array, array->count, item, &index);
    return removeAt(array, item, index);
}

int toggle(AutoArray * array, Item item)
{
    uint32_t index;
    if(binarySearch(array->array, array->count, item, &index))
        return addAt(array, item, index)+1;
    else
        return removeAt(array, item, index)-1;
}

int sort(AutoArray * array)
{
    return quickSort(array->array, array->count);
}

int has(AutoArray * array, Item item)
{
    uint32_t index;
    return !binarySearch(array->array, array->count, item, &index);
}

int push(AutoArray * array, Item item)
{
    array->array[array->count] = item;
    array->count++;
}
int pop(AutoArray * array)
{
    array->count--;
}