#include <iostream>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

#include "lin_exception.h"
#include "strutil.h"
#include "lin_socket.h"
#include "protocolfactory.h"
#include "protocol.h"
#include "http.h"
#include "lin_download.h"

using namespace std;

static int      s_iAlarmFlag = 0;

typedef struct _SERVER_INFO
{
	string                          host;
	string				path;	
	int   			        port;
	int 				num;
}SERVER_INFO;

void callSigInt(int iSig)
{
	s_iAlarmFlag = 1;
}

static void* client_thread(void* server_info);

int main(int argc, char* argv[])
{
        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT,callSigInt);

        pthread_t pid;
        pthread_t pt[16];
        SERVER_INFO server_info;
        string url = string("http://sp1.yokacdn.com/photos/0c/c3/673154/photo_117482_500.jpg");
        DEBUG(argc);
        if(argc == 2)
        {
                url = string(argv[1]);
        }
	unsigned char ch[1024];
        DEBUG(url);
	protocol *p;
	try 
	{
		string downloadpath = "/tmp";
		p = protocolfactory::getprotocol(url);
	
		DEBUG(p->download(downloadpath));		
		DEBUG(p->gethost());
		DEBUG(p->getpath());
		DEBUG(p->getport());
		DEBUG(p->gethead());
		DEBUG(p->getfilename());
		DEBUG(p->getrethead());
		cout << "File download to [" << downloadpath <<"/"<< p->getfilename() << "]" <<endl;
	delete p;
	}
	catch(linexception& e)
	{
		cout << e.strerrmsg<<endl;
	}
	return 0;
}
