#include "protocol.h"
#include "http.h"
#include "ftp.h"
#include "lin_exception.h"

#ifndef __PROTOCOL_FACTORY_H
#define __PROTOCOL_FACTORY_H

class protocol;

class protocolfactory
{
	public:
	protocolfactory(){}
	~protocolfactory(){}
	static protocol* getprotocol(string strurl);
	
};
#endif




