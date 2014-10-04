#include "httpResponce.h"

char *filePath(char *relaPath){	
	char *fullPath;
	if(!relaPath){
		return NULL;
	}

	if((fullPath = malloc(strlen(relaPath)+
		strlen(argument.wwwPath)+1)))
	{
		strcpy(fullPath,argument.wwwPath);
		strcat(fullPath, relaPath);
	}
	return fullPath;
}

static int writeHttp(int fd, const char *buffer, const int totalLen)
{
	int totalWriten = 0;
	while (totalWriten != totalLen) {
		int writen = write(fd, buffer + totalWriten, totalLen - totalWriten);
		if(writen >=0)
		{
			totalWriten += writen;
		}
	}
	return 0;
}

static int writeHttps(SSL *client_context, const char *buffer, const int totalLen)
{
	int totalWriten = 0;
	printf("buffer :  %s\n", buffer);
	while (totalWriten != totalLen) {
		int writen = SSL_write(client_context, buffer + totalWriten, totalLen - totalWriten);
		if(writen >=0)
		{
			totalWriten += writen;
		}
	}
	return 0;

}
static char *handleRead(requestLine *req, clientSock *client)
{
		int pagefd = open(req->relativePath, O_RDONLY);
		
		if (!pagefd) 
		{
		
			fprintf(stderr, "error opening file %s\n", req->relativePath);
			req->status = 500; /* Internal error */

			return NULL;

			exit(-1);

		}

		char *src = mmap(NULL, req->contentLength, PROT_READ, MAP_SHARED, 
			pagefd, 0);
		if (MAP_FAILED == src)
		{
			fprintf(stderr, "error mapping file, errno %d, error %s\n",
				errno,
				strerror(errno));

			return NULL;
		}
		 close(pagefd);
	   return src;
	  

}
static void handleHttp(requestLine *req, clientSock *client)
{
	if (strcmp(req->method, "POST")==0 || strcmp(req->method, "GET")==0) 
	{	
		
			char *src = handleRead(req,client);
			writeHttp(client->sock, src, req->contentLength);
		
	}
}

static void handleHttps(requestLine *req, clientSock *client)
{
	if (strcmp(req->method, "POST")==0 || strcmp(req->method, "GET")==0) 
	{	
		char *src = handleRead(req,client);
		writeHttps(client->client_context, src, req->contentLength);
	}
}
/*
static void handleNotFound(requestLine *req, clientSock *client)
{

}
*/

void responseLine(requestLine *req,clientSock *client, SetPool *pool)
{
	checkReqst(req);
	contentType(req);
	//strcpy(req->connect, "Keep-Alive");
	statusAdd(req);
	headerAdd(req);
	strcat(req->header,"\r\n");
	//printf("lslslsl\n");
	//printf("client->type:: %d\n",client->type);
	if(client->type == HTTP)
	{
		write(client->sock, req->statusLine, strlen(req->statusLine));
		write(client->sock, req->header, strlen(req->header));
	//	printf("req->status: %d\n",req->status);

		if (req->status == 200)
		{
			handleHttp(req, client);
		} 
		
	}else if(client->type == HTTPS)
	{
		SSL_write(client->client_context,req->statusLine,strlen(req->statusLine));
		SSL_write(client->client_context,req->header,strlen(req->header));
		//printf("req->status: %d\n",req->status);
		if(req->status == 200)
		{
			handleHttps(req,client);
		}

	}	
}

int statusAdd(requestLine *req)
{
	switch(req->status){
		case 200:
			strcpy(req->statusLine,"HTTP/1.1 200 OK\r\n\0");
			break;
		case 400:
			strcpy(req->statusLine,"HTTP/1.1 400 BAD_REQUEST\r\n\0");
			break;
		case 404:
			strcpy(req->statusLine,"HTTP/1.1 404 NOT_FOUND\r\n\0");
			break;
		case 411:
			strcpy(req->statusLine,"HTTP/1.1 411 LENGTH_REQUIRED\r\n\0");
			break;
		case 500:
			strcpy(req->statusLine,"HTTP/1.1 500 INTERNAL_SERVER_EROR\r\n\0");
			break;
		case 501:
			strcpy(req->statusLine,"HTTP/1.1 501 NOT_IMPLEMENTED\r\n\0");
			break;
		case 503:
			strcpy(req->statusLine,"HTTP/1.1 503 SERVICE_UNAVILABLE\r\n\0");
			break;
		case 505:
			strcpy(req->statusLine,"HTTP/1.1 505 HTTP_VERSION_OT_SUPPORTED\r\n\0");
			break;
		
	}
	return req->status;
}


int contentType(requestLine *req){
	char *extension;
	if(!(extension = strrchr(req->relativePath, '.')))
	{
		req->status = 404;
		return 0;
	}
	if(!strcmp(extension,".jpg")){
		strcpy(req->contentType,"image/jpeg");
	}else if(!strcmp(extension,".png")){
		strcpy(req->contentType,"image/png");	
	}else if(!strcmp(extension,".html")){
		strcpy(req->contentType,"text/html");
	}else if(!strcmp(extension,".gif")){
		strcpy(req->contentType,"image/gif");
	}else if(!strcmp(extension,".css")){
		strcpy(req->contentType,"text/css");
	}else {
		strcpy(req->contentType,"Unknown");
	}
	return 1;

}

void headerAdd(requestLine *req)
{
	char temp[BUF_SIZE];
	char year[10],date[10],mon[10],hour[10],day[10];

	time_t now = time(0);
	strftime(temp, 256, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	strcpy(req->date, temp);
	/* Date add ends */
	sprintf(req->server,"Server: Liso/1.0\r\n");
	strcat(req->header,req->server);
	sprintf(temp,"Date: %s\r\n",req->date);
	strcat(req->header,temp);
	sprintf(temp,"Connection: %s\r\n",req->connect);
	strcat(req->header,temp);
	sprintf(temp,"Content-Type: %s\r\n",req->contentType);
	strcat(req->header,temp);
	sprintf(temp,"Content-Length: %d\r\n",req->contentLength);
	strcat(req->header,temp);
	if(req->status != 404)
	{
		sscanf(req->lastModified,"%s %s %s %s %s",day,mon,date,hour,year);
		sprintf(temp,"Last-Modified: %s, %s %s %s %s\r\n",day, date, mon, year,hour);
		strcat(req->header,temp);
	}
	
}

static void formatFileModifedTime(struct stat *file, char *dst)
{
	
	
	strcpy(dst, ctime(&file->st_mtime));
}

/* Check the integrity of the requst */
int checkReqst(requestLine *req)
{
	struct stat file;
	if(strcmp(req->method,"GET") != 0 &&
		strcmp(req->method,"POST") != 0 &&
		strcmp(req->method,"HEAD") != 0) {
		req->status = 501;
	}

	if(strcmp(req->version, "HTTP/1.1") != 0) {
		req->status = 505;

	}
	if(req->status == 200)
	{

		/* Get the full path of the request file */
		strcpy(req->relativePath, filePath(req->relativePath));
		//printf("filepath   %s\n\n", req->relativePath);
		if(stat(req->relativePath, &file) == -1)
		{
			req->status = 404;
			return -1;
		}

		req->contentLength = file.st_size;
		formatFileModifedTime(&file, req->lastModified);
	}
	
	return 1;
}