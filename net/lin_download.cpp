/*
//
// lindownload class
//
*/
#include "lin_download.h"

static void* client_thread(void* client_info)
{
        
	FILE* fp = *(((CLIENT_INFO*)(client_info))->filehandle);
	protocol* p = (protocol*)((CLIENT_INFO*)(client_info))->pp;
        
	int offset = p->getstart();	
	int len2read  = p->getlen2read();
	int ret;
	u_char buffer[SOCK_MSG_LEN];
	string strrethead;
	DEBUG(offset);
	p->writeheadtoserver();
	strrethead = p-> readheadfromserver();
	DEBUG(strrethead);
	
	if(fp != NULL)
        {
		int pos = 0;
                while((ret = p->readfromserver(buffer, len2read > SOCK_MSG_LEN ?\
                 SOCK_MSG_LEN : len2read)) > 0  && len2read > 0)
                {
                        DEBUG(ret);
                        len2read -= ret;
                        DEBUG(len2read);
			fseek(fp, offset + pos, SEEK_SET);
                        fwrite(buffer, ret, 1, fp);
			pos += ret;
                }
                if (len2read != 0)
                        throw linexception("Read error! len2read is not match !");
        }
	return NULL; 
}

lindownload::lindownload(string url, int inum)
{
	_strurl = url;
	_ithreadnum = inum;
}

int lindownload::init()
{
	return 0;
}

int lindownload::download(const string pathtosave)
{
	DEBUG(pathtosave);
	int len2read;
	protocol* pp = protocolfactory::getprotocol(_strurl);
        try
        {
        	DEBUG(pp->writeheadtoserver());
        	DEBUG(pp->readheadfromserver()); 
		pp->parsresult();
		len2read = pp->getlength();
		string localpath = pathtosave + pp->getfilename();
		FILE* fp = fopen (localpath.c_str(), "w");
		if(NULL == fp)
			throw;
		
		pthread_t pid;
		pthread_t pt[MAX_PT]; //MAX_PT is the maxthreadnum;
		CLIENT_INFO clientinfo[MAX_PT]; 
		int inum = len2read / _ithreadnum;
		int lenread = 0;
		for(int i = 0; i < _ithreadnum; i++)
		{
			int ileft = len2read - lenread;
			protocol* ptmpp = protocolfactory::getprotocol(_strurl);
			
			ptmpp->setstart(i*inum);	
			if(i == _ithreadnum - 1) // the last time
				ptmpp->setlen2read(ileft);
			else
				ptmpp->setlen2read(inum);

			ptmpp->wapperhead();
			
			clientinfo[i].filehandle = &fp;
			clientinfo[i].pp = (void*)ptmpp;
			
			lenread += ptmpp->getlen2read();
			
				
		}
		for (int i=0; i<_ithreadnum; i++)
        	{
                	pid = pthread_create(&pt[i], NULL, client_thread ,&(clientinfo[i]));
	                if(pid != 0)
        	                return 0;
               		else
	                {
        	                cout<<"you have create a new thread pid is "<<pt[i]<<endl;
               		}

        	}
        	for(int i=0; i<_ithreadnum; i++)
        	{
	                pthread_join(pt[i], NULL);
        	}
		for(int i=0; i<_ithreadnum; i++)
		{
			delete (protocol*)clientinfo[i].pp;
		}
		fclose(fp);
		cout << "File download to [" << pathtosave <<"/"<< pp->getfilename() << "]" <<endl;

	}
        catch(linexception& e)
        {
                cout << e.strerrmsg<<endl;
        }
	delete pp;
	pp = NULL;
	return 0;	
}
