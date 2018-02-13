#ifndef __VALUE_LIST_H__
#define __VALUE_LIST_H__
#include "string_list.h"
#include "property.h"

class ValueList : public StringList
{
private:	
	std::string valueSymbol;	
protected:
public:
	//operator
	//class ValuList & operator <<(std::string);
	//method
	std::string get_value(std::string key);
	int set_value(std::string key, std::string value);
	static std::string get_config_value(std::string fn, std::string key);
	
	//static std::string get_text_file_row(std::string, unsigned int);
	//useful properties
	//constructor
	ValueList(std::string symbol, bool winStyle=false);
	//destructor	
	~ValueList(void);
};

#endif //ndef __VALUE_LIST_H__
