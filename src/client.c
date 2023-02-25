#include <stdio.h>
#include <gui.h>
#include <client.h>
#include <socket.h>
#include <sleep.h>
#include <player.h>

char * host = DEFAULT_HOST;
char * port = DEFAULT_PORT;
struct addrinfo * resolved;
SOCKET fd;
Player me;

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
      if(success = !initAddr() && resolved)
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

}

int main(int argc, char ** argv)
{
   if(argc > 1)
      host = argv[1];

   if(argc > 2)
      host = argv[2];

   printf("Attempting to connect to %s:%s\n", host, port);
   socketInit();

   if(tryResolve())
   {
      socketQuit();
      return 1;
   }

   for(struct addrinfo * it = resolved; it; it=it->ai_next){
        struct sockaddr_in* addr = (struct sockaddr_in*) it->ai_addr; // <- rzutowanie bezpieczne,
        printf("Resolved: %s\n", inet_ntoa(addr->sin_addr));      //    bo w podpowiedziach 
   }
   fd = socket(AF_INET, SOCK_STREAM, 0);
   int result = connect(fd, resolved->ai_addr, resolved->ai_addrlen);
   initPlayer(&me, fd, RESPONSE, REQUEST);
   uint16_t one = 1;
   struct timespec interval;
   interval.tv_sec = 0;
   interval.tv_nsec = 100000;
   for(int i = 0; i <= 5; i++)
   {
       for (int j = 0; j <= 10; j++)
       {
           Request req;
           req.type = REQUEST_ENTITY;
           req.entity = j;
           appendRequests(&me, &req, &one);
           startSending(&me);
           thrd_sleep(&interval, NULL);
       }
      thrd_sleep(&interval, NULL);
      thrd_sleep(&interval, NULL);
      thrd_sleep(&interval, NULL);
   }
   guiRun(0);
   //sleep(20000);
   socketClose(fd);
   socketQuit();
   return 0;
}