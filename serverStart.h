#ifndef SERVERSTART_H
#define SERVERSTART_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <openssl/ssl.h>

#include "fileIO.h"
#include "httpResponce.h"
#include "init.h"
#include "parser.h"
#include "creatStruct.h"
#include "daemonize.h"
#include "wrapSSL.h"
//#include "sslwrap.h"

int serverStart();
int initHttp();
int initHttps();
#endif