#include <extensions.h>
#include <math.h>
#ifdef _WIN32
    #include <windows.h>
    #include <inttypes.h>
#else
    #include <sys/time.h>
#endif

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

#ifdef _WIN32
    double getTime()
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        uint64_t tt = (uint64_t)ft.dwHighDateTime << 32 | ft.dwLowDateTime; // Windows time in 100 nanoseconds
        tt /= 10; // Convert to microseconds
        tt -= 11644473600000000ULL; // Convert to unix epoch

        return tt * 1e-6;
    }
#else
    double getTime()
    {
        struct timeval measured;
        gettimeofday(&measured, 0);
        double elapsed = measured.tv_sec + measured.tv_usec * 1e-6;

        return elapsed;
    }
#endif


