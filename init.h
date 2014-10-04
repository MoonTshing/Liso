#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "parser.h"
#include "fileIO.h"
#include "creatStruct.h"
#include "wrapSSL.h"

void Usage(int argc, char *argv[], Argu *argument);
void initClientSock(clientSock *client,int client_sock);
void cleanClientSock(clientSock *client,SetPool *pool);
void initArgu(requestLine *argu);
void freeArgu(requestLine *argu);
void initPool(SetPool *pool, int sock);
void clearClientBuf();


#endif