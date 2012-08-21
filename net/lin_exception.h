
#include "strutil.h"
#ifndef __LIN_EXCEPTION__H
#define __LIN_EXCEPTION__H
class linexception
{
	public:
	linexception(){}
	linexception(const char* errmsg){strerrmsg = string(errmsg);}
	//linexception(string errmsg){strerrmsg = errmsg;}
	
	public:
	
		string strerrmsg;
};
#endif
