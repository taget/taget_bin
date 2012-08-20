#include "protocolfactory.h"

protocol* protocolfactory::getprotocol(string strurl)
{
	protocol* pp = NULL;
	string host;
	string path;
	int port;
	strutil::parsurl(strurl, host, path, port);
	switch (port)
	{
		case 80:
			pp = new http(strurl);
			break;
		case 81:
			pp = new http(strurl);
			break;
		case 21:
			pp = NULL;
		default:
			pp = NULL;
	}
	if(pp != NULL)
	{
		pp->init();
	}
	return pp;
}

