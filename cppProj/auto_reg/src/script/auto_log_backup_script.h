#ifndef _AUTO_LOG_BACKUP_SCRIPT_H_
#define _AUTO_LOG_BACKUP_SCRIPT_H_
#include "tnvme_log_backup_shell.h"
#include "shell_script.h"


class AutoLogBackupScript : public ShellScript
{
public:
	void script_log(std::vector<std::string> &list);
	AutoLogBackupScript(void);
	~AutoLogBackupScript(void);
protected:
	//void shell_monitor(int sc);
private:
	static TNVMeLogBackupShell *_new_shell(void);
};

#endif //_FILE_CTRL_H_
