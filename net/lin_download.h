/*
//
// lindownload class
//
*/
#include "util.h"
#include "lin_socket.h"
#include "lin_exception.h"
#include <stdio.h>

#ifndef __LINDOWNLOAD__H
#define __LINDOWNLOAD__H

class lindownload
{
	
	public:
	lindownload(){}
	lindownload(const string url, const int inum);
        int init();
	int download(const string pathtosave);
	
	private:
	string _strurl;
	int _ithreadnum;

};
#endif
