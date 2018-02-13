#include "svn_check_shell.h"

std::string const SVNCheckShell::shellFile="svn_check.sh";

SVNCheckShell::SVNCheckShell(std::string absFWBinary)
	: ExternShell(shellFile)
{
	//push parameter
	*this << absFWBinary;
}

SVNCheckShell::~SVNCheckShell(void)
{
}
