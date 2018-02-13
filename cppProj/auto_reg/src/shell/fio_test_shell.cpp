#include "fio_test_shell.h"

std::string const FioTestShell::shellFile="fio_test.sh";

FioTestShell::FioTestShell(std::string runTime, std::string testSize)
	: ExternShell(shellFile)
{
	//push parameter
	*this << runTime << testSize;
}

FioTestShell::~FioTestShell(void)
{
}
