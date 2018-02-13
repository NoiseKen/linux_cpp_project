#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "utils.h"
//#include "terminal_ctrl.h"
//#include "key_ctrl.h"
#include "legacy_string.h"
#include "extern_shell.h"
#include "tnvme_script.h"
#include "file_ctrl.h"
#include "init_card_script.h"
#include "auto_reg_cfg.h"
#include "auto_reg_manager.h"
#include "auto_reg_server.h"
#define VER	"0.0.2"
#define VER_MAJOR 0
#define VER_MINOR 2
#define VER_BUILD     5

int main(int argc, char *argv[])
{
	bool localAuto=false;
	//std::string line;
	FileCtrl::get_current_root(ExternShell::appRoot);
	AutoRegManager *autoRegMgmt;
	AutoRegServer *autoRegServer;
	ShellScript *script=NULL;
	
	printf("==== Auto Regression Ver %d.%d.%d ====\n", VER_MAJOR, VER_MINOR, VER_BUILD);
//printf("TNVME GRP count = %d\n", TNVMeScript::group_count());
	autoRegMgmt = new AutoRegManager();
	autoRegMgmt->new_auto_log_folder();
	autoRegMgmt->check_reboot();
	
	if(argc>1)
	{
		if(strcmp(argv[1], "local")==0)
		{
			localAuto=true;
		}
	}
	
	
	printf("Device alive = %s\n", autoRegMgmt->device_alive()?"YES":"NO");
	
	if(!localAuto)
	{
		autoRegServer = new AutoRegServer();
		AutoRegServer::smb_mount_to_server();
		printf("smb path=%s\n", AutoRegServer::abs_server_folder().c_str());
	}
	else
	{
		printf("Local AutoREG!!\n");
		autoRegServer = NULL;
	}

	autoRegServer->localREG = localAuto;

	while(!autoRegMgmt->exit)
	{
		int sc;
		if(!localAuto)
		{
			autoRegServer->routine(script, autoRegMgmt->is_idle());
		}
		
		autoRegMgmt->routine(script);
		
		TimerCtrl::routine();
		
		if(NULL!=script)
		{			
			sc = script->routine();
			if(sc!=ShellScript::SHELL_RUNNING)
			{
				printf("script done -- %d/%d[sc/result]\n", sc, script->result());				
				if(!localAuto&&autoRegServer->ping_server())
				{
					LegacyString *ls;
					std::vector<std::string> logs;
					int rst=script->result();
					ls = new LegacyString();
					ls->sprintf("Test Status = %d -- %s", rst, script->is_pass()?"PASS":"FAIL"); 
					autoRegServer->test_status_update(ls->str());
					script->script_log(logs);
					autoRegServer->log_update(logs);
					logs.resize(0);
					delete ls;
					
					if(!script->is_pass())
					{
						printf("not pass, is blockScript=%s!!\n", script->blockScript?"YES":"NO");
						if(script->blockScript)
						{
							printf("clean all server command!!\n");
							autoRegServer->cmd_flow_finish();
						}
					}
				}
				delete script;				
				script=NULL;
			}
		}
		
	}
	delete autoRegServer;
	delete autoRegMgmt;
	printf("AutoREG Bye Bye!!\n");
    return 0;
}
