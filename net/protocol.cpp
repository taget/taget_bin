/*
//
// base class of protocol 
//
*/
#include "protocol.h"

protocol::protocol(string url)
{
	strurl = url;
	//strversion = version;
}
protocol::~protocol()
{
	delete psocket;
}
const string protocol::gethead()
{
	return strhead;
}
const string protocol::geturl()
{
	return strurl;
}
const string protocol::getversion()
{
	return strversion;
}
const int protocol::getlength()
{
	return ilength;
}
const int protocol::getport()
{
	return iport;
}

const string protocol::getrethead()
{
	return strrethead;
}
const string protocol::getpath()
{
	return strpath;
}
const string protocol::gethost()
{
	return strhost;
}
const int protocol::parsurl()
{
	return strutil::parsurl(strurl, strhost, strpath, iport);
}
const int protocol::init()
{
	parsurl();
	psocket = new linsocket(iport, strhost.c_str());
	psocket->init();
	wapperhead();
}
