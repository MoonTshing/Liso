#ifndef WRAPSSL_H
#define WRAPSSL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "creatStruct.h"
#include <sys/errno.h>

int initSSL();
int socketSSLWrap();

#endif