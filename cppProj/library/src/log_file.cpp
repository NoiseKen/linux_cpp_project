#include <fstream>
#include "log_file.h"

class LogFile &
LogFile::operator<<(std::string in)
{
	if(is_exist())
	{
		std::fstream *fs;
		fs = new std::fstream(fileName.c_str(), std::fstream::out|std::fstream::app);
		if(fs != NULL)
		{
			in.push_back('\n');		
			fs->write(in.c_str(), in.size());
			fs->flush();
			delete fs;		
		}
	}
	return *this;
}

int 
LogFile::_f_get_file_line_count(void)
{
	int cnt=-1;
	if(is_exist())
	{
		std::istream *fs;	
		fs = new std::fstream(fileName.c_str(), std::fstream::in);
		if(fs != NULL)
		{
			char *buf;
			buf = new char[PATH_MAX];
			while(!fs->eof())
			{
				fs->getline(buf, PATH_MAX);
				cnt++;
			}
			delete buf;
			delete fs;
		}
	}
	return cnt;
}

std::string 
LogFile::_f_get_file_line(int i)
{
	std::string str="";
	if(is_exist())
	{
		int cnt=0;
		std::istream *fs;
		fs = new std::fstream(fileName.c_str(), std::fstream::in);
		if(fs != NULL)
		{
			char *buf;
			buf = new char[PATH_MAX];
			while(!fs->eof())
			{
				fs->getline(buf, PATH_MAX);
				str = std::string(buf);
				if(i>=0)
				{
					if(i==cnt)
					{
						break;
					}
				}			
				cnt++;
			}
			delete buf;
			delete fs;
		}		
	}	
	return str;
}

LogFile::LogFile(std::string fn, bool deleteAfterClose) : FileCtrl(fn)
	, lifeCycleOnly(deleteAfterClose)
	, lineCount(this, &LogFile::_f_get_file_line_count)
	, line(this, &LogFile::_f_get_file_line)
{
	if(!is_exist())
	{
		create(fn);
	}
}

LogFile::~LogFile(void)
{
	if(lifeCycleOnly)
	{
		destory(fileName.c_str());
	}
}
