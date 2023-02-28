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
sock_t fd;
Player me;
Room room = { 0 };
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
      port = argv[2];

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
   if(!socketValid(fd))
   {
      printf("Could not open socket, exiting...\n");
      return 1;
   }
   int result = -1;
   for(int i = 0; i < RETRY_ATTEMPTS; i++)
   {
      result = connect(fd, resolved->ai_addr, resolved->ai_addrlen);
      printf("Connection attempt %d...\n", i);
      if(result >= 0)
         break;
      sleep_ms(RETRY_WAITTIME);
   }

   if(result < 0)
   {
      printf("Issue while connecting, %d tries failed, exiting...\n", RETRY_ATTEMPTS);
      return 1;
   }
   else
   {
      printf("Connected successfully\n");
   }
   printf("Joining room %s\n", room_name);
   uint16_t room_name_size = strlen(room_name);
   room_name_size = htons(room_name_size);
   send(fd, (char *) & room_name_size, sizeof(uint16_t), 0);
   send(fd, room_name, strlen(room_name), 0);
   initRooms();
   initRoom(&room, room_name, strlen(room_name));
   initRoomThread(&room);
   initPlayer(&room.player, fd, RESPONSE, REQUEST);
   recv(fd, &room.player.entity, sizeof(uint16_t), 0);
   room.player.entity = ntohs(room.player.entity);
   printf("Joined room, ours entity index is %d\n", room.player.entity);
   guiRun(1);
   socketClose(fd);
   socketQuit();
   return 0;
}