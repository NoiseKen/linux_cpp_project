#include <unistd.h>
#include "init_card_script.h"
#include "auto_reg_cfg.h"
#include "auto_reg_manager.h"
#include "log_file.h"

std::string const InitCardScript::morphResult=".init.card";

InitializeCardShell *
InitCardScript::_new_shell(void)
{
	AutoRegCfg *cfg;
	cfg = new AutoRegCfg();
	
	InitializeCardShell *shell;
	shell = new InitializeCardShell(cfg->value[CFG_MPTOOL_ROOT], ExternShell::appRoot+"/"+cfg->value[CFG_FW_BIN_FOLDER]);
	delete cfg;
	return shell;
}

void 
InitCardScript::script_log(std::vector<std::string> &list)
{
	list.resize(1);
	list[0]=ExternShell::resultFile;
}

void
InitCardScript::shell_monitor(int sc)
{
	if(sc==SHELL_FINISH)
	{
		std::string cmd;
		cmd = "cat "+ExternShell::appRoot+"/"+ExternShell::resultFile;
		system(cmd.c_str());
		if(0==result())
		{//init card success
			AutoRegManager *arm;
			arm = new AutoRegManager();
			arm->cmd_schedule(AutoRegManager::CMD_DO_REBOOT);
			delete arm;
		}
	}
}

InitCardScript::InitCardScript(int to)
	: ShellScript(to, _new_shell(), true)
{
}

InitCardScript::~InitCardScript(void)
{
}
