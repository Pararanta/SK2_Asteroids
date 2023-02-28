#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void sleep_ms(int ms)
{
    if(ms <= 0)
        return;
        
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}