#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <netdb.h>
#include "strutil.h"
#include "protocol.h"
#include "http.h"


using namespace std;

pthread_mutex_t mut;
#define SOCK_MSG_LEN    4096

//a saft exit signal flag
static int      s_iAlarmFlag = 0;
static int 		s_maxnum     = 0;
const  int      MAXNUM       = 128;

vector<string> vecCmds;

int maxnum = 128;

#define HTTPHEAD "\
GET /gun/wget/wget-1.9.tar.gz HTTP/1.0\r\n\
Host: ftp.gnu.org:80\r\n\
Accept: \*\/\*\r\n\
User-Agent: GeneralDownloadApplication\r\n\
Connection: close\r\n"

#define DEBUG(x) cout<<"[DEBUG] "<< __FILE__ << ":"<<__LINE__ <<" : " << #x << " : "<<x<<endl;
pthread_mutex_t mnt;
//a saft exit signal flag

typedef struct _SERVER_INFO
{
	string                          host;
	string				path;	
	int   			        port;
	int 				num;
}SERVER_INFO;

static void* client_thread(void* server_info);

void callSigInt(int iSig)
{
	s_iAlarmFlag = 1;
}
int setNonblockMode(int iFd)
{
	int iRet;

	if((iRet = fcntl(iFd, F_GETFL, 0)) < 0)
	{
		return -1;
	}
	if((fcntl(iFd, F_SETFL, iRet | O_NONBLOCK)) < 0)
	{
		return -1;
	}
	return 0;
}
int SocketServer(short nPort)
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

	setNonblockMode(sock);	
	
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
int SocketCLient(const char* ch_server, int nPort)
{
        int sock, n;
        struct sockaddr_in addr;
	struct hostent *phent = NULL;
        //sock = socket(AF_INET, SOCK_STREAM,0);
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock < 0)
        {
		cout<<"Open socket error!"<<endl;
                return -1;
        }
	
	phent = gethostbyname(ch_server);
	if(phent == NULL)
		return -1; 
	  
	memset(&addr, 0, sizeof(addr));
        addr.sin_addr.s_addr = *((unsigned long *)phent->h_addr);
	addr.sin_family = AF_INET;
        addr.sin_port = htons(nPort);
        
        n = connect(sock, (struct sockaddr*)&addr ,sizeof(addr));
        if(-1 == n)
	{
		cout<< "connect error!"<<endl;
                return -1;
	}
        return sock;

}
/*
int SocketClient(const char* ch_ip, int nPort)
{
	int sock, n;
	struct sockaddr_in addr;
	struct in_addr s;
	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0)
	{
		return -1;
	}

	//setNonblockMode(sock);	
	memset(&addr, 0, sizeof(addr));
	inet_pton(AF_INET, ch_ip,&s);
	addr.sin_family = AF_INET;
	addr.sin_addr = s;
	addr.sin_port = htons(nPort);
	
	n = connect(sock, (struct sockaddr*)&addr ,sizeof(addr));
	if(-1 == n)
		return -1;
	return sock;
       
}
*/

