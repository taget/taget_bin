#include "util.h"
#include <pthread.h>

#ifndef __LIN_THREAD_H
#define __LIN_THREAD_H
class linthread
{
	public:
	linthread(){}
	~linthread(){}
	virtual int init();
	virtual int run();
	private:
	
};
#endif
