/*
//
// http class
//
*/
#include "http.h"

http::http(string url):protocol(url)
{
	strversion = "1.1";
}

const int http::wapperhead()
{
        strhead = "GET " + strhost + " HTTP/" + strversion;
        strutil::add_val("Accept", "*/*", strhead);
        strutil::add_val("Connection", "Keep-Alive", strhead);
        strutil::add_val("Host", strhost, strhead);
        strutil::add_val("Pragma"," no-cache", strhead);
        strutil::add_val("User-Agent", "GeneralDownloadApplication", strhead);
        strhead += "\r\n\r\n";
	return 0;
}
const int http::readhead()
{
	strhead = "head";
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
		DEBUG(strrethead);
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
				return "";
		}
		
	}
	return strrethead;
}
