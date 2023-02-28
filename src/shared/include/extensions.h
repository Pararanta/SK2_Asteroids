#ifndef H_ASTEROIDS_EXTENSIONS
#define H_ASTEROIDS_EXTENSIONS

    #include <tinycthread.h>

    typedef struct {
        float x;
        float y;
    } Point;

    int cnd_waitonce(cnd_t * cond, mtx_t * mtx);
    Point normalize(float x, float y);
    double getTime();
#endif