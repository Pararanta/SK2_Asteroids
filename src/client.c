#include <stdio.h>
#include <gui.h>
#include <client.h>
#include <socket.h>
#include <sleep.h>

char * host = DEFAULT_HOST;
char * port = DEFAULT_PORT;
struct addrinfo * resolved;

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
         sleep(RETRY_WAITTIME);
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
        printf(" %s\n", inet_ntoa(addr->sin_addr));      //    bo w podpowiedziach 
   } 
    
   guiRun(0);
   socketQuit();
   return 0;
}