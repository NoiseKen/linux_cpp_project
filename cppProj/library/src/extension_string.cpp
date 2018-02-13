#include "extension_string.h"

std::string 
ExtensionString::_f_get_string(void)
{
	std::string str;	
	unsigned int last;	
	std::vector<std::string>::iterator item;

	last = size();
	item = begin();
	while(last)
	{
		str.append(*item);
		item++;
		if(--last)
		{
			str.append(extSymbol);
		}
	}	
	return str;
}


void 
ExtensionString::_f_set_string(std::string inStr)
{
	size_t pos;
	std::string element;	
	//clear all item in vector
	clear();
	pos = 0;
	while(pos != std::string::npos)
	{	
		pos = inStr.find( extSymbol, 0 );
		push_back(inStr.substr(0, pos));
		//inStr = inStr.substr(pos+1, std::string::npos);
		inStr = inStr.substr(pos+extSymbol.length(), std::string::npos);
	}	
}

class ExtensionString & 
ExtensionString::operator <<(std::string in)
{
	push_back(in);
	return *this;
}

class ExtensionString & 
ExtensionString::operator >>(std::string &out)
{
    out=*(end()-1);
    pop_back();
    return *this;
}

ExtensionString::ExtensionString(std::string ext)
					: extSymbol(ext)
					, string(this, &ExtensionString::_f_get_string, &ExtensionString::_f_set_string)					
{
}

ExtensionString::~ExtensionString(void)
{
}
