#include <poll.h>
#include <stdint.h>
#include <stdio.h>

#define EPSILON_SQUARED EPSILON*EPSILON

int exit_code = 0;

int assert(int assertion, const char * error)
{
    
    printf("Assertion %s: %s\n", assertion ? "Succeeded" : "Failed", error);

    if(assertion)
        return 0;

    exit_code = 1;
    return 1;
}

void testRequestOverCount()
{
    Poll poll = {0};
    uint32_t indexes[4];
    pollInit(&poll, indexes, 4);
    assert(!pollRequest(&poll, NULL), "test 1 - request 1");
    assert(!pollRequest(&poll, NULL), "test 1 - request 2");
    assert(!pollRequest(&poll, NULL), "test 1 - request 3");
    assert(!pollRequest(&poll, NULL), "test 1 - request 4");
    assert( pollRequest(&poll, NULL), "test 1 - request 5");
}

void testReturnUnused()
{
    Poll poll = {0};
    uint32_t indexes[4];
    pollInit(&poll, indexes, 4);
    assert(pollReturn(&poll, 0), "test 2 - return 0");
    assert(pollReturn(&poll, 3), "test 2 - return 3");
}

void testMaxIndexAndCount()
{
    Poll poll = {0};
    uint32_t indexes[4];
    pollInit(&poll, indexes, 4);

    assert(poll.used_indexes == 0, "test 3 - step 1");
    assert(poll.max_index == 0, "test 3 - step 2");

    pollRequest(&poll, NULL);
    assert(poll.used_indexes == 1, "test 3 - step 3");
    assert(poll.max_index == 0, "test 3 - step 4");

    pollRequest(&poll, NULL);
    assert(poll.used_indexes == 2, "test 3 - step 5");
    assert(poll.max_index == 1, "test 3 - step 6");

    pollRequest(&poll, NULL);
    assert(poll.used_indexes == 3, "test 3 - step 7");
    assert(poll.max_index == 2, "test 3 - step 8");

    pollReturn(&poll, 1);
    assert(poll.used_indexes == 2, "test 3 - step 9");
    assert(poll.max_index == 2, "test 3 - step 10");

    pollReturn(&poll, 2);
    assert(poll.used_indexes == 1, "test 3 - step 11");
    assert(poll.max_index == 0, "test 3 - step 12");

    pollRequest(&poll, NULL);
    assert(poll.used_indexes == 2, "test 3 - step 13");
    assert(poll.max_index == 1, "test 3 - step 14");

    pollReturn(&poll, 0);
    pollReturn(&poll, 1);
    assert(poll.used_indexes == 0, "test 3 - step 15");
    assert(poll.max_index == 0, "test 3 - step 16");

}

void testRequestReturned()
{
    Poll poll = {0};
    uint32_t indexes[4];
    pollInit(&poll, indexes, 4);

    uint32_t index;

    pollRequest(&poll, &index);
    assert(index == 0, "test 4 - step 1");
    pollRequest(&poll, &index);
    assert(index == 1, "test 4 - step 2");
    pollReturn(&poll, 0);
    pollRequest(&poll, &index);
    assert(index == 0, "test 4 - step 3");
}

int main()
{
    testRequestOverCount();
    testReturnUnused();
    testMaxIndexAndCount();
    testRequestReturned();
    return exit_code;
}