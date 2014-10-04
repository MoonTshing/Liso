#ifndef CREATSTRUCT_H
#define CREATSTRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/mman.h>
#include <openssl/ssl.h>
int sockHttp;
int sockHttps;
struct timeval tv;
SSL_CTX *ssl_context;
int serverStatus;
typedef enum{HTTP,HTTPS}sockType;
#define BUF_SIZE 8192

typedef struct{                   /*  Requestion line of client */
  char method[20];
  char uri[BUF_SIZE];
  char relativePath[BUF_SIZE];
  char version[20];
  char host[100];
  char header[BUF_SIZE];
  char contentType[100];
  char connect[100];
  char date[100];
  int contentLength;
  char lastModified[100];
  char server[100];
  char statusLine[BUF_SIZE];
  int status;    // default to be "HTTP/1.1 200 OK\r\n"
}requestLine;


typedef struct{                   /* Pool of descriptor set */
  fd_set master;
  fd_set temp;
  int fdmax;
}SetPool;

typedef struct{                   /* Argument of liso server */
  int httpPort;
  int httpsPort;
  char *logfile;
  char *lckfile;
  char *wwwPath;
  char *cgiPath;
  char *priKyFile;
  char *certKyFile;
}Argu;


typedef struct 
{
  int sock;
  char buf[BUF_SIZE];
  ssize_t alreadyRead;            // Bytes already read to the buffer 
  int status;
  ssize_t readret;
  sockType type;
  SSL *client_context;
}clientSock;
SetPool poolHttp;
SetPool poolHttps;
clientSock client[1024];

extern Argu argument;
#endif