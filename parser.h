#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>
#include "creatStruct.h"
#include "fileIO.h"

int httpParser(char *buf, requestLine *argu);
void httpLineParser(char *temp, int lens, requestLine *argu);
void parseUri(requestLine *argu);
void setReqStatus(requestLine *argu, int statusCode);
int bufParse(clientSock *client, requestLine *req,char *cmpLine);
int Parser(clientSock *client,requestLine *argu);
#endif