#include <string>
#include <stdlib.h>
using namespace std;
#ifndef __STRUTIL_H
#define __STRUTIL_H

class strutil
{
	public:

	static int findstr(const string basestr, const string strstart, const string strend, string& str);

	static int parsurl(const string url, string& host, string& path, int& port);

	static int getfilename(const string path, string& filename);

	static int add_val(const string name, const string val, string& ret);

	static int get_val(const string header,const string name, string& val);
};
#endif
