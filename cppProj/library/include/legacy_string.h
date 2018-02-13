#ifndef __LEGACY_STRING_H__
#define __LEGACY_STRING_H__
#include <sstream>
#include "property.h"

class LegacyString  : public std::stringstream
{
private:
	//void _f_set_string(std::string s) { str(s); };
	const char * _f_get_c_str(void) { return str().c_str(); };
protected:
public:
	//operator	
	class LegacyString & operator =(std::string);	
	class LegacyString & operator +=(std::string);
	//method
	std::string sprintf(const std::string format, ...);
	static int to_integer(std::string str);
	//useful properties	
	property_ro<LegacyString, const char *> data;
	//constructor
	LegacyString(void);
	//destructor	
	~LegacyString(void);
};

#endif //ndef __LEGACY_STRING_H__
