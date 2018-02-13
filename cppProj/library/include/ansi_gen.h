#ifndef _ANSI_GEN_H_
#define _ANSI_GEN_H_
#include <iostream>
typedef enum{
	COLOR_216_FORE=0,
	COLOR_216_BACK,
	COLOR_216_VALID,
}Color216Type;

typedef class{
public:
	Color216Type type;
	char red;
	char green;
	char black;
}Color216;

typedef class{
public:
	unsigned int x;
	unsigned int y;
}XYLocate;

/*
 * Class name : AnsiGen
 * Description: Generate ANSI escape code for control ternimal
 * Usage :
 *      AnsiGen ansi;								//declare & create ansi generator
 * 		cout << (ansi << EC_CLR_SCREEN);			//generate valid escape code & push to cout
 * 		cout << (ansi << (XYLocate){1, 10});		//goto xy
 * 		cout << (ansi << (Color216){COLOR_216_FORE, 4, 0, 0});	//change color (pure red for example)
 * 		cout << (ansi << "2J";	//direct input control code, if you know about it
 */
class AnsiGen
{	
public:
	typedef enum{
		EC_CLR_SCREEN=0,
		EC_CLR_AFTER,
		EC_SAVE_CURSOR,
		EC_RESTORE_CURSOR,
		EC_CURSOR_UP,
		EC_CURSOR_DOWN,
		EC_CURSOR_FORWARD,
		EC_CURSOR_BACKWARD,
		EC_VALID,
	}ValidEscapeCode;
	//push operator	
	std::string operator <<(ValidEscapeCode);	//get valid escape code
	std::string operator <<(std::string);	//direct assert ansi code just insert escape code before string
	std::string operator <<(XYLocate);		//locat
	std::string operator <<(Color216);		//color
	//pop operator, this operator get expired info in each slot
	//override event function (for inheritance)
	//method
	//useful properties	
	//constructor
	AnsiGen(void);
	//destructor
	~AnsiGen(void);
protected:	
private:
	static const char _escCode[3];
	static const std::string _validEC[EC_VALID];
};

#endif //ndef _ANSI_GEN_H_
