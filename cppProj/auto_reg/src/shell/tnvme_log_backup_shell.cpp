#include "tnvme_log_backup_shell.h"

std::string const TNVMeLogBackupShell::shellFile="backup_log.sh";

TNVMeLogBackupShell::TNVMeLogBackupShell(std::string tarName, std::string tarFolder)
	: ExternShell(shellFile)
{
	//push parameter
	*this << tarName << tarFolder;	
}

TNVMeLogBackupShell::~TNVMeLogBackupShell(void)
{
}
