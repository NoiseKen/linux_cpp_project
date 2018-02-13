#include "initialize_card_shell.h"

std::string const InitializeCardShell::shellFile="initialize_card.sh";

InitializeCardShell::InitializeCardShell(std::string mpToolsRoot, std::string svnFWRoot)
	: ExternShell(shellFile)
{
	//push parameter
	*this << mpToolsRoot << svnFWRoot;
}

InitializeCardShell::~InitializeCardShell(void)
{
}
