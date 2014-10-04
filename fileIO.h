#ifndef FILEIO_H
#define FILEIO_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>
#include <openssl/ssl.h>
#include "creatStruct.h"
#include "wrapSSL.h"

FILE *logfd;

//ssize_t readMesg(char **buf, int i, ssize_t *readret);
void readMesg(clientSock *client, int i);
//void sendMesg(char *buf, clientSock *client, SetPool *pool);
void Select(SetPool *pool, int sock);
int connectHttp(struct sockaddr_in cli_addr, SetPool *pool);
int connectHttps(struct sockaddr_in cli_addr, SetPool *pool);
int close_socket(int sock);
void logging(FILE *fd, char *logInfo);
void readSSL(clientSock *client);
#endif