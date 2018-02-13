#include "tnvme_shell.h"

std::string const TNVMeShell::shellFile="newtest.sh";

TNVMeShell::TNVMeShell(std::string tnvmeSuitePath, std::string test, std::string logPath, std::string summary)
	: ExternShell(shellFile, "-t"+test)
{
	//push parameter
	*this << tnvmeSuitePath << test << logPath << summary;	
}

TNVMeShell::~TNVMeShell(void)
{
}
