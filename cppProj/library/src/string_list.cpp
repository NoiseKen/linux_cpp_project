#include <fstream>
#include "string_list.h"

std::string StringList::dummyStr="";

int
StringList::load_from(std::string fn)
{
#define	BUFFER_SIZE	256
	int ret=-1;
	char *buf;
	std::fstream *fs;
	fs = new std::fstream(fn.c_str(), std::fstream::in);
	if(fs->good())
	{
		buf = new char[BUFFER_SIZE];
		while(!fs->eof())
		{
			fs->getline(buf, BUFFER_SIZE);
			if(!fs->eof())
			{
				std::string str;
				str = buf;
				str.erase(str.end()-(extSymbol.length()-1), str.end());
				push_back(str);	
			}
		}
		ret = 0;
		delete buf;
		delete fs;
	}		
	return ret;
#undef	BUFFER_SIZE	//256		
}

int
StringList::save_to(std::string fn)
{
	int ret=-1;
	std::fstream *fs;
	fs = new std::fstream(fn.c_str(), std::fstream::out|std::fstream::trunc);
	if(fs != NULL)
	{
		std::string str;
		str = string;// + extSymbol;
		str = str + extSymbol;
		//str.push_back('\n');
		fs->write(str.c_str(), str.size());
		fs->flush();
		ret = 0;
		delete fs;
	}	
	return ret;
}

std::string&
StringList::operator[](unsigned int i)
{
	std::string *str;
	unsigned int validIdx=size();
	if(i<validIdx)
	{
		std::vector<std::string>::iterator item;	
		item = begin()+i;
		str = &*item;		
	}
	else
	{
		str = &dummyStr;
	}
	return *str;
}

int 
StringList::search(std::string str)
{
	std::string line;
	int cnt;
	int i;
	cnt = size();
	
	for(i=0;i<cnt;i++)
	{
		line = (*this)[i];
		if(str == line)
		{
			break;
		}
	}
	
	if(i==cnt)
	{
		i = -1;
	}
	
	return i;
}

StringList::StringList(bool winStyle) 
	: ExtensionString((winStyle)?"\r\n":"\n")
{
}

StringList::~StringList(void)
{
	
}
