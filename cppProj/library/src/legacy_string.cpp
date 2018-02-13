#include <stdarg.h>
#include <stdio.h>
#include "legacy_string.h"

int
LegacyString::to_integer(std::string str)
{
	int i32;
	LegacyString lstr;
	lstr = str;
	lstr >> i32;
	return i32;
}

std::string 
LegacyString::sprintf(const std::string format, ...)
{
	int n, size=100;
	std::string s;
	bool b=false;
	va_list ap;
	while (!b) 
	{
        s.resize(size);
        va_start(ap, format);
        n = vsnprintf((char*)s.c_str(), size, format.c_str(), ap);
        va_end(ap);
        if ((n>0) && ((b=(n<size))==true)) 
			s.resize(n); 
        else 
			size*=2;
	}
	
	str(s);
	return s;	
}

class LegacyString &
LegacyString::operator =(std::string s)
{
	str(s);	
	return *this;	
}

class LegacyString&
LegacyString::operator +=(std::string cat)
{
	std::string s;
	s = str() + cat;
	str(s);
	return *this;
}

LegacyString::LegacyString(void) : std::stringstream()
				, data(this, &LegacyString::_f_get_c_str)
{
}

LegacyString::~LegacyString(void)
{
}
