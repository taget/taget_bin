#include "protocolfactory.h"

protocol* protocolfactory::getprotocol(string strurl)
{
	protocol* pp = NULL;
	string host;
	string path;
	int port;
	strutil::parsurl(strurl, host, path, port);
	DEBUG(strurl);
	DEBUG(host);
	DEBUG(path);
	
	DEBUG(port);
	switch (port)
	{
		case 80:
			pp = new http(strurl);
			break;
		case 81:
			pp = new http(strurl);
			break;
		default:
		//case 21:
			pp = new ftp(strurl);
		//	throw linexception("FTP protocol");
		//default:
		//	throw linexception("Unknow protocol");
	}
	if(pp != NULL)
	{
		pp->init();
	}
	return pp;
}

