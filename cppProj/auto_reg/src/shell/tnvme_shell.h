#ifndef _TNVME_SHELL_H_
#define _TNVME_SHELL_H_
#include "extern_shell.h"

class TNVMeShell : public ExternShell
{
public:
	static std::string const shellFile;
	TNVMeShell(std::string tnvmeSuitePath, std::string test, std::string logPath, std::string summary);
	~TNVMeShell(void);
protected:	
private:
};

#endif //_FILE_CTRL_H_
