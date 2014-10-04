#include "fileIO.h"

void readMesg(clientSock *client, int i){
     //*readret = 0;
    if((client->readret = recv(i, client->buf+client->alreadyRead,
           BUF_SIZE- client->alreadyRead, 0)) > 0)
     {
       client->alreadyRead += client->readret;   
      }    
}

void readSSL(clientSock *client)
{   
  printf("wrap %p\n",client->client_context );
   if((client->readret = SSL_read(client->client_context, client->buf+client->alreadyRead,
           BUF_SIZE- client->alreadyRead)) > 0)
     {
        client->alreadyRead += client->readret;
     }
       
}


void Select(SetPool *pool, int sock){
    if(select(pool->fdmax + 1, &pool->temp, NULL, NULL, &tv) == -1)
       {
            fprintf(stderr, "Select error.\n");
            close_socket(sock);
           // exit(0);
            return;
       }
                
}

int close_socket(int sock)
{
    if (close(sock))
    {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}
int connectHttp(struct sockaddr_in cli_addr, SetPool *pool){
    /* Accept the connection and add the FD to the set pool*/
       socklen_t cli_size;
       cli_size = sizeof(cli_addr);
       int client_sock;
       if ((client_sock = accept(sockHttp, (struct sockaddr *) &cli_addr,
                                                    &cli_size)) == -1)
        {             
             close_socket(sockHttp);
             fprintf(stderr, "Error accepting connection.\n");
             //exit(-1);
             return EXIT_FAILURE;
         }
      int flags = fcntl(client_sock, F_GETFL, 0);
      fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);
       if(client_sock == -1)
       {

               fprintf(stderr, "Error opening client socket.\n");

       } 
      else{

           FD_SET(client_sock, &pool->master);
          if(client_sock > pool->fdmax)
            pool->fdmax = client_sock;

          if(pool->fdmax == FD_SETSIZE){
            fprintf(stderr, "Number of connections reaches the maximum.\n");
            close_socket(sockHttp);
            
          }

      } 
      return client_sock;
}

int connectHttps(struct sockaddr_in cli_addr, 
                  SetPool *pool)
{
    /* Accept the connection and add the FD to the set pool*/
       socklen_t cli_size;
       cli_size = sizeof(cli_addr);
       int client_sock;
       if ((client_sock = accept(sockHttps, (struct sockaddr *) &cli_addr,
                                                    &cli_size)) == -1)
        {             
             close_socket(sockHttps);
             fprintf(stderr, "Error accepting connection.\n");
             //exit(-1);
             return EXIT_FAILURE;
         }
        //  printf("sock %d\n", client->sock);
      client[client_sock].sock = client_sock;
      if(socketSSLWrap(&client[client_sock])== EXIT_SUCCESS)
      {
        printf("socket wrap EXIT_SUCCESS\n");
      }
      int flags = fcntl(client_sock, F_GETFL, 0);
      fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);
       if(client_sock == -1)
       {

               fprintf(stderr, "Error opening client socket.\n");

       } 
      else{

           FD_SET(client_sock, &pool->master);
          if(client_sock > pool->fdmax)
            pool->fdmax = client_sock;

          if(pool->fdmax == FD_SETSIZE){
            fprintf(stderr, "Number of connections reaches the maximum.\n");
            close_socket(sockHttps);
            
          }

      } 
      return client_sock;
}
void logging(FILE *fd, char *logInfo){
    fprintf(fd,"%s",logInfo);
}
