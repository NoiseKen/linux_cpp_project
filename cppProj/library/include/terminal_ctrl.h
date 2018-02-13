#ifndef _TERMINAL_CTRL_H_
#define _TERMINAL_CTRL_H_
#include <sys/ioctl.h>
#include "ansi_gen.h"
#include "property.h"

class TerminalCtrl
{	
public:
#define COLOR_HI_GREEN		(Color216){COLOR_216_FORE, 0, 5, 0}
#define COLOR_HI_RED		(Color216){COLOR_216_FORE, 5, 0, 0}
#define COLOR_SELECTED		(Color216){COLOR_216_BACK, 0, 0, 2}
#define COLOR_NORMAL		(Color216){COLOR_216_FORE, 5, 5, 5} << (Color216){COLOR_216_BACK, 0, 0, 0}
#define COLOR_SEPERATE		(Color216){COLOR_216_FORE, 0, 4, 4} << (Color216){COLOR_216_BACK, 0, 0, 0}
#define COLOR_NOTICE		(Color216){COLOR_216_FORE, 3, 3, 0} << (Color216){COLOR_216_BACK, 3, 0, 0}
#define COLOR_FOCUS			(Color216){COLOR_216_FORE, 5, 5, 5} << (Color216){COLOR_216_BACK, 0, 3, 3}

#define COLOR_SYMBOL_FOLDER	(Color216){COLOR_216_FORE, 5, 5, 5}
#define COLOR_SYMBOL_TASK	(Color216){COLOR_216_FORE, 5, 5, 0}
#define COLOR_SYMBOL_STRING	(Color216){COLOR_216_FORE, 2, 2, 2}
	typedef enum{
		TC_CONFIG_STANDARD=0,
		TC_CONFIG_NO_RESPONSE,
		TC_CONFIG_NO_ECHO,
		TC_FLUSH,
		TC_VALID,
	}ValidTerminalCode;
	//legacyString stream;
	//push operator	
	class TerminalCtrl& operator <<(unsigned char);		//output character
	class TerminalCtrl& operator <<(std::string);		//output string
	class TerminalCtrl& operator <<(AnsiGen::ValidEscapeCode);	//output ec
	class TerminalCtrl& operator <<(ValidTerminalCode);	//output tc
	class TerminalCtrl& operator <<(XYLocate);			//goto locat
	class TerminalCtrl& operator <<(Color216);			//change color
	property_ro<TerminalCtrl, XYLocate> resolution;
	//pop operator, this operator get expired info in each slot
	//override event function (for inheritance)
	//method
	static int get_terminal_resolution(struct winsize *);
	static void flush(void);
	//void printf(std::string format, ...);
	//useful properties	
	//constructor
	TerminalCtrl(void);
	//destructor
	~TerminalCtrl(void);
protected:	
private:
	static void config_terminal(ValidTerminalCode);
	XYLocate _f_get_resolution(void);
};

#endif //ndef _TERMINAL_CTRL_H_