int readSpecifiedData(int iFd, unsigned char lpszLineBuffer[], int iNum)
{
	int iBytes=0, iTotalBytes=0;

	if (iFd < 0)
		return -1;
	assert(lpszLineBuffer != NULL);	
	while(iTotalBytes<iNum)
	{
		iBytes = read(iFd, lpszLineBuffer+iTotalBytes, iNum-iTotalBytes);
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

int writeSpecifiedData(int iFd, unsigned char lpszLineBuffer[], int iNum)
{
	int iBytes=0, iTotalBytes=0;

	if (iFd < 0)
		return -1;
	assert(lpszLineBuffer != NULL);
	while(iTotalBytes<iNum)
	{
		iBytes = write(iFd, lpszLineBuffer+iTotalBytes, iNum-iTotalBytes);
		if(iBytes < 0)
		{
			if((errno == EAGAIN) || (errno == EINTR))
				continue;
			cout<<"write socket failed"<<endl;
			return -1;
		}
		else if(iBytes > 0)
			iTotalBytes += iBytes;
		else
			break;
	}
	return iTotalBytes;
}


int savetofile(FILE* fp, const u_char* buffer, int size)
{
	return fwrite(buffer,size,1024,fp);
}

int readheadfromserver2(int fd, string& head)
{
	u_char buffer = 0;
	int ret;
	int ifind = 0;
	
	while((ret = readSpecifiedData(fd, &buffer, 1)) != 0)
	{
		head += buffer;
		switch(ifind)
		{
			case 0:
				if (buffer == '\r')
				{
					ifind ++;
				}
				break;
			case 1:
				if (buffer == '\n')
				{
					ifind ++;
				}
				else
				{
					ifind = 0;
				}
				break;
			case 2:
				if (buffer == '\r')
				{
					ifind ++;
				}
				else
				{
					ifind = 0;
				}
				break;
			case 3:
				return 0;
		}
		
	}
	return 1;
}

/*
int readheadfromserver(int fd, string& head, unsigned char* left)
{
	// return left byte after read head.
	u_char buffer[SOCK_MSG_LEN];
        int ret, nret;
        memset(buffer, 0, SOCK_MSG_LEN);
        ret = readSpecifiedData(fd, buffer, SOCK_MSG_LEN);
	string strtmp = string(buffer);
	size_t pos;
	while((pos = strtmp.find("\r\n\r\n")) == string::npos)
	{	
		head += strtmp;
		ret = readSpecifiedData(fd, buffer, SOCK_MSG_LEN);		
	}
	head += strtmp.substr(0, pos);
	nret = ret - head.length() - 4;
	memcpy(left, &buffer[head.length() + 4], nret);
	return nret;
}
*/
int readfromserver(int fd, string path)
{
// read from server , then save to path
        unsigned char leftbuf[SOCK_MSG_LEN];
	u_char buffer[SOCK_MSG_LEN];
        string rethead = "";
	string len = "0";
	int ret;
	readheadfromserver2(fd, rethead);
	DEBUG(rethead);
	if(rethead.find("OK") == string::npos)
	{	
		cout<< "Server refused" <<endl;
		return 1;
	}
        if(strutil::get_val(rethead, "Content-Length", len) !=0)
                return 2;	
	int len2read = atoi(len.c_str());
	FILE* fp = fopen (path.c_str(), "w");
	while((ret = readSpecifiedData(fd, buffer, len2read > SOCK_MSG_LEN ? SOCK_MSG_LEN : len2read)) > 0 \
             && len2read > 0)
	{		
		DEBUG(ret);
		len2read -= ret;
		DEBUG(len2read);
		fwrite(buffer,ret,1,fp);
	}
	if (len2read != 0)
		cout <<"read error!"<<endl;
	fclose(fp);
	return 0;
		
}
int writetoserver(int fd, string str)
{
	u_char buff[SOCK_MSG_LEN];
	memcpy(buff ,str.c_str(), str.length());
	DEBUG(buff);
	return writeSpecifiedData(fd, buff , str.length());
		
}
const string wapperhead(const string path, const string servername, const int nport)
{
	string strhead;
	strhead = "GET " + path + " HTTP/1.1";
	
	strutil::add_val("Accept", "*/*", strhead);
	strutil::add_val("Connection", "Keep-Alive", strhead);
	strutil::add_val("Host", servername, strhead);
	strutil::add_val("Pragma"," no-cache", strhead);
	strutil::add_val("User-Agent", "GeneralDownloadApplication", strhead);
	strhead += "\r\n\r\n";
	return strhead;
}
static void* client_thread(void* server_info) 
{
	string host = ((SERVER_INFO*)(server_info))->host;
	string path = ((SERVER_INFO*)(server_info))->path;
	int port = ((SERVER_INFO*)(server_info))->port;
	string filename;
	int sock = SocketCLient(host.c_str(), port);
	if(sock < 0)
	{
		cout << "Open socket error!"<<endl;
		return NULL;
	}	
	int ret = 0;
/*
	string str_httphead = wapperhead(path, host, port);
	DEBUG(str_httphead);
	DEBUG(sock);
	writetoserver(sock, str_httphead);
	getfilename(path, filename);
	cout<< "reading ... and save download to "<< filename <<endl;
	readfromserver(sock, filename);
*/	
    	close(sock);
	return NULL;	
}

int main(int argc,char **argv)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT,callSigInt);
	
        pthread_t pid;
	pthread_t pt[16];	
        int ithreadnum = 1;
	SERVER_INFO server_info;
	string url = string("http://sp1.yokacdn.com/photos/0c/c3/673154/photo_117482_500.jpg");
	DEBUG(argc);
	if(argc == 2)
	{
		url = string(argv[1]);
	}
	DEBUG(url);
	/*
	server_info.host = host;
	server_info.path = path;
	server_info.port = port;
	server_info.num = 1;
	*/
//
	protocol *p;
	p = new http(url);
	p->parse();
	p->wapperhead();	
	DEBUG(p->gethost());
	DEBUG(p->getpath());
	DEBUG(p->getport());
	DEBUG(p->gethead());
/*
	for (int i=0; i<ithreadnum ;i++)
	{
		pid = pthread_create(&pt[i], NULL, client_thread ,&server_info);
		
		if(pid != 0)
			return 0;
		else 
		{
			cout<<"you have create a new thread pid is "<<pt[i]<<endl;
		}

	}
	for(int i=0; i<ithreadnum; i++)
	{
		pthread_join(pt[i], NULL);
	}
	*/
	return 0;
}
