#ifndef _TNVME_LOG_BACKUP_SHELL_H_
#define _TNVME_LOG_BACKUP_SHELL_H_
#include "extern_shell.h"

class TNVMeLogBackupShell : public ExternShell
{
public:
	static std::string const shellFile;
	TNVMeLogBackupShell(std::string tarName, std::string tarFolder);
	~TNVMeLogBackupShell(void);
protected:	
private:
};

#endif //_FILE_CTRL_H_
