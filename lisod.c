/******************************************************************************
* lisod.c                                                               *
* Description: This file contains the C source code for an echo server.  The  *
*              server runs on a hard-coded port and simply write back anything*
*              sent to it by connected clients.  It does not support          *
*              concurrent clients.                                            *
*                                                                             *
* Authors: Chanjuan Zheng <chanjuaz@andrew.cmu.edu>                           *
*                                                                             *
*                                                                             *
*******************************************************************************/


#include "lisod.h"


/*----------- Global Value -----------*/
 Argu argument;
int already = 0;
int main(int argc, char* argv[])
{         
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    Usage(argc, argv, &argument);  
    logfd = fopen(argument.logfile,"a+");
    if(initSSL()==EXIT_FAILURE)
      {printf("ssl init failed\n");}
  /*if(daemonize(argument.lckfile) == EXIT_FAILURE)
    {
      printf("Failed to daemonize\n");
      return EXIT_FAILURE;
    }*/
   
 // do{
        if(initHttp() == EXIT_FAILURE )
         {
            logging(logfd,"Failed to initialize server.\n");
            return EXIT_FAILURE;
          }
         if(initHttps() == EXIT_FAILURE )
         {
            logging(logfd,"Failed to initialize server.\n");
            return EXIT_FAILURE;
          }

         initPool(&poolHttp,sockHttp);
         initPool(&poolHttps,sockHttps);
         if(FD_ISSET(sockHttp, &poolHttp.master)){
          printf("lalallalla\n");
         }
         printf("before server start\n");
        // printf("%d %d\n",poolHttp.fdmax,poolHttps.fdmax );
         /* finally, loop waiting for input and then write it back */
        serverStatus = 1;
        serverStatus = serverStart();
         clearClientBuf();
        close_socket(sockHttps);

//   }while(serverStatus == 2);

   
    fclose(logfd);
  //  close_socket(sockHttp);
    return EXIT_SUCCESS;
}




