#include "strutil.h"
#include "util.h"
// findstr("http://xyz//abc", "://", "//", str) will return xyz
int strutil::findstr(const string basestr, const string strstart, const string strend, string& str)
{
	size_t pos_start, pos_end;
	int len_start = strstart.length();
	if((pos_start = basestr.find(strstart)) != string::npos)
	{
		if(strend.length() == 0)
		{
			pos_end = basestr.length();
		}
		else if((pos_end = basestr.find(strend, pos_start + len_start)) != string ::npos)
		{
			
		}
		else
			return 1;
		str = basestr.substr(pos_start + len_start, pos_end - pos_start - len_start);
		return 0;
	}
	else 
		return 1;
	
	
}

int strutil::parsurl(const string url, string& host, string& path, int& port)
{
/* http://sp1.yokacdn.com/photos/0c/c3/673154/photo_117482_500.jpg */
/* return sp1.yokacdn.com */
	int ret;
	string tmp;
	ret = findstr(url, "://", "/", host);
	DEBUG(host);
	ret = findstr(url, host, "", path);
	if(0 == findstr(host, "@", "", tmp)) // name:passwd@serverhost:port
	{
		string tmpport;
		findstr(tmp, ":", "", tmpport);
		host = tmp.substr(0, tmp.length() - tmpport.length() -1);
		port = atoi(tmpport.c_str());
	}
	else if(0 == findstr(host, ":", "", tmp))
	{
		host = host.substr(0, host.length() - tmp.length() - 1);
		port = atoi(tmp.c_str());
	}
	else 
	{
		if(0 == findstr(url, "http", "//", tmp))
			port = 80;
		else if(0 == findstr(url, "https", "//",tmp))
			port = 81;
		else if(0 == findstr(url, "ftp", "//",tmp))
			port = 21;
	}
	
	return ret;
		
}

int strutil::getfilename(const string path, string& filename)
{
// for /photos/0c/c3/673154/photo_117482_500.jpg return photo_117482_500.jpg 
	filename = path.substr(path.find_last_of('/') + 1, path.length());
	return 0;
}

int strutil::add_val(const string name, const string val, string& ret)
{
	ret +=  "\r\n" + name + ": " + val;
	return 0;
}

int strutil::get_val(const string header,const string name, string& val)
{
	size_t pos_start = header.find(name);
	size_t pos_end;
	if(pos_start != string::npos)
	{
		pos_end = header.find("\r\n", pos_start);
		val = header.substr(pos_start + name.length() + 2, pos_end);
		return 0;
	}
	return 1;
}
