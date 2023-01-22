#include "./include/collisions.h"

#define ELEMENT_COUNT 4096

typedef struct {
    uint32_t key;
    float value;
    int end;
} KeyValue;

typedef struct {
    union {
        struct Coord
        {
            float x_min;
            float x_max;
            float y_min;
            float y_max;
        } coord;
        float arr[4];
    } box;
    int calculated;
    int magic;
} BoundingBox;

typedef struct {
    uint32_t count;
    uint32_t keys[ELEMENT_COUNT];
} ActiveList;

typedef struct {
    uint32_t count;
    KeyValue boxes[ELEMENT_COUNT*2];
} BoundingList;

BoundingBox elements[ELEMENT_COUNT];

// Find an element in an ActiveList by its key
int ActiveFind(ActiveList *list, uint32_t key) {
  for (int i = 0; i < list->count; i++) {
    if (list->keys[i] == key) {
      return i;
    }
  }
  return -1;
}

// Insert an element into an ActiveList
int ActiveAdd(ActiveList *list, uint32_t key) {
  elements[key].magic++;
  if (list->count >= ELEMENT_COUNT) {
    // List is full
    return 1;
  }
  list->keys[list->count++] = key;
  return 0;
}

// Delete an element from an ActiveList
int ActiveDelete(ActiveList *list, uint32_t key) {
  elements[key].magic--;
  int index = ActiveHas(list, key);
  if (index == -1) {
    // Element not found
    return 1;
  }
  // Shift elements after the deleted element to the left
  for (int i = index; i < list->count - 1; i++) {
    list->keys[i] = list->keys[i + 1];
  }
  list->count--;
  return 0;
}

void BoundingSort(BoundingList *list) {
  int n = list->count;
  if (n > 1) {
    // Split the list into two halves
    int mid = n / 2;
    BoundingList left = { mid, list->boxes };
    BoundingList right = { n - mid, list->boxes + mid };
    // Recursively sort the two halves
    bounding_list_sort(&left);
    bounding_list_sort(&right);
    // Merge the sorted halves back together
    int i = 0, j = 0, k = 0;
    while (i < left.count && j < right.count) {
      if (left.boxes[i].value < right.boxes[j].value) {
        list->boxes[k++] = left.boxes[i++];
      } else {
        list->boxes[k++] = right.boxes[j++];
      }
    }
    // Copy the remaining elements, if any
    while (i < left.count) {
      list->boxes[k++] = left.boxes[i++];
    }
    while (j < right.count) {
      list->boxes[k++] = right.boxes[j++];
  
    }
  }
}

void activeToBounding(ActiveList * active, BoundingList * bounding, int start_coord)
{
    bounding->count = active->count*2;
    for(int i = 0; i < active->count; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            bounding->boxes[i*2 + j].key = active->keys[i];
            bounding->boxes[i*2 + j].end = j;
            bounding->boxes[i*2 + j].value = elements[active->keys[i]].box.arr[start_coord + j];
        }
    }

    BoundingSort(bounding);
}

void checkCollision(uint32_t a, uint32_t b)
{

}

ActiveList Active;
ActiveList ActiveX;
ActiveList ActiveY;
BoundingList BoundingX;
BoundingList BoundingY;

int detect()
{
    Active.count = 0;
    for(int i = 0; i < ELEMENT_COUNT; i++)
    {
        elements[i].calculated = 0;
        elements[i].magic = 0;
        ActiveAdd(&Active, i);
    }

    activeToBounding(&Active, &BoundingX, 0);
    ActiveX.count = 0;
    int next = 0;
    for(int i = 0; i < BoundingX.count; i++)
    {
        if(!BoundingX.boxes[i].end)
        {
            next++;
            ActiveAdd(&ActiveX, BoundingX.boxes[i].key);
            continue;
        }

        if(!next)
        {
            ActiveDelete(&ActiveX, BoundingX.boxes[i].key);
            continue;
        }

        activeToBounding(&ActiveX, &BoundingY, 0);
        ActiveY.count = 0;
        for(int j = 0; j < BoundingX.count; j++)
        {
            if(BoundingX.boxes[i].end)
            {
                ActiveDelete(&ActiveY, BoundingX.boxes[i].key);
                continue;
            }

            if(elements[BoundingX.boxes[i].key].calculated)
            {
                ActiveAdd(&ActiveY, BoundingX.boxes[i].key);
                continue;
            }

            for(int k = 0; k < ActiveY.count; k++)
                checkCollision(ActiveY.keys[k], BoundingX.boxes[i].key);

            elements[BoundingX.boxes[i].key].calculated = 1;
            ActiveAdd(&ActiveY, BoundingX.boxes[i].key);
        }

        ActiveDelete(&ActiveX, BoundingX.boxes[i].key);
    }

    return 0;
}

/*int detect()
{
    sort(&A);
    X.count = 0;
    Y.count = 0;

    uint32_t new_active = 0;
    for(int i = 0; i < A.count; i++)
    {
        Item y1 = X.array[i];
        y1.coord = 2;
        Item y2 = X.array[i];
        y2.coord = 3;

        if(has(&X, y1) + has(&X, y2) == 0)
        {
            add(&X, y1);
            add(&X, y2);
            new_active++;
            continue;
        }

        if(!new_active)
        {
            remove(&X, y1);
            remove(&X, y2);
            continue;
        }

        for(int j = 0; j < X.count; j++)
        {
            
            Item copy = X.array[j];
            X.array[j].checked = 1;
            copy.coord = 2;
            if(has(&Y, copy))
            {
                remove(&Y, copy);
                continue;
            }
            if(copy.checked)
                continue;
            
            for(int k = 0; k < Y.count; k++)
                checkCollision(copy, Y.array[k]);

            add(&Y, copy);
        }

        remove(&X, y1);
        remove(&X, y2);
        Y.count = 0;
        new_active = 0;
    }
}*/