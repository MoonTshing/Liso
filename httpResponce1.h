#ifndef RESPONCE_H
#define RESPONCE_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "creatStruct.h"
#include "fileIO.h"

char *filePath(char *relaPath);
void responseLine(requestLine *req,clientSock *client, SetPool *pool);
int statusAdd(requestLine *req);
int contentType(requestLine *req);
void headerAdd(requestLine *req);
int checkReqst(requestLine *req);

#endif
