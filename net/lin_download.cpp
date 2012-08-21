/*
//
// lindownload class
//
*/
#include "lin_download.h"

lindownload::lindownload(string url, int inum)
{
	_strurl = url;
	_ithreadnum = inum;
}

int lindownload::init()
{
	return 0;
}

int lindownload::download(const string pathtosave)
{
	return 0;	
}
