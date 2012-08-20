#include "lin_socket.h"


linsocket::linsocket(const int nPort, const char* ch_server = NULL)
{
	_ch_server = ch_server;
	_nPort = nPort;
	DEBUG(_ch_server);
	DEBUG(_nPort);
}
int linsocket::init()
{
	if(_ch_server == NULL)
	{
		_type = SERVER;
		return SocketServer(_nPort);
	}
	else
	{
		_type = CLIENT;
		return SocketClient(_ch_server ,_nPort);
	}
	return 0;
}     

int linsocket::setNonblockMode()
{
	int iRet;

	if((iRet = fcntl(_ifd, F_GETFL, 0)) < 0)
	{
		return -1;
	}
	if((fcntl(_ifd, F_SETFL, iRet | O_NONBLOCK)) < 0)
	{
		return -1;
	}
	return 0;
}
int linsocket::SocketServer(const short nPort)
{
	int sock;
	struct sockaddr_in addr;
	int opt = 1;
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		return -1;
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	setNonblockMode();	
	
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(nPort);
	
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr)) < 0)
	{
		return -2;
	}
	
	if(listen(sock,20) < 0)
	{
		return -3;
	}
	
	return sock;
}

/*
 * Create a socket by server name and port
 */
int linsocket::SocketClient(const char* ch_server, const int nPort)
{
        int n;
        struct sockaddr_in addr;
	struct hostent *phent = NULL;
	_ifd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(_ifd < 0)
        {
                return -1;
        }
	
	phent = gethostbyname(ch_server);
	if(phent == NULL)
		return -1; 
	  
	memset(&addr, 0, sizeof(addr));
        addr.sin_addr.s_addr = *((unsigned long *)phent->h_addr);
	addr.sin_family = AF_INET;
        addr.sin_port = htons(nPort);
        
        n = connect(_ifd, (struct sockaddr*)&addr ,sizeof(addr));
        if(-1 == n)
	{
                return -1;
	}
	DEBUG(_ifd);
	return 0;
}


int linsocket::readSpecifiedData(unsigned char lpszLineBuffer[], const int iNum)
{
        int iBytes=0, iTotalBytes=0;

        if (_ifd < 0)
                return -1;
        assert(lpszLineBuffer != NULL);
        while(iTotalBytes < iNum)
        {
                iBytes = read(_ifd, lpszLineBuffer+iTotalBytes, iNum-iTotalBytes);
                if(iBytes < 0)
                {
                        if((errno == EAGAIN) || (errno == EINTR))
                                continue;
                        return -1;
                }
                else if(iBytes > 0)
                        iTotalBytes += iBytes;
                else
                        break;
        }
        return iTotalBytes;

}

int linsocket::writeSpecifiedData(unsigned char lpszLineBuffer[], const int iNum)
{
	int iBytes=0, iTotalBytes=0;

	assert(lpszLineBuffer != NULL);
	while(iTotalBytes<iNum)
	{
		iBytes = write(_ifd, lpszLineBuffer+iTotalBytes, iNum-iTotalBytes);
		if(iBytes < 0)
		{
			if((errno == EAGAIN) || (errno == EINTR))
				continue;
			return -1;
		}
		else if(iBytes > 0)
			iTotalBytes += iBytes;
		else
			break;
	}
	return iTotalBytes;
}
