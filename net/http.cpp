/*
//
// http class
//
*/
#include "http.h"

//thread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void* client_thread(void* server_info)
{
	int len2read = ((SERVER_INFO*)(server_info))->i2read;
	string strurl = ((SERVER_INFO*)(server_info))->strurl;
        string strhead = ((SERVER_INFO*)(server_info))->strhead;
        FILE* fp = *(((SERVER_INFO*)(server_info))->filehandle);
        int offset = ((SERVER_INFO*)(server_info))->istart;
	linsocket* p = (linsocket*)((SERVER_INFO*)(server_info))->psocket;
        
	
	DEBUG(strhead);
	DEBUG(offset);
	p->init();
	p->writeSpecifiedData((unsigned char*)strhead.c_str(), strhead.length());
	
	u_char buffer = 0;
	u_char buffers[SOCK_MSG_LEN];
	int ret;
	int ifind = 0;
	string	strrethead = "";
	while((ret = p->readSpecifiedData(&buffer, 1)) != 0)
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
				DEBUG(strrethead);
				goto body;
		}
		
	}
body:
	// write data to file.
	DEBUG(strrethead);
	if(fp != NULL)
        {
		int pos = 0;
                while((ret = p->readSpecifiedData(buffers, len2read > SOCK_MSG_LEN ?\
                 SOCK_MSG_LEN : len2read)) > 0  && len2read > 0)
                {
                        DEBUG(ret);
                        len2read -= ret;
                        DEBUG(len2read);
			fseek(fp, offset + pos, SEEK_SET);
                        fwrite(buffers,ret,1,fp);
			pos += ret;
                }
                if (len2read != 0)
                        throw linexception("Read error! len2read is not match !");
        }
	return NULL; 
}
http::http(string url):protocol(url)
{
	strversion = "1.1";
}

const string http::wapperhead(const int start_range)
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
	FILE* fp = fopen (local_path_to_save.c_str(), "w");
	/*
	if (ithreadnum < 2) // sigle thread
	{
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
	}//end if ithreadnum < 2
	*/
	if(0)// this is only for testing.
		;
	else //
	{
		DEBUG(ithreadnum);
		SERVER_INFO serverinfo[32]; // 32 is the max threadnum.
		pthread_t pid;
		pthread_t pt[32]; //32 is the maxthreadnum;
		
		int inum = len2read /ithreadnum;
		int lenread = 0;
		for(int i = 0; i < ithreadnum; i++)
		{
			int ileft = len2read - lenread;
			linsocket *tmpsocket = new linsocket(this->getport(), (this->gethost()).c_str());
			serverinfo[i].istart = i * inum; //- 1 > 0 ? i*inum -1 : 0;
			if(i == ithreadnum - 1) // the last time
				serverinfo[i].i2read = ileft;
			else
			serverinfo[i].i2read = inum;
			serverinfo[i].strurl = this->strurl;
			serverinfo[i].strhead = this->wapperhead(serverinfo[i].istart);
			serverinfo[i].filehandle = &fp;
			serverinfo[i].psocket = (void*)tmpsocket;
			
			lenread += serverinfo[i].i2read;
			DEBUG(serverinfo[i].istart);
			DEBUG(serverinfo[i].i2read);
			DEBUG(serverinfo[i].strurl);
			DEBUG(serverinfo[i].strhead);
			
				
		}
		// create threads
		for (int i=0; i<ithreadnum; i++)
        	{
                	pid = pthread_create(&pt[i], NULL, client_thread ,&(serverinfo[i]));

	                if(pid != 0)
        	                return 0;
               		else
	                {
        	                cout<<"you have create a new thread pid is "<<pt[i]<<endl;
               		}

        	}
        	for(int i=0; i<ithreadnum; i++)
        	{
	                pthread_join(pt[i], NULL);
        	}
		for(int i=0; i<ithreadnum; i++)
		{
			delete (linsocket*)serverinfo[i].psocket;
		}
		/*
		pthread_create(&pt[0], NULL, client_thread, &(serverinfo[0]));
		pthread_join(pt[0], NULL);

		pthread_create(&pt[1], NULL, client_thread, &(serverinfo[1]));
		pthread_join(pt[1], NULL);
		*/
		//delete tmpsocket;
		//delete tmpsocket;
	}	
	fclose(fp);
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
