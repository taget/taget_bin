#include <string>

using namespace std;

static int findstr(const string basestr, const string strstart, const string strend, string& str);

static int parsurl(const string url, string& host, string& path);

static int getfilename(const string path, string& filename);

static int add_val(const string name, const string val, string& ret);

static int get_val(const string header,const string name, string& val);
