#include <fstream>
#include "value_list.h"

std::string 
ValueList::get_config_value(std::string fn, std::string key)
{
	std::string value;
	ValueList *configFile;
	configFile = new ValueList("=");
	configFile->load_from(fn);
	value = configFile->get_value(key);
	delete configFile;
	return value;
}

std::string
ValueList::get_value(std::string key)
{
	ExtensionString configStr(valueSymbol);
	std::string value;
	std::vector<std::string>::iterator item;	
	item = begin();
	value = "";
	while(item != end())	
	{
		configStr.string = *item++;
		if(configStr[0]==key)
		{
			value = configStr[1];
			break;
		}		
	}
	return value;
}

int 
ValueList::set_value(std::string key, std::string value)
{
	int ret = -1;
	ExtensionString configStr(valueSymbol);	
	std::vector<std::string>::iterator item;	
	item = begin();
	
	while(item != end())	
	{
		configStr.string = *item;
		if(configStr[0]==key)
		{
			configStr[1]=value;
			*item = configStr.string;
			ret = 0;
			break;
		}
		item++;
	}
	return ret;
}

ValueList::ValueList(std::string symbol, bool winStyle) 
	: StringList(winStyle)
{
	valueSymbol = symbol;
}

ValueList::~ValueList(void)
{
	
}
