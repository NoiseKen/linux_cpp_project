#ifndef __STRING_LIST_H__
#define __STRING_LIST_H__
#include "extension_string.h"
#include "property.h"

class StringList : public ExtensionString
{
private:	
	static std::string dummyStr;	
protected:
public:
	static const unsigned int nolimit;
	//operator
	std::string & operator[](unsigned int i);	
	//method
	int search(std::string);
	virtual int load_from(std::string);
	virtual int save_to(std::string);
	//useful properties
	//constructor
	StringList(bool winStyle=false);
	//destructor	
	virtual ~StringList(void);
};

#endif //ndef __STRING_LIST_H__
