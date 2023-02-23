#include <stdio.h>
#include <gui.h>
#include <server.h>
#include <socket.h>
#include <errno.h>
#include <tinycthread.h>
#include <player.h>
#include <sleep.h>

thrd_t accept_thrd;
char * host = DEFAULT_HOST;
char * port = DEFAULT_PORT;
int socket_server;
struct sockaddr_in address_server;
const int one = 1;

Player player;

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
   while (1) {
      printf("Waiting for new connection...\n");
      struct sockaddr_in client_address;
      size_t size = sizeof(client_address);
      int connection = accept(socket_server, (struct sockaddr *)&client_address, (socklen_t*)&size);
      printf("New Player: %s\n", inet_ntoa(client_address.sin_addr));
      initPlayer(&player, connection, REQUEST, RESPONSE);
   }

   return 0;
}

int main(int argc, char ** argv)
{
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

   thrd_create(&accept_thrd, listenForConnections, NULL);
   
   Request requests[255];
   uint16_t count;
   while(1)
   {
       sleep(500);
       if (!player.status)
       {
           continue;
       }
      consumeRequests(&player, requests, &count);
      for(int i = 0; i < count; i++)
         printf("%d\n", requests[i].entity);
   }

   guiRun(0);
   socketQuit();

   return 0;
}