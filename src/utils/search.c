#include "include/search.h"

Item createItem(uint32_t index)
{
    Item result = {0};
    return result;
}

int setKeys(Item * item, float key)
{
    return 0;
}

int binarySearch(Item array_sorted[], uint32_t array_count, Item item_searched, uint32_t * result)
{
    return binarySearchHelper(array_sorted, 0, array_count-1, item_searched, result);
}

int binarySearchHelper(Item array_sorted[], uint32_t start, uint32_t end, Item searched, uint32_t * result)
{
    if(start > end)
    {
        *result = start;
        return 1;
    }

    uint32_t center = (start+end)/2;

    if(array_sorted[center].index == searched.index && array_sorted[center].coord == searched.coord)
    {
        *result = center;
        return 0;
    }

    if(searched.coords[searched.coord] > array_sorted[center].coords[array_sorted[center].coord])
        return binarySearchHelper(array_sorted, center+1, end, searched, result);
    else
        return binarySearchHelper(array_sorted, start, center-1, searched, result);
}

int quickSort(Item * arr, int count) {
    return quickSortHelper(arr, 0, count);
}

int quickSortHelper(Item * arr, int start, int end) {
    int pivot, i, j;
    Item temp;

    if (start >= end)
        return 0;

    pivot = start;
    i = start;
    j = end;

    while (i < j) {
        while (arr[i].coords[arr[i].coord] <= arr[pivot].coords[arr[pivot].coord] && i <= end)
            i++;
        while (arr[j].coords[arr[j].coord] > arr[pivot].coords[arr[pivot].coord])
            j--;
        if (i < j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            }
    }

    temp = arr[j];
    arr[j] = arr[pivot];
    arr[pivot] = temp;

    quick_sort(arr, start, j - 1, coord);
    quick_sort(arr, j + 1, end, coord);

    return 0;
}