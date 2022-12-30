#include <stdint.h>
#include <stdio.h>
#include <queue.h>

int exit_code = 0;

int assert(int assertion, const char * error)
{
    
    printf("Assertion %s: %s\n", assertion ? "Succeeded" : "Failed", error);

    if(assertion)
        return 0;

    exit_code = 1;
    return 1;
}

void testSetup()
{
    Queue test_object = {0};
    queueInit(&test_object, 5, ROTATE);
    printf("%d elements", test_object.element_size_bytes);
    assert(test_object.mode == ROTATE, "test 1 - wrong mode");
    assert(test_object.element_size_bytes == 5, "test 1 - wrong element size");
    assert(test_object.max_elements == 1, "test 1 - wrong max elements");
    assert(test_object.push_cursor == 0, "test 1 - wrong push cursor");
    assert(test_object.pop_cursor == 0, "test 1 - wrong pop cursor");
    assert(test_object.push_pop_balance == 0, "test 1 - wrong push pop balance");
}

void testPushPeekPop()
{
    Queue test_object = {0};

    int32_t a = 1;
    int32_t b = 2;

    queueInit(&test_object, sizeof(int32_t), ROTATE);

    assert(queuePeek(&test_object) == NULL, "test 2 - peek on empty not null");
    assert(queuePop(&test_object) == NULL, "test 2 - pop on empty not null");

    assert(*(int32_t *)queuePush(&test_object, &a) == a, "test 2 - first push return not equal to pushed value");
    assert(*(int32_t *)queuePeek(&test_object) == 1, "test 2 - peek after push not equal to pushed value");
    assert(*(int32_t *)queuePop(&test_object) == 1, "test 2 - pop after peek not equal to pushed value");
    assert(queuePeek(&test_object) == NULL, "test 2 - peek after pop not null");
    assert(queuePop(&test_object) == NULL, "test 2 - pop after pop not null");

    assert(*(int32_t *)queuePush(&test_object, &b) == b, "test 2 - second push return not equal to pushed value");
    assert(*(int32_t *)queuePush(&test_object, &a) == a, "test 2 - third push return not equal to pushed value");
    assert(*(int32_t *)queuePop(&test_object) == 2, "test 2 - pop not equal to first value pushed");
    assert(*(int32_t *)queuePush(&test_object, &b) == b, "test 2 - fourth push return not equal to pushed value");
    assert(*(int32_t *)queuePop(&test_object) == 1, "test 2 - pop not equal to second value pushed");
    assert(*(int32_t *)queuePop(&test_object) == 2, "test 2 - pop not equal to third value pushed");
    assert(queuePop(&test_object) == NULL, "test 2 - pop not null after popping three values");
}

void testMode()
{
    Queue test_object = {0};

    int32_t a = 1;
    int32_t b = 2;
    int32_t c = 3;

    queueInit(&test_object, sizeof(int32_t), ROTATE);
    assert(*(int32_t *)queuePush(&test_object, &a) == a, "test 3 - ROTATE - first push return not equal to pushed value");
    assert(*(int32_t *)queuePush(&test_object, &b) == b, "test 3 - ROTATE - second push return not equal to pushed value");
    assert(*(int32_t *)queuePush(&test_object, &c) == c, "test 3 - ROTATE - third push return not equal to pushed value");

    assert(*(int32_t *)queuePop(&test_object) == 2, "test 3 - ROTATE - pop not equal to second value pushed");
    assert(*(int32_t *)queuePop(&test_object) == 3, "test 3 - ROTATE - pop not equal to third value pushed");
    assert(queuePop(&test_object) == NULL, "test 3 - ROTATE - pop not null after popping two values");

    queueInit(&test_object, sizeof(int32_t), LIMIT);
    assert(*(int32_t *)queuePush(&test_object, &a) == a, "test 3 - LIMIT - first push return not equal to pushed value");
    assert(*(int32_t *)queuePush(&test_object, &b) == b, "test 3 - LIMIT - second push return not equal to pushed value");
    assert(queuePush(&test_object, &c) == NULL, "test 3 - LIMIT - push not null while pushing over capacity");

    assert(*(int32_t *)queuePop(&test_object) == 1, "test 3 - pop not equal to first value pushed");
    assert(*(int32_t *)queuePop(&test_object) == 2, "test 3 - pop not equal to second value pushed");
    assert(queuePop(&test_object) == NULL, "test 3 - pop not null after popping two values");
}

int main()
{
    testSetup();
    testPushPeekPop();
    testMode();
    return exit_code;
}