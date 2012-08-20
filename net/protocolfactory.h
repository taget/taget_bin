#include "protocol.h"
#include "http.h"

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




