#ifndef H_ASTEROIDS_LOOP
#define H_ASTEROIDS_LOOP

    #include <room.h>
    #include <entity.h>
    #include <tinycthread.h>

    extern tss_t room_key;
    void initRooms();
    int initRoomThread(void * room_v);
    Room * getThreadRoom();

#endif