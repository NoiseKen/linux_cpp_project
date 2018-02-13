#include <unistd.h>
#include "auto_log_backup_script.h"
#include "auto_reg_cfg.h"
#include "auto_reg_manager.h"
#include "log_file.h"
#include "legacy_string.h"

TNVMeLogBackupShell *
AutoLogBackupScript::_new_shell(void)
{
	AutoRegCfg *arc;
	arc = new AutoRegCfg();
	
	TNVMeLogBackupShell *shell;
	shell = new TNVMeLogBackupShell( arc->value[CFG_AUTO_LOG_COMPRESS_FILE]
									,arc->value[CFG_AUTO_LOG_FOLDER]);
	delete arc;
	return shell;
}

void 
AutoLogBackupScript::script_log(std::vector<std::string> &list)
{
	list.resize(1);
	AutoRegCfg *arc;
	arc = new AutoRegCfg();	
	list[0]=arc->value[CFG_AUTO_LOG_COMPRESS_FILE]+".tar.gz";
	delete arc;
}


#if 0
void
AutoLogBackupScript::shell_monitor(int sc)
{
	if(sc==SHELL_FINISH)
	{
		if(0==result())
		{//init card success
			AutoRegManager *arm;
			arm = new AutoRegManager();
			arm->cmd_schedule(AutoRegManager::CMD_DO_REBOOT);
			delete arm;
		}
	}
}
#endif

AutoLogBackupScript::AutoLogBackupScript(void)
	: ShellScript(ShellScript::SCRIPT_INFINITE, _new_shell())
{
	AutoRegManager *arm;
	LegacyString *ls;
	ls = new LegacyString();
	arm = new AutoRegManager();
	//copy all fio_test log to log 
	std::string from, to;
	from = ExternShell::appRoot+"/fio_*.log";
	to = arm->abs_auto_log_folder()+"/.";
	ls->sprintf("mv %s %s", from.c_str(), to.c_str());
	system(ls->str().c_str());
	delete arm;
	delete ls;	
}

AutoLogBackupScript::~AutoLogBackupScript(void)
{
}
