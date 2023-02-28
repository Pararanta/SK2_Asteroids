#include <stdio.h>
#include <gui.h>
#include <server.h>
#include <socket.h>
#include <errno.h>
#include <tinycthread.h>
#include <player.h>
#include <sleep.h>
#include <stdlib.h>
#include <loop.h>
#include <string.h>

#include <stdio.h>
//#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
//#include <unistd.h>

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  //size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  //backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

thrd_t accept_thrd;
char * host = DEFAULT_HOST;
char * port = DEFAULT_PORT;
int socket_server;
struct sockaddr_in address_server;
const int one = 1;

Room rooms[MAX_ROOMS];

int run_room(void * room_v)
{
   Room * room = room_v;
   initRoomThread(room);
   guiRun(0);
}

int setupAddress()
{
   struct in_addr address;
   if (inet_pton(AF_INET, host, &address) <= 0)
   {
       printf ("Invalid address\n");
       return 1;
   }

   address_server.sin_family = AF_INET;
   address_server.sin_addr = address;
   address_server.sin_port = htons( atoi(port) );

   return 0;
}

int listenForConnections(void * param)
{
   Entity new_player = { 1, 0.5f, 0.5f };
   while (1) {
      printf("Waiting for new connection...\n");

      struct sockaddr_in client_address;
      size_t size = sizeof(client_address);

      int connection = accept(socket_server, (struct sockaddr *)&client_address, (socklen_t*)&size);

      uint16_t name_len;
      recv(connection, &name_len, sizeof(uint16_t), MSG_WAITALL);

      name_len = ntohs(name_len);
      if(name_len > MAX_ROOM_NAME)
         name_len = MAX_ROOM_NAME;

      char name_buffer[MAX_ROOM_NAME+1] = {0};
      recv(connection, name_buffer, name_len, MSG_WAITALL);

      printf("New Player: %s %s %d\n", inet_ntoa(client_address.sin_addr), name_buffer, name_len);

      uint16_t found_room_index = MAX_ROOMS;
      uint16_t empty_room_index = MAX_ROOMS;

      for(int i = 0; i < MAX_ROOMS; i++)
      {
         if(!rooms[i].status && empty_room_index == MAX_ROOMS)
            empty_room_index = i;

         if(!strcmp(name_buffer, rooms[i].name))
            found_room_index = i;
      }
      
      if(empty_room_index == MAX_ROOMS && found_room_index == MAX_ROOMS)
      {
         socketClose(connection);
         continue;
      }

      if(found_room_index != MAX_ROOMS)
      {
         uint16_t player_index = MAX_PLAYER_COUNT;
         for(int i = 0; i < MAX_PLAYER_COUNT; i++)
            if(!rooms[found_room_index].players[i].status)
            {
               player_index = i;
               break;
            }

         if(player_index == MAX_PLAYER_COUNT)
         {
            socketClose(connection);
            continue;

         }
         printf("New Player Index: %d\n", player_index);
         initPlayer(&rooms[found_room_index].players[player_index], connection, REQUEST, RESPONSE);
         uint16_t instance_index = instantiate(&rooms[found_room_index], new_player);
         rooms[found_room_index].players[player_index].entity = instance_index;
         instance_index = htons(instance_index);
         send(connection, (char*)&instance_index, sizeof(uint16_t), 0);
         
         continue;

      }
      
      if(empty_room_index != MAX_ROOMS)
      {
         printf("New Room Index: %d\n", empty_room_index);
         initRoom(&rooms[empty_room_index], name_buffer, name_len);
         initPlayer(&rooms[empty_room_index].players[0], connection, REQUEST, RESPONSE);
         uint16_t instance_index = instantiate(&rooms[empty_room_index], new_player);
         rooms[empty_room_index].players[0].entity = instance_index;
         instance_index = htons(instance_index);
         send(connection, (char*)&instance_index, sizeof(uint16_t), 0);
         thrd_create(&rooms[empty_room_index].room_thrd, run_room, (void *) &rooms[empty_room_index]);
         continue;
      }

   }

   return 0;
}

int main(int argc, char ** argv)
{
   signal(SIGSEGV, handler);
   socketInit();

   if(argc > 1)
      host = argv[1];

   if(argc > 2)
      host = argv[2];
   
   printf("Attempting to host on %s:%s", host, port);
   socket_server = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_server < 0)
   {
      perror("Could not create file descriptor\n");
   }
   setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
   setupAddress();
   bind(socket_server, (struct sockaddr *) &address_server, sizeof(address_server));
   listen(socket_server, 4);
   
   initRooms();

   thrd_create(&accept_thrd, listenForConnections, NULL);
   int res;
   thrd_join(accept_thrd, &res);
   socketQuit();

   return 0;
}