#include <extensions.h>

int cnd_waitonce(cnd_t * cond, mtx_t * mtx)
{
    mtx_lock(mtx);
    cnd_wait(cond, mtx);
    mtx_unlock(mtx);

    return 0;
}