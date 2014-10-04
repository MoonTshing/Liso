#ifndef DAEMONZIE_H
#define DAEMONZIE_H

/* daemonize includes */
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "creatStruct.h"
int daemonize(char* lock_file);
void signal_handler(int sig);

#endif