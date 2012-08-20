/*
//
// http class
//
*/
#include "protocol.h"

#ifndef __HTTP_H_H
#define __HTTP_H_H
class http:public protocol
{
	
	public:
	http(){}
	http(string url);
	virtual const int wapperhead();
        virtual const int writeheadtoserver();
	virtual const string readheadfromserver();
};
#endif
