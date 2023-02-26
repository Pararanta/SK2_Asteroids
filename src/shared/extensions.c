#include <extensions.h>
#include <math.h>

int cnd_waitonce(cnd_t * cond, mtx_t * mtx)
{
    mtx_lock(mtx);
    cnd_wait(cond, mtx);
    mtx_unlock(mtx);

    return 0;
}

Point normalize(float x, float y)
{
    Point result = {0};
    float len = sqrtf(x*x + y*y);
    if(len == 0.0f)
        return result;
    
    result.x = x/len;
    result.y = y/len;

    return result;
}
