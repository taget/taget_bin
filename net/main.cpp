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

void callSigInt(int iSig)
{
	s_iAlarmFlag = 1;
}

static void* client_thread(void* server_info);

int main(int argc, char* argv[])
{
        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT,callSigInt);

        string url = string("http://sp1.yokacdn.com/photos/0c/c3/673154/photo_117482_500.jpg");
	int ithreadnum = 1;
        if(argc >= 2)
        {
                url = string(argv[1]);
        }
	if(argc >= 3)
	{
		ithreadnum = atoi(argv[2]);
	}
        DEBUG(url);
	lindownload* pd = new lindownload(url, ithreadnum);
	try 
	{
		pd->init();
		pd->download("./");
	}
	catch(linexception& e)
	{
		cout << e.strerrmsg<<endl;
	}
	delete pd;
	return 0;
}
