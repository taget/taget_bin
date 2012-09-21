/*
//
// lindownload class
//
*/
#include "util.h"
#include "lin_exception.h"
#include "strutil.h"
#include "lin_socket.h"
#include "protocolfactory.h"
#include "protocol.h"
#include "http.h"

#include <stdio.h>

#ifndef __LINDOWNLOAD__H
#define __LINDOWNLOAD__H

#define MAX_PT 32
typedef struct _CLIENT_INFO
{
        FILE**                          filehandle;
        void*                           pp;
}CLIENT_INFO;

class lindownload
{
	
	public:
	lindownload(){}
	lindownload(const string url, const int inum);
	~lindownload(){}
        int init();
	int download(const string pathtosave);
	
	private:
	string _strurl;
	int _ithreadnum;

};
#endif
