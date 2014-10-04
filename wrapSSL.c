#include "wrapSSL.h"

int initSSL()
{
	/************ SSL INIT ************/
    SSL_load_error_strings();
    SSL_library_init();

    /* we want to use TLSv1 only */
    if ((ssl_context = SSL_CTX_new(TLSv1_server_method())) == NULL)
    {
        fprintf(stderr, "Error creating SSL context.\n");
        return EXIT_FAILURE;
    }

    /* register private key */
    if (SSL_CTX_use_PrivateKey_file(ssl_context, argument.priKyFile,
                                    SSL_FILETYPE_PEM) == 0)
    {
        //SSL_CTX_free(ssl_context);
        fprintf(stderr, "Error associating private key.\n");
        return EXIT_FAILURE;
    }

    /* register public key (certificate) */
    if (SSL_CTX_use_certificate_file(ssl_context, argument.certKyFile,
                                     SSL_FILETYPE_PEM) == 0)
    {
        //SSL_CTX_free(ssl_context);
        fprintf(stderr, "Error associating certificate.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int socketSSLWrap(clientSock *client)
{
  if ((client->client_context = SSL_new(ssl_context)) == NULL)
        {
            close(sockHttps);
           // SSL_CTX_free(ssl_context);
            fprintf(stderr, "new: Error creating client SSL context: %s\n",
                ERR_reason_error_string(ERR_get_error()));
            return EXIT_FAILURE;
        }
        printf("wrap %p\n",client->client_context);

        if (SSL_set_fd(client->client_context,client->sock) == 0)
        {
            //close(sockHttps);
            //SSL_free(client->client_context);
            //SSL_CTX_free(ssl_context);
            fprintf(stderr, "set_fd: Error creating client SSL context: %s\n",
                ERR_reason_error_string(ERR_get_error()));
            return EXIT_FAILURE;
        }  
        int ret;
        if((ret = SSL_accept(client->client_context)) <= 0)
        {
            //close(sockHTTPs);
            //SSL_free(client_context);
            //SSL_CTX_free(ssl_context);
            fprintf(stderr,"errno: %d\n",ret);
            fprintf(stderr, "Error accepting (handshake) client SSL context: %s\n",
            ERR_reason_error_string(ERR_get_error()));
            //return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
}