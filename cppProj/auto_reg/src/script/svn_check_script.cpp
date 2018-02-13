#include <unistd.h>
#include "auto_reg_manager.h"
#include "legacy_string.h"
#include "svn_check_script.h"
#include "auto_reg_cfg.h"

std::string const SVNCheckScript::morphResult=".svn.checkout";

SVNCheckShell *
SVNCheckScript::_new_shell(std::string localBin)
{
	//AutoRegCfg *cfg;
	//cfg = new AutoRegCfg();
	
	SVNCheckShell *shell;
	//shell = new FioTestShell(cfg->value[CFG_MPTOOL_ROOT]);
	shell = new SVNCheckShell(ExternShell::appRoot+"/"+localBin);
	//delete cfg;
	return shell;
}

bool 
SVNCheckScript::is_pass(void)
{
	bool pass;
	int rst=result();
	switch(rst)
	{
		case 0://new fw 
		case -88888:	//use smae fw do test again
			pass=true;
			break;
		default:
			pass=false;
			break;
	}
	
	return pass;
}

void
SVNCheckScript::shell_monitor(int sc)
{
	if(sc==SHELL_FINISH)
	{
		int rst=result();
		switch(rst)
		{
			case 0://new fw 
				AutoRegManager::cmd_schedule(AutoRegManager::CMD_INIT_CARD);
				break;
			case -88888:	//use smae fw do test again
				break;
		}
	}
}

void 
SVNCheckScript::script_log(std::vector<std::string> &list)
{	
	std::string morphLog;
	morphLog = extern_shell()->morph_result_file(morphResult);
	list.resize(2);	
	list[0]=morphLog;
	list[1]="svninfo";
}

SVNCheckScript::SVNCheckScript(std::string localBin)
	: ShellScript(ShellScript::SCRIPT_INFINITE, _new_shell(localBin), true)
{
}

SVNCheckScript::~SVNCheckScript(void)
{
}
