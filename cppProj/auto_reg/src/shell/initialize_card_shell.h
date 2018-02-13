#ifndef _INITIALIZE_CARD_SHELL_H_
#define _INITIALIZE_CARD_SHELL_H_
#include "extern_shell.h"

class InitializeCardShell : public ExternShell
{
public:
	static std::string const shellFile;
	InitializeCardShell(std::string mpToolsRoot, std::string svnFWRoot);
	~InitializeCardShell(void);
protected:	
private:
};

#endif //_FILE_CTRL_H_
