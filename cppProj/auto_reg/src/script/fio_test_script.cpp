#include <unistd.h>
#include "legacy_string.h"
#include "fio_test_script.h"
#include "auto_reg_cfg.h"
#include "auto_reg_manager.h"
#include "log_file.h"

FioTestShell *
FioTestScript::_new_shell(std::string runTime, std::string testSize)
{
//	AutoRegCfg *cfg;
//	cfg = new AutoRegCfg();
	
	FioTestShell *shell;
	//shell = new FioTestShell(cfg->value[CFG_MPTOOL_ROOT]);
	shell = new FioTestShell(runTime, testSize);
	//delete cfg;
	return shell;
}

void
FioTestScript::shell_monitor(int sc)
{
	if(sc==SHELL_FINISH)
	{
		if(0==result())
		{//fio test ok
#if 0
			AutoRegManager *arm;			
			arm = new AutoRegManager();
			if(arm->was_reboot())
			{
				LegacyString *ls;
				ls = new LegacyString();				
				//copy all fio_test log to log 
				std::string from, to;
				from = ExternShell::appRoot+"/fio_*.log";
				to = arm->abs_auto_log_folder()+"/.";
				ls->sprintf("mv %s %s", from.c_str(), to.c_str());
				system(ls->str().c_str());
				arm->cmd_schedule(AutoRegManager::CMD_RUN_TNVME_ALL);
				delete ls;				
			}
			delete arm;
#endif			
		}
	}
}

void 
FioTestScript::script_log(std::vector<std::string> &list)
{
	list.resize(4);
	list[0]="fio_rand_read.log";
	list[1]="fio_rand_write.log";
	list[2]="fio_seq_read.log";
	list[3]="fio_seq_write.log";
}

FioTestScript::FioTestScript(std::string runTime, std::string testSize)
	: ShellScript(ShellScript::SCRIPT_INFINITE, _new_shell(runTime, testSize))
{
}

FioTestScript::~FioTestScript(void)
{
}
