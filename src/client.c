#include <stdio.h>
#include <gui.h>
#include <client.h>
#include <socket.h>
#include <sleep.h>
#include <player.h>
#include <loop.h>
#include <string.h>

char * host = DEFAULT_HOST;
char * port = DEFAULT_PORT;
char * room_name = DEFAULT_ROOM;

struct addrinfo * resolved;
SOCKET fd;
Player me;
Room room;
int initAddr()
{
   struct addrinfo hints    = {0};
   hints.ai_family          = AF_INET;
   hints.ai_protocol        = IPPROTO_UDP;

   int result = getaddrinfo(host, port, &hints, &resolved);

   if(result && resolved)
      freeaddrinfo(resolved);

   return result;
}

int tryResolve()
{
   int success = 0;
   for(int i = 0; i < RETRY_ATTEMPTS; i++)
      if((success = (!initAddr() && resolved)))
         break;
      else
      {
         printf("Attempt to resolve %s:%s %d failed, waiting %d ms\n", host, port, i+1, RETRY_WAITTIME);
         sleep_ms(RETRY_WAITTIME);
      }

   if(!success)
   {
      printf("Could not resolve host %s:%s failed %d times\n", host, port, RETRY_ATTEMPTS);
   }

   return !success;
}

int messageListen(void * args)
{
   return 0;
}

int main(int argc, char ** argv)
{
   if(argc > 1)
      host = argv[1];

   if(argc > 2)
      host = argv[2];

   if(argc > 3)
      room_name = argv[3];

   if(strlen(room_name) > MAX_ROOM_NAME)
      room_name[MAX_ROOM_NAME] = '\0';

   printf("Attempting to connect to %s:%s\n", host, port);
   socketInit();

   if(tryResolve())
   {
      socketQuit();
      return 1;
   }

   for(struct addrinfo * it = resolved; it; it=it->ai_next){
        struct sockaddr_in* addr = (struct sockaddr_in*) it->ai_addr;
        printf("Resolved: %s\n", inet_ntoa(addr->sin_addr));
   }
   fd = socket(AF_INET, SOCK_STREAM, 0);
   int result = connect(fd, resolved->ai_addr, resolved->ai_addrlen);

   uint16_t room_name_size = strlen(room_name);
   printf("%d %s\n", room_name_size, room_name);
   room_name_size = htons(room_name_size);
   send(fd, &room_name_size, sizeof(uint16_t), 0);
   send(fd, room_name, strlen(room_name), 0);
   initRooms();
   initRoomThread(&room);
   initPlayer(&room.player, fd, RESPONSE, REQUEST);
   guiRun(1);
   socketClose(fd);
   socketQuit();
   return 0;
}