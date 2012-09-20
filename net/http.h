/*
//
// http class
//
*/
#include "protocol.h"
#include "util.h"
#include "lin_exception.h"
#include <stdio.h>

#ifndef __HTTP_H_H
#define __HTTP_H_H

#define HTTP_OK 0
#define HTTP_PART 1
#define HTTP_ERROR 9

typedef struct _SERVER_INFO
{
        string                          strurl;
	string 				strhead;
        int                             i2read;
        int				istart;
	FILE**                          filehandle;
	void*				psocket;
}SERVER_INFO;

class http:public protocol
{
	
	public:
	http(){}
	http(string url);
	virtual const string wapperhead(const int start_range = 0);
        virtual const int writeheadtoserver();
	virtual const string readheadfromserver();
        virtual const int download(const string pathtosave, const int inum = 1);
	
	int parsresult();
	int setpsocket(linsocket* p);

};
#endif
