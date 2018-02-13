#include <unistd.h>
#include <fstream>
#include <time.h>
#include "file_ctrl.h"


std::string 
FileCtrl::get_date_time(void)
{
	std::string dateTime;
	time_t	now = time(0);
	struct tm  tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	dateTime = std::string(buf);
	return dateTime;
}

int
FileCtrl::get_current_root(std::string &root)
{
    int sc;
    char *temp;
    temp = new char[PATH_MAX];
    if (getcwd(temp, PATH_MAX)!= 0) 
    {
		root = std::string(temp);
		sc = 0;
	}
	else
	{
		sc = -errno;
	}
	delete temp;
	return sc;
}

uint64_t 
FileCtrl::file_size(std::string file)
{
	uint64_t size=0;
    if(file=="") file=fileName;
    if(this->is_exist(file))
    {
        FILE *pfile;
        pfile=fopen(file.c_str(), "rb");
        fseek(pfile, 0, SEEK_END);
        size=ftell(pfile);
        fclose(pfile);
    }
	return size;
}

int 
FileCtrl::create(std::string file)
{
	int sc=-1;
	std::fstream *fs;
	if(file=="") file=fileName;
	fs = new std::fstream(file.c_str(), std::fstream::out|std::fstream::trunc|std::fstream::binary);
	if((fs != NULL)&&this->is_exist())
	{
        /*
		std::string str;
		str = "dddd";
		str.push_back('\n');
		fs->write(str.c_str(), str.size());
		fs->flush();
        fs->close();
        */
		sc = 0;
		delete fs;
	}	
	return sc;	
}

int 
FileCtrl::destory(std::string file)
{
	int sc;
	if(file=="") file=fileName;	
	sc = remove(fileName.c_str());
	sc = (sc==0)?0:-errno;
	return sc;
}

bool 
FileCtrl::is_exist(std::string file)
{
	bool exist;
	if(file=="") file=fileName;
	exist = (access(file.c_str(), F_OK )!=-1)?true:false;
	return exist;
}

FileCtrl::FileCtrl(std::string fn) : fileName(fn)
{
}

FileCtrl::~FileCtrl(void)
{
}
