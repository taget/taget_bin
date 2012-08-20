#include <ctype.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include "util.h"

#ifndef LIN_SOCKET_H
#define LIN_SOCKET_H

#define SERVER 0
#define CLIENT 1
#define SOCK_MSG_LEN 4096
class linsocket
{
	public:
	linsocket(){};
	linsocket(const int nPort, const char* ch_server = NULL);
	int setNonblockMode();
	int SocketServer(const short nPort);
	int SocketClient(const char* ch_server, const int nPort);
	int readSpecifiedData(unsigned char lpszLineBuffer[], const int iNum);
	int writeSpecifiedData(unsigned char lpszLineBuffer[], const int iNum);
	int init();
	int getfd(){return _ifd;}

	private:
	int _ifd;
	int _type;
	int _nPort;
	char* _ch_server;
};

#endif

