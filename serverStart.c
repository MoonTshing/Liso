#include "serverStart.h"

int initHttp()
{    
     struct sockaddr_in addr;

    /* all networked programs must create a socket */
    if ((sockHttp = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }
    // set SO_REUSEADDR on a socket to true (1):
    int optval = 1;
    setsockopt(sockHttp, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(argument.httpPort);
    //addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    fprintf(stdout, "----- Echo Server3 -----\n");
    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sockHttp, (struct sockaddr *) &addr, sizeof(addr)))
    {
        close_socket(sockHttp);
        fprintf(logfd, "%d\n", sockHttp);
        logging(logfd, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }
  
    if (listen(sockHttp, 100))
    {
        close_socket(sockHttp);
        logging(logfd, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    /* Initializing the file descriptor set pool */  
}


int initHttps()
{    
     struct sockaddr_in addr;

    /* all networked programs must create a socket */
    if ((sockHttps = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }
    // set SO_REUSEADDR on a socket to true (1):
    int optval = 1;
    setsockopt(sockHttps, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(argument.httpsPort);
    //addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sockHttps, (struct sockaddr *) &addr, sizeof(addr)))
    {
        close_socket(sockHttps);
        fprintf(logfd, "%d\n", sockHttps);
        logging(logfd, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }
  
    if (listen(sockHttps, 100))
    {
        close_socket(sockHttps);
        logging(logfd, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    /* Initializing the file descriptor set pool */  
}

int serverStart()
{
  
  int i;
  requestLine reqLine;
  struct sockaddr_in cli_addr;
  //int p = 0;
  for(i = 0;i<1024;i++)
    {
      client[i].status = 0;
    }

   while (1)
    {      

            /*if(serverStatus ==2)
        {
          return 2;
        }*/
        poolHttp.temp = poolHttp.master;
        Select(&poolHttp, sockHttp); 
        for(i = 4; i <= poolHttp.fdmax; i++)
       {  
           /* if(serverStatus == 2)
            {
              return 2;
            }
  */        
            if(FD_ISSET(i, &poolHttp.temp))
            {    
                if(client[i].status == 0) 
                    {
                      initClientSock(&client[i],i);
                    }
                /* Find a descriptor needed to be processed */
                 if(client[i].sock == sockHttp)
                 {    int j;
                    if(( j= connectHttp(cli_addr, &poolHttp))!=EXIT_FAILURE)
                    {
                      client[j].sock = j;
                      client[j].type = HTTP; 
                    }

                 } else
                 {
                      
                      readMesg(&client[i], client[i].sock);

                      if(client[i].readret == 0)
                      {   
                          cleanClientSock(&client[i], &poolHttp);
                          printf("client [%d]  http sock:%d\n", i,client[i].sock);
                      } 
                      else if( (client[i].readret < 0 && errno != EWOULDBLOCK)||
                                         (client[i].readret < 0 && errno != EAGAIN))
                      {
                          /*  Other error occurs */
                           logging(logfd, "Error reading from socket.\n");
                           close_socket(client[i].sock);
                           FD_CLR(client[i].sock, &poolHttp.master);

                      }
                      else if( (client[i].readret < 0 && errno == EWOULDBLOCK)||
                                 (client[i].readret < 0 && errno == EAGAIN))
                      {

                      }
                      else {  /* Read success and ready to send */
                          printf("client [%d]  http sock:%d\n", i,client[i].sock);
                          printf("%s\n", client[i].buf);
                          initArgu(&reqLine);
                          if(Parser(&client[i],&reqLine))
                          {
                          //  printf("counter:  %d\n", ++p);
                            responseLine(&reqLine,&client[i], &poolHttp);
                          }
                          if(strcmp(reqLine.connect,"Close") == 0)
                          {
                            cleanClientSock(&client[i],&poolHttp);
                          }
                         
                       
                      }     
               }

            }
      }

           /*  *******ssl part ********/

        poolHttps.temp = poolHttps.master;
        Select(&poolHttps,sockHttps); 
        for(i = 4; i <= poolHttps.fdmax; i++)
       {  

           /* if(serverStatus == 2)
            {
              return 2;
            }
  */      if(client[i].status == 0) 
                    {
                      initClientSock(&client[i],i);
                    }
            if(FD_ISSET(client[i].sock, &poolHttps.temp))
            {    
                
                  printf("sock %d\n", client[i].sock);
                /* Find a descriptor needed to be processed */
                 if(i == sockHttps)
                 {    
                    int j;
                    if((j = connectHttps(cli_addr, &poolHttps))!= EXIT_FAILURE)
                    {
                      client[j].type = HTTPS;
                    }
                   
                 } else{
                    printf("i ::: %d\n", i );
                       readSSL(&client[i]);
                      if(client[i].readret == 0)
                      {   
                          cleanClientSock(&client[i], &poolHttps);

                      } 
                      else if( (client[i].readret < 0 && errno != EWOULDBLOCK)||
                                (client[i].readret < 0 && errno != EAGAIN))
                      {
                          /*  Other error occurs */
                           logging(logfd, "Error reading from socket.\n");
                           close_socket(client[i].sock);
                           FD_CLR(client[i].sock, &poolHttps.master);

                      }
                      else if( (client[i].readret < 0 && errno == EWOULDBLOCK)||
                               (client[i].readret < 0 && errno == EAGAIN))
                      {

                      }
                      else {  /* Read success and ready to send */
                          printf("buffer : %s\n", client[i].buf);
                          initArgu(&reqLine);
                          if(Parser(&client[i],&reqLine))
                          {
                             responseLine(&reqLine,&client[i], &poolHttps); 
                          }/*else{
                            cleanClientSock(&client[i], &poolHttps);
                          }*/
                         
                          if(strcmp(reqLine.connect,"Close") == 0)
                          {
                            cleanClientSock(&client[i],&poolHttps);
                          }
                       
                      }     
               }

            }
        }
    
}
    return 1;
   /* initServer();
    initEvery();*/
}
