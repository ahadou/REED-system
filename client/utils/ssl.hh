/*
 * ssl.hh
 */

#ifndef __SSL_HH__
#define __SSL_HH__

#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sysexits.h>
#include <unistd.h>

#include "openssl/bio.h"
#include "openssl/err.h"
#include "openssl/ssl.h"

#define CLCRT "keys/sslKeys/client-cert.pem"
#define CLKEY "keys/sslKeys/client-key.pem"
#define CACRT "keys/sslKeys/ca-cert.pem"

#define SOCKET_BUFFER_SIZE (8 + 4 * 1024 * 1024)

/* action indicators */
#define SEND_META (-1)
#define SEND_DATA (-2)
#define GET_STAT (-3)
#define INIT_DOWNLOAD (-7)

using namespace std;

class Ssl {
private:
    /* port number */
    int hostPort_;

    /* ip address */
    char* hostName_;

    /* address structure */
    struct sockaddr_in myAddr_;

    /* host socket */
    SSL_CTX* ctx_;
    SSL* ssl_;

    char buffer_[SOCKET_BUFFER_SIZE];

public:
    /*
		 * constructor: initialize sock structure and connect
		 *
		 * @param ip - server ip address
		 * @param port - port number
		 */
    Ssl(char* ip, int port, int userID);

    int hostSock_;

    /*
		 * @ destructor
		 */
    ~Ssl();

    /*
		 * basic send function
		 * 
		 * @param raw - raw data buffer_
		 * @param rawSize - size of raw data
		 */
    int genericSend(char* raw, int rawSize);

    /*
		 * data download function
		 *
		 * @param raw - raw data buffer
		 * @param rawSize - the size of data to be downloaded
		 * @return raw
		 */
    int genericDownload(char* raw, int rawSize);

    void closeConn();

    /*
		 * metadata send function
		 *
		 * @param raw - raw data buffer_
		 * @param rawSize - size of raw data
		 *
		 */
    int sendMeta(char* raw, int rawSize);

    /*
		 * data send function
		 *
		 * @param raw - raw data buffer_
		 * @param rawSize - size of raw data
		 *
		 */
    int sendData(char* raw, int rawSize);

    /*
		 * status recv function
		 *
		 * @param statusList - return int list
		 * @param num - num of returned indicator
		 *
		 * @return statusList
		 */
    int getStatus(bool* statusList, int* numOfShare);

    /*
		 * initiate downloading a file
		 *
		 * @param filename - the full name of the targeting file
		 * @param namesize - the size of the file path
		 *
		 *
		 */
    int initDownload(char* filename, int namesize);

    /*
		 * download a chunk of data
		 *
		 * @param raw - the returned raw data chunk
		 * @param retSize - the size of returned data chunk
		 * @return raw 
		 * @return retSize
		 */
    int downloadChunk(char* raw, int* retSize);
};

#endif
