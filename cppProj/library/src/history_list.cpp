#include <fstream>
#include "history_list.h"

const unsigned int HistoryList::nolimit=-1;

int
HistoryList::load_from(std::string fn)
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
				str.erase(str.end(), str.end());
				push_back(str);	
			}
		}
		ret = 0;
		delete buf;
		delete fs;
		
		//if limit item, resize it
		if(_depth != nolimit)
		{
			if(size()>_depth)
			{
				resize(_depth);
			}
		}		
	}

	return ret;
#undef	BUFFER_SIZE	//256		
}

class HistoryList& 
HistoryList::operator<<(const std::string appendStr)
{
	push_back(appendStr);
	return *this;
}

class HistoryList& 
HistoryList::operator=(class StringList src)
{

	unsigned int i;
	unsigned int size;

	size = src.size();

	if(_depth!=nolimit)
	{
		size = (size>_depth)?_depth:size;
	}

	for(i=0;i<size;i++)
	{
		push_back(src[i]);
	}	
	return *this;
}


void 
HistoryList::push_back(const std::string appendStr)
{
	if((_depth==size())&&(_depth!=nolimit))
	{		
		erase(begin());
	}

	((ExtensionString *)this)->push_back(appendStr);

}

HistoryList::HistoryList(unsigned int limit) : StringList()
{
	_depth = limit;
}

HistoryList::~HistoryList(void)
{
	
}
