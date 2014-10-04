#include "init.h"
/*---------------Usage Infomation----------------*/
/*  Print usage information about the server when 
 *  user input wrong arguments
 */

void Usage(int argc, char *argv[], Argu *argument){
  if(argc != 9)
  {
    fprintf(stdout, "Usage: ./lisod <HTTP port> <HTTPS port> <log file> <lock file> <www folder> <CGI script path> <private key file> <certificate file>\n");
    fprintf(stdout, "HTTP port – the port for the HTTP (or echo) server to listen on\n");
    fprintf(stdout, "HTTPS port – the port for the HTTPS server to listen on\n");
    fprintf(stdout, "log file – file to send log messages to (debug, info, error)\n");
    fprintf(stdout, "lock file – file to lock on when becoming a daemon process\n");
    fprintf(stdout, "www folder – folder containing a tree to serve as the root of a website\n");
    fprintf(stdout, "CGI script name (or folder) – for this project, this is a file that should be a script where you redirect all /cgi/* URIs. In the real world, this would likely be a directory of executable programs.\n");
    fprintf(stdout, "private key file – private key file path\n");
    fprintf(stdout, "certificate file – certificate file path\n");
    
    exit(0);
  }
  argument->httpPort = atoi(argv[1]);
  argument->httpsPort = atoi(argv[2]);
  argument->logfile = argv[3];
  argument->lckfile = argv[4];
  argument->wwwPath = argv[5];
  argument->cgiPath = argv[6];
  argument->priKyFile = argv[7];
  argument->certKyFile = argv[8];

}


/**********Initialize the clientsock**********/
void initClientSock(clientSock *client,int client_sock)
{
  client->sock = client_sock;
  // fprintf(stdout, "SOCKET :%d\n",client->sock);
  memset(client->buf,0,BUF_SIZE);
  client->alreadyRead = 0;
  client->status = 1;
  client->readret = 0;
}

void cleanClientSock(clientSock *client,SetPool *pool){
  close_socket(client->sock);
  FD_CLR(client->sock,&pool->master);
  memset(client->buf,0,BUF_SIZE);
  client->alreadyRead = 0;
  client->status = 0;

}


void initArgu(requestLine *argu){
  strcpy(argu->method," ");
  strcpy(argu->uri, "\0");
  strcpy(argu->relativePath,"");
  strcpy(argu->version,"HTTP/1.1");
  argu->contentLength = 0;
  argu->status = 200;
  strcpy(argu->host," ");
  strcpy(argu->header,"");
  strcpy(argu->contentType,"");
  strcpy(argu->lastModified,"");
  strcpy(argu->server,"Liso/1.0");
  strcpy(argu->statusLine,"");
}

void initPool(SetPool *pool, int sock)
{
    FD_ZERO(&pool->master);
    FD_ZERO(&pool->temp);
    FD_SET(sock, &pool->master);
    pool->fdmax = sock;
}

void clearClientBuf()
{
  int j;
  for(j = 0; j < 1024; j++)
  {
    if(client[j].buf != NULL)
    {
      free(client[j].buf);
      client[j].alreadyRead = 0;
      client[j].status = 1;
      client[j].readret = 0;
     close_socket(client[j].sock);
   }
  }
}
