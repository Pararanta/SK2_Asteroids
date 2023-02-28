/* https://stackoverflow.com/questions/28027937/cross-platform-sockets */
#ifndef H_ASTEROIDS_SOCKET
#define H_ASTEROIDS_SOCKET
    #ifdef _WIN32
    /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0601  /* Windows 7. */
    #endif
        #include <winsock2.h>
        #include <Ws2tcpip.h>

        typedef SOCKET sock_t;
    #else
        /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
        #include <unistd.h> /* Needed for close() */

        typedef int sock_t;
    #endif

    int socketInit(void);
    int socketQuit(void);
    int socketClose(sock_t socket);
    int socketValid(sock_t socket);
 #endif
