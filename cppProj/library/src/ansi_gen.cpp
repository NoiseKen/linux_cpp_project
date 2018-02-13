#include "ansi_gen.h"
#include "legacy_string.h"
using namespace std;
const char AnsiGen::_escCode[3]={'\33','[', 0};
const std::string
AnsiGen::_validEC[EC_VALID]={
	"2J",	//EC_CLR_SCREEN
	"K",	//EC_CLR_AFTER
	"s",	//EC_SAVE_CURSOR
	"u",	//EC_RESTORE_CURSOR
	"A",	//EC_CURSOR_UP
	"B",	//EC_CURSOR_DOWN
	"C",	//EC_CURSOR_FORWARD
	"D",	//EC_CURSOR_BACKWARD	
};

std::string 
AnsiGen::operator << (ValidEscapeCode i)
{	
	LegacyString lstr;
	lstr << _escCode;
	if(i<EC_VALID)
	{
		lstr << _validEC[i];
	}
	return lstr.str();
}

std::string 
AnsiGen::operator << (std::string s)
{
	return s.insert(0,_escCode);	
}

std::string
AnsiGen::operator << (XYLocate l)
{
	LegacyString lstr;
	lstr.sprintf("%s%02d;%02dH", _escCode, l.y, l.x);
	return lstr.str();
}

std::string
AnsiGen::operator << (Color216 c)
{	
	unsigned char color;	
	LegacyString lstr;
	std::string str;
	const int mark[2]={ 38, 48 };

	str = _escCode;
	if(c.type<COLOR_216_VALID)
	{
		color = 16 + 36*(c.red%6) + 6*(c.green%6) + c.black%6;		
		lstr.sprintf("%d;5;%dm", mark[c.type], color);
	}

	str += lstr.str();
	
	return str;
}

AnsiGen::AnsiGen(void)
{
}

AnsiGen::~AnsiGen(void)
{
}
