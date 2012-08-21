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

const int http::wapperhead(const int start_range)
{
        strhead = "GET " + strpath + " HTTP/" + strversion;
        strutil::add_val("Accept", "*/*", strhead);
        strutil::add_val("Connection", "Keep-Alive", strhead);
        strutil::add_val("Host", strhost, strhead);
        strutil::add_val("Pragma"," no-cache", strhead);
        strutil::add_val("User-Agent", "GeneralDownloadApplication", strhead);
	if(0 != start_range)
	{
		char str[255];
		sprintf(str, "%d", start_range);
		strutil::add_val("Range: bytes=", str, strhead);
		strhead += "-";
	}
        strhead += "\r\n\r\n";
	return 0;
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
const int http::download(const string pathtosave, const int ithreadnum)
{
	string local_path_to_save = pathtosave + "/" + strfilename;
	string len;
	int ret;
	int len2read;
	u_char buffer[SOCK_MSG_LEN];

	try
	{
        	DEBUG(writeheadtoserver());
        	DEBUG(readheadfromserver()); 
		ret = parsresult();
	}
	catch(linexception& e)
	{
		throw e;
	}

	len2read = this->ilength;

	if (ithreadnum < 2) // sigle thread
	{
		FILE* fp = fopen (local_path_to_save.c_str(), "w");
        	while((ret = psocket->readSpecifiedData(buffer, len2read > SOCK_MSG_LEN ?\
            	 SOCK_MSG_LEN : len2read)) > 0  && len2read > 0)
       		{
        	        DEBUG(ret);
	                len2read -= ret;
                	DEBUG(len2read);
        	        fwrite(buffer,ret,1,fp);
       		}
	        if (len2read != 0)
                	throw linexception("Read error! len2read is not match !");
       		fclose(fp);
	}//end if itheadnum < 2
	else //
	{
		int i2read = len2read / ithreadnum;
		linsocket *tmpsocket = new linsocket(this->getport(), (this->gethost()).c_str());
		delete tmpsocket;
	}	
	return 0;
}
int http::parsresult()
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
