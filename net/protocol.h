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
	virtual const string wapperhead() = 0;
	virtual const string readheadfromserver() = 0;
	virtual const int writeheadtoserver() = 0;
	// download(pathtosave , thread number)
	virtual const int readfromserver(u_char* buffer, const int num) = 0;
	virtual const int parsresult() = 0;

	const string gethead();
	const string geturl();
	const string getversion();
	const string getrethead();
	const string getpath();
	const string gethost();
	const string getfilename();

	const int getlength();
	const int getport();
	const int getlen2read();
	const int getstart();

	const int setlen2read(const int len2read);
	const int setstart(const int istart);
	public:

	string strhead;		//head
	string strurl;		//url
	string strrethead;	//return head
	
	string strhost;		//server host
	string strversion;	//protocol version
	string strpath;		//file patch in server
	string strfilename;	//file name
	int ilength;		//file length 
	int iport;		//port
	
	int ilen2read;		//length to read num
	int istart;		//start pos
	public:
	
	linsocket * psocket;	
};
#endif
