#ifndef __EXTENSION_STRING_H__
#define __EXTENSION_STRING_H__
#include <vector>
#include <string>
#include "property.h"

class ExtensionString : public std::vector<std::string>
{
private:
	//std::string& _f_access_extension(int);
	std::string  _f_get_string(void);
	void _f_set_string(std::string);
protected:
	std::string const extSymbol;
public:	
	//operator
	class ExtensionString & operator <<(std::string in);
    class ExtensionString & operator >>(std::string &out);
	//method

	//useful properties
	property_rw<ExtensionString, std::string>string;
	//properties_rw<ExtensionString, std::string>extension;
	//constructor
	ExtensionString(std::string);
	//destructor	
	~ExtensionString(void);
};

#endif //ndef __EXTENSION_STRING_H__
