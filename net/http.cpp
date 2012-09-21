/*
//
// http class
//
*/
#include "http.h"

http::http(string url):protocol(url)
{
	strversion = "1.1";
	istart = 0;
}

const string http::wapperhead()
{
        strhead = "GET " + strpath + " HTTP/" + strversion;
        strutil::add_val("Accept", "*/*", strhead);
        strutil::add_val("Connection", "Keep-Alive", strhead);
        strutil::add_val("Host", strhost, strhead);
        strutil::add_val("Pragma"," no-cache", strhead);
        strutil::add_val("User-Agent", "GeneralDownloadApplication", strhead);
	if(0 != istart)
	{
		char str[255];
		sprintf(str, "%d", istart);
		strhead += "\r\nRange: bytes=";
		strhead += str;
		strhead += "-";	
	}
        strhead += "\r\n\r\n";
	return strhead;
}
const int http::writeheadtoserver()
{
	return psocket->writeSpecifiedData((unsigned char*)strhead.c_str(), strhead.length());
}

const string http::readheadfromserver()
{
	u_char buffer = 0;
	int ret;
	int ifind = 0;
	strrethead = "";
	while((ret = psocket->readSpecifiedData(&buffer, 1)) != 0)
	{
		strrethead += buffer;
		switch(ifind)
		{
			case 0:
				if (buffer == '\r')
				{
					ifind ++;
				}
				break;
			case 1:
				if (buffer == '\n')
				{
					ifind ++;
				}
				else
				{
					ifind = 0;
				}
				break;
			case 2:
				if (buffer == '\r')
				{
					ifind ++;
				}
				else
				{
					ifind = 0;
				}
				break;
			case 3:
				return strrethead;
		}
		
	}
	return strrethead;
}
const int http::readfromserver(u_char* buffer, const int num)
{
	return  psocket->readSpecifiedData(buffer, num);
}

const int http::parsresult()
{
	string len;
	int ret = -1;
        if(strrethead.find("200 OK") != string::npos)
        {
		if(strutil::get_val(strrethead, "Content-Length", len) != 0)
		{
			throw linexception("200 OK, but Content-Length not found");
		}
                ret = HTTP_OK;
        }
	else if (strrethead.find("206 Partial Content") != string::npos)
	{
		if(strutil::get_val(strrethead, "Content-Length", len) != 0)
                {
                        throw linexception("206 Content-Length not found");
		}
		ret = HTTP_PART;
	}
	else
	{
		throw linexception("Http ERROR");
	}
	ilength = atoi(len.c_str());
	return ret;
}
int http::setpsocket(linsocket* p)
{
	if(psocket != NULL && p != psocket)
	{
		delete psocket;
		psocket = NULL;
	}
	psocket = p;

	return 0;
}
