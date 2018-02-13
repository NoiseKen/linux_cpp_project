#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include "terminal_ctrl.h"
#include "key_ctrl.h"

XYLocate 
TerminalCtrl::_f_get_resolution(void)
{
	XYLocate res;
	struct winsize w;
	get_terminal_resolution(&w);
	res.x = w.ws_col;
	res.y = w.ws_row;
	return res;	
}

int
TerminalCtrl::get_terminal_resolution(struct winsize *w)
{
	return ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
}
/*
void 
TerminalCtrl::printf(std::string format, ...)
{
	va_list ap;
	va_start(ap, format);
	std::vprintf( format.c_str(), ap );
	va_end(ap);
}
*/
void
TerminalCtrl::config_terminal(ValidTerminalCode cc)
{
	
	termios term;
	void(*sig_handler)(int) = SIG_DFL;
	
	tcgetattr(STDOUT_FILENO, &term);
	switch(cc)
	{
		case TC_CONFIG_STANDARD:
		default:
			term.c_lflag |= (ICANON|ECHO);
			//sig_handler = SIG_DFL;
			break;
		case TC_CONFIG_NO_RESPONSE:
			sig_handler = SIG_IGN;
		case TC_CONFIG_NO_ECHO:
			term.c_lflag &= ~(ICANON|ECHO);
			break;					
	}
	
	signal(SIGINT, sig_handler);
	tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}

class TerminalCtrl& 
TerminalCtrl::operator << (std::string s)
{
	std::cout << s;
	return *this;
}

class TerminalCtrl& 
TerminalCtrl::operator << (unsigned char c)
{
	std::cout << c;
	return *this;
}

class TerminalCtrl& 
TerminalCtrl::operator << (ValidTerminalCode tc)
{
	if(tc < TC_VALID)
	{
		switch(tc)
		{
			case TC_CONFIG_STANDARD:
			case TC_CONFIG_NO_RESPONSE:
			case TC_CONFIG_NO_ECHO:
				config_terminal(tc);
				break;
			case TC_FLUSH:
				fflush(stdout);
			default:
				break;			
		}
	}
	return *this;
}

class TerminalCtrl& 
TerminalCtrl::operator << (AnsiGen::ValidEscapeCode ec)
{
	if(ec < AnsiGen::EC_VALID)		
	{
		std::cout << (AnsiGen() << ec);
	}	
	return *this;
}

class TerminalCtrl& 
TerminalCtrl::operator << (Color216 c)
{	
	std::cout << (AnsiGen() << c);
	return *this;
}

class TerminalCtrl& 
TerminalCtrl::operator << (XYLocate l)
{	
	std::cout << (AnsiGen() << l);
	return *this;
}

void
TerminalCtrl::flush(void)
{	
	fflush(stdout);
}

TerminalCtrl::TerminalCtrl(void) : resolution(this, &TerminalCtrl::_f_get_resolution)
{
}

TerminalCtrl::~TerminalCtrl(void)
{
}
