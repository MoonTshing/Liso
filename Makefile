################################################################################
# Makefile                                                                     #
#                                                                              #
# Description: This file contains the make rules for Recitation 1.             #
#                                                                              #
# Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                          #
#          Wolf Richter <wolf@cs.cmu.edu>                                      #
#                                                                              #
################################################################################

default: lisod

lisod: init.c parser.c fileIO.c httpResponce.c lisod.c daemonize.c wrapSSL.c serverStart.c
	gcc $^ -lssl -lcrypto -o lisod -Wall -Werror

clean:
	@rm lisod
