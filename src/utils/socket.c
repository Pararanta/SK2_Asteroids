#include <socket.h>

int socketInit(void)
{
  #ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1,1), &wsa_data);
  #else
    return 0;
  #endif
}

int socketQuit(void)
{
  #ifdef _WIN32
    return WSACleanup();
  #else
    return 0;
  #endif
}


int socketClose(SOCKET socket)
{

  int status = 0;

  #ifdef _WIN32
    status = shutdown(socket, SD_BOTH);
    if (status == 0) { status = closesocket(socket); }
  #else
    status = shutdown(socket, SHUT_RDWR);
    if (status == 0) { status = close(socket); }
  #endif

  return status;

}

int socketValid(SOCKET socket)
{
  #ifdef _WIN32
    return socket != INVALID_SOCKET;
  #else
    return socket > 0;
  #endif
}