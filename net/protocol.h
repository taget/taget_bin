/*
//
// base class of protocol 
//
*/
#include "strutil.h"
#include "lin_socket.h"

#ifndef __PROTOCOL_H
#define __PROTOCOL_H
class protocol
{
	public:
	protocol(){}
	protocol(string url);
	virtual ~protocol();
	virtual const int init();
	virtual	const int parsurl();
	virtual const int wapperhead() = 0;
	virtual const string readheadfromserver() = 0;
	virtual const int writeheadtoserver() = 0;

	const string gethead();
	const string geturl();
	const string getversion();
	const string getrethead();
	const string getpath();
	const string gethost();

	const int getlength();
	const int getport();

	public:

	string strhead;		//head
	string strurl;		//url
	string strrethead;	//return head
	
	string strhost;		//server host
	string strversion;	//protocol version
	string strpath;		//file patch
	int ilength;		//file length 
	int iport;		//port
	
	public:
	
	linsocket * psocket;	
};
#endif
