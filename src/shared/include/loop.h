#ifndef H_ASTEROIDS_LOOP
#define H_ASTEROIDS_LOOP

    #include <room.h>
    #include <entity.h>
    #include <tinycthread.h>

    extern tss_t room_key;
    void initRooms();
    int initRoomThread(void * room_v);
    Room * getThreadRoom();

    int gameStep(float delta);

    int clientBeforeGameStep();
    int clientAfterGameStep(uint8_t requestAll, float dx, float dy, uint8_t shootState, float tx, float ty);

    int serverBeforeGameStep(double time, double* lastAsteroid);
    int serverAfterGameStep();
#endif