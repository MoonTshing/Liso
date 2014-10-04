#include "parser.h"

int Parser(clientSock *client,requestLine *argu)
{

	char cmpletReq[BUF_SIZE];
	if(bufParse(client,argu,cmpletReq)==1)
	{
		if( (cmpletReq != NULL) && argu->status != 411) {
			httpParser(cmpletReq,argu);
		} 
		return 1;
	}
	else{
		return 0;
	}
	

}
static int BadRq(requestLine *argu, char *key)
{
    char *headerSet[] = {
                         "Content-Length:",
                         "Content-Type:",
                         "Last-Modifed:",
                         "Connection:",
                         "Date:",
                         "Host:",
                         "User-Agent:",
                         "Accept:",
                         "Accept-Language:",
                         "Accept-Encoding:",
                         "Cache-Control:",
                         "Accept-Encoding:"
                        };
    int n = 12;
    int i;
   // printf("key: %s\n", key);
    for(i = 0; i < n; i ++)
    {   
       // printf("headerset :: %s\n", headerSet[i]);
        if(!strcmp(headerSet[i],key))
        {
            return 1;
        }
    }
    return 0;
}
int httpParser(char *buf, requestLine *argu)
{
    int lineSize;
    char *temp;
    char *currPtr;
    if(!(currPtr = strstr(buf,"\r\n")))
    {
    	argu->status = 500;
    	return 0;
    }
    if((lineSize = strlen(buf)-strlen(currPtr))<=0)
    {	
    	argu->status = 500;
    	return 0;
    }
    if(!(temp = malloc(lineSize+1)))
    {
    	argu->status = 500;
    	return 0;
    }

    char *t = strncpy(temp,buf,lineSize);
    t[lineSize] = 0;
    httpLineParser(temp,lineSize, argu);
   	if(!temp)
   	{
   		free(temp); 
   	}
    char *leftLine;
    int length = strlen(currPtr)-2;
    leftLine = (char *)malloc(length+1);
    strncpy(leftLine,currPtr+2,length);
    leftLine[length]='\0';

    char *pch;
    pch = strtok(leftLine,"\n");
    printf("req->status: %d\n",argu->status);
    printf("leftLine::  %s\n",leftLine );
    while(pch != NULL)
    {   
        char key[20];
        char value[40];
        sscanf(pch,"%s %s",key,value);
        if(0 == BadRq(argu,key))
        {
            argu->status = 400;
        }else
        {
            if(strcmp(key,"Content-Type:")==0)
             {   
             strcpy(argu->contentType,value);
             }else if(strcmp(key,"Content-Length:")==0)
             {
                    argu->contentLength = atoi(value);
             }
             else if(strcmp(key,"Connection:")==0)
             {
                     strcpy(argu->connect,value);
              }
        }
        
        
        pch = strtok(NULL,"\n");

    }
    free(leftLine);
    printf("req->status: %d\n",argu->status);
    return 1;
}

int bufParse(clientSock *client, requestLine *req, char *cmpletReq)
{
  printf("buf parse\n");
	if(strcmp(client->buf,"\r\n") == 0)   /* Request is not a complete request */
	{
		memset(client->buf,0,BUF_SIZE);
		client->alreadyRead=0;
		return 0;
	}
	char *temp = strstr(client->buf,"\r\n\r\n");
	if(temp)
	{
	
		int lenReq = strlen(client->buf)-strlen(temp);
		if(lenReq > 8182) 			/* Request length exceeds the limitation 8192 */
		{
      printf("long request line\n");
			memset(client->buf, 0, BUF_SIZE);
			req->status = 411;
		}	
		else					/* Normal request */
		{
      assert(req->status == 200 && "status must be 200");
      char *end = strncpy(cmpletReq, client->buf, lenReq+2);
			end[lenReq+2] = 0; /* null terminate */
		 	client->alreadyRead = 0;
      memset(client->buf, 0, BUF_SIZE);
		}
		return 1;
	} else{
    req->status = 400;
  }
	return 0;
}



void httpLineParser(char *temp, int lens, requestLine *argu){
		printf("temp ::  %s\n", temp);
    sscanf(temp,"%s %s %s", argu->method, argu->uri, argu->version);
    printf("line parser result: %s, %s, %s\n", argu->method, argu->uri, argu->version);
    parseUri(argu);
}

void parseUri(requestLine *argu){
    int tmpNum=0;
    int *tmpPort=NULL;
    if (strstr(argu->uri, "http://")) {
        if (sscanf(argu->uri, "http://%8192[^:]:%i/%8192[^\n]", argu->host, tmpPort, argu->relativePath) == 3) 
        {
        	tmpNum = 1;
        }else if (sscanf(argu->uri, "http://%8192[^/]/%8192[^\n]", argu->host, argu->relativePath) == 2) 
        {
        	tmpNum = 2;
        }else if (sscanf(argu->uri, "http://%8192[^:]:%i[^\n]", argu->host, tmpPort) == 2)
        {
        	tmpNum = 3;
        }else if (sscanf(argu->uri, "http://%8192[^/]", argu->host) == 1) 
        {
        	tmpNum = 4;
        }
    }
    else if (!strstr(argu->uri, "/")) {
        sprintf(argu->relativePath, "/");
    }
    else {
        strcpy(argu->relativePath, argu->uri);
    }

    if (tmpNum == 4) {
        sprintf(argu->relativePath, "/index.html");
    }
    else if (!strcmp(argu->relativePath, "/")) {
        strcat(argu->relativePath, "index.html");
    }

}
