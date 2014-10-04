#ifndef LISOD_H
#define LISOD_H

//#define BUF_SIZE 4096

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "fileIO.h"
#include "httpResponce.h"
#include "init.h"
#include "parser.h"
#include "creatStruct.h"
#include "daemonize.h"
#include "serverStart.h"
#include "wrapSSL.h"
//int initServer();
//int serverStart();


#endif