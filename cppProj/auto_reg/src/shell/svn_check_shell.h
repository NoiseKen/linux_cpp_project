#ifndef _SVN_CHECK_SHELL_H_
#define _SVN_CHECK_SHELL_H_
#include "extern_shell.h"

class SVNCheckShell : public ExternShell
{
public:
	static std::string const shellFile;
	SVNCheckShell(std::string absFWBinary);
	~SVNCheckShell(void);
protected:	
private:
};

#endif //_FILE_CTRL_H_
