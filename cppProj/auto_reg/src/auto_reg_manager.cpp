#include <unistd.h>
#include <dirent.h>
#include "log_file.h"
#include "auto_reg_cfg.h"
#include "auto_reg_manager.h"
#include "extern_shell.h"
#include "init_card_script.h"
#include "fio_test_script.h"
#include "tnvme_script.h"
#include "svn_check_script.h"
#include "auto_log_backup_script.h"
#include "directory_ctrl.h"
#include "terminal_ctrl.h"
#include "legacy_string.h"
#include "string_list.h"
#include "timer_ctrl.h"

bool AutoRegManager::exit=false;
bool AutoRegManager::wasReboot=false;
std::string const AutoRegManager::cmdFolder="cmd";
std::string const AutoRegManager::statusFolder="status";
std::string const AutoRegManager::lspciLog="lspci.log";

std::string const AutoRegManager::validCmd[CMD_VALID]={
	"init_card",
	"do_reboot",
	"fw_checkout",
	"new_summary",
	"run_fio",
	"run_tnvme",
	"run_tnvme_all",
	"log_backup",
	"run_tnvme_grp",
};

std::string const AutoRegManager::validEmergency[EMERGENCY_VALID]={
	"abort",	
};

std::string const AutoRegManager::validStatus[STATUS_VALID]={
	"reboot",
};

std::string 
AutoRegManager::abs_auto_log_folder(void)
{
	return ExternShell::appRoot+"/"+AutoRegCfg().value[CFG_AUTO_LOG_FOLDER];	
}

std::string 
AutoRegManager::abs_cmd_folder(void)
{
	return ExternShell::appRoot+"/"+cmdFolder;
}

std::string 
AutoRegManager::abs_status_folder(void)
{
	return ExternShell::appRoot+"/"+statusFolder;
}

void 
AutoRegManager::new_auto_log_folder(void)
{
	std::string folder;
	std::string str;
	folder = abs_auto_log_folder();
	str = "rm -rf " + folder;
	system(str.c_str());
	str = "mkdir " + folder;
	system(str.c_str());		
}

bool 
AutoRegManager::device_alive(void)
{
	bool alive;
	FileCtrl *fc;
	fc=new FileCtrl("/dev/nvme0n1");
	alive = fc->is_exist();
	delete fc;
	return alive;
}


bool
AutoRegManager::was_reboot(void)
{
	return wasReboot;
}

void 
AutoRegManager::do_reboot(void)
{
	//save file notify reboot
	//std::string file;
	FileCtrl *fc;
	fc=new FileCtrl(abs_status_folder()+"/"+validStatus[STATUS_REBOOT]);
	fc->create();
	sleep(1);

	delete fc;

	std::string cli;
	cli = "gnome-terminal -e \""+ExternShell::appRoot+"/"+AutoRegCfg().value[CFG_SHELL_FOLDER]+"/reboot.sh 5\"";
	system(cli.c_str());
	exit = true;
}

bool 
AutoRegManager::check_reboot(void)
{
	FileCtrl *fc;
	fc=new FileCtrl(abs_status_folder()+"/"+validStatus[STATUS_REBOOT]);
	wasReboot = fc->is_exist();
	if(wasReboot)
	{
		AutoRegCfg *arc;
		printf("status reboot detect!!\n");
		fc->destory();
		arc = new AutoRegCfg();
		//printf("bsf = %s\n", bsf.c_str());
		//store lspci log
		LegacyString *ls;
		ls = new LegacyString();
		ls->sprintf("lspci -s %s -vvv > %s", arc->value[CFG_PCI_BSF].c_str(), lspciLog.c_str());
		system(ls->str().c_str());
		//move shrst to log folder
		std::string from, to;
		from = ExternShell::appRoot+"/"+ExternShell::resultFile;
		to = abs_auto_log_folder()+"/"+ExternShell::resultFile+".init_card";
		ls->sprintf("mv %s %s", from.c_str(), to.c_str());
		//printf("reboot ==> %s\n", ls->str().c_str());
		system(ls->str().c_str());		
		delete ls;
		delete arc;
		//printf("start test fio!!\n");
		//cmd_schedule(CMD_RUN_FIO);
	}
	delete fc;
	return wasReboot;
}

void 
AutoRegManager::cmd_schedule(int i)
{
	if((i>=0)&&(i<CMD_VALID))
	{
		//std::string file;
		FileCtrl *fc;
		fc=new FileCtrl(abs_cmd_folder()+"/"+validCmd[i]);
		fc->create();
		while(!fc->is_exist());	
		delete fc;		
	}
}

void 
AutoRegManager::routine(ShellScript *&script)
{	
	bool refresh=false;
	bool abort=false;
	AutoRegCfg *arc;
	arc = new AutoRegCfg();
	TimeTick nowTick;

	*_tmr >> nowTick;	
	if(nowTick.s!=_lastTick.s)
	{
		_lastTick.s=nowTick.s;
		refresh=true;
	}

	
	if(NULL==script)
	{//idle
		std::string cmd;
		if(cmd_arrived(cmd))
		{//command arrived
			printf("command arrived -- %s\n", cmd.c_str());
			bool removeCmd=true;
			FileCtrl *fc;
			fc = new FileCtrl(abs_cmd_folder()+"/"+cmd);
			//cmd decode
			if(cmd==validCmd[CMD_INIT_CARD])
			{
				script = new InitCardScript(120);
			}
			else
			if(cmd==validCmd[CMD_DO_REBOOT])
			{
				do_reboot();				
			}
			else
			if(cmd==validCmd[CMD_FW_CHECKOUT])
			{
				script = new SVNCheckScript(arc->value[CFG_FW_BIN_FOLDER]);
			}
			else
			if(cmd==validCmd[CMD_LOG_BACK_UP])
			{
				script = new AutoLogBackupScript();
				//other file out auto log, copy to auto log folder
				std::string cmd;
				cmd = "cp "+lspciLog+" ./"+arc->value[CFG_AUTO_LOG_FOLDER]+"/.";				
				system(cmd.c_str());
			}			
			else
			if(cmd==validCmd[CMD_NEW_SUMMARY])
			{
				TNVMeScript(abs_auto_log_folder()).summary_log_prepare(true);
			}
			else
			if(cmd==validCmd[CMD_RUN_FIO])
			{
				script = new FioTestScript(arc->value[CFG_FIO_TEST_TIME], arc->value[CFG_FIO_TEST_SIZE]);
			}
			else
			if(cmd==validCmd[CMD_RUN_TNVME])
			{
				StringList *sl;
				std::string runTNVMeCmdFile;
				std::string grpXYZ;
				runTNVMeCmdFile = abs_cmd_folder()+"/"+validCmd[CMD_RUN_TNVME];				
				sl = new StringList();
				sl->load_from(runTNVMeCmdFile);
				grpXYZ = (*sl)[0];
				if((sl->size()>0)&&(grpXYZ!=""))
				{
					script = new TNVMeScript(abs_auto_log_folder(), grpXYZ);
					sl->erase(sl->begin());
					sl->save_to(runTNVMeCmdFile);
					removeCmd=false;
				}
				else
				{//all finish, backup
					TNVMeScript *tnvme;
					tnvme = new TNVMeScript(abs_auto_log_folder(), "11");
					tnvme->final_summary();
					delete tnvme;
					cmd_schedule(AutoRegManager::CMD_LOG_BACK_UP);
				}
				delete sl;
			}
			else
			if(cmd==validCmd[CMD_RUN_TNVME_ALL])
			{
				StringList *sl;
				std::string runTNVMeCmdFile;
				runTNVMeCmdFile = abs_cmd_folder()+"/"+validCmd[CMD_RUN_TNVME];
				TNVMeScript *tnvme;
				tnvme = new TNVMeScript(abs_auto_log_folder());
				tnvme->summary_log_prepare(true);				
				sl = new StringList();
				int grpCnt = tnvme->group_count();
				for(int grp=0;grp<grpCnt;grp++)
				{
					int testCnt = tnvme->group_test_count(grp);
					for(int test=0;test<testCnt;test++)
					{
						*sl << tnvme->group_sub_test(grp, test);
					}					
				}
				sl->save_to(runTNVMeCmdFile);
				delete sl;
				delete tnvme;
			}			
			else
			if(cmd==validCmd[CMD_RUN_TNVME_GRP])
			{
				StringList *sl;
				std::string runTNVMeCmdFile;
				runTNVMeCmdFile = abs_cmd_folder()+"/"+validCmd[CMD_RUN_TNVME];
				TNVMeScript *tnvme;
				tnvme = new TNVMeScript(abs_auto_log_folder());
				tnvme->summary_log_prepare(true);				
				sl = new StringList();
				int grpCnt = 35;//tnvme->group_count();
				//for(int grp=0;grp<grpCnt;grp++)
				for(int grp=32;grp<grpCnt;grp++)
				{
					int testCnt = tnvme->group_test_count(grp);
					for(int test=0;test<testCnt;test++)
					{
						*sl << tnvme->group_sub_test(grp, test);
					}					
				}
				sl->save_to(runTNVMeCmdFile);
				delete sl;
				delete tnvme;
			}
			else
			if(cmd==validEmergency[EMERGENCY_ABORT])
			{
				abort=true;
			}
			
			if(removeCmd)
			{
				//printf("remove file %s\n", fc->fileName.c_str());
				fc->destory();
			}
			delete fc;					
		}
		else
		{//no any command
			if(refresh)
			{
				std::string dateTime;
				dateTime = FileCtrl::get_date_time();
				printf("\rwait new command : %s", dateTime.c_str());
				fflush(stdout);
			}
		}
	}
	else
	{//running		
		std::string emergency;
		if(emergency_arrived(emergency))
		{//command arrived
			printf("emergency arrived -- %s\n", emergency.c_str());
			//FileCtrl *fc;
			//fc = new FileCtrl(abs_cmd_folder()+"/"+emergency);
			if(emergency==validEmergency[EMERGENCY_ABORT])
			{
				abort=true;
			}
			
			//fc->destory();
			//delete fc;			
		}
		else
		if(refresh)
		{
			printf("\rScript %s running : run/to = %d/%d", script->extern_shell()->absShell.c_str(), script->run_time(), script->timeOut);
			fflush(stdout);
		}		
	}	
	
	if(abort)
	{
		printf("abort task!\n");
		std::string cli;
		cli = "rm -rf "+abs_cmd_folder();
		system(cli.c_str());
		TimerCtrl::delay_ms(1000);
		cli = "mkdir "+abs_cmd_folder();
		system(cli.c_str());
		if(NULL!=script)
		{		
			script->extern_shell()->kill();
		}
	}
	
	delete arc;
		
}

bool 
AutoRegManager::is_idle(void)
{
	std::string dummy;
	return !cmd_arrived(dummy);
}

bool 
AutoRegManager::cmd_arrived(std::string &cmd)
{
	bool arrived=false;
	DIR *dp;
	struct dirent *dirp;
	cmd="";
	dp = opendir(abs_cmd_folder().c_str());
	if(NULL!=dp)
	{
		while(!arrived&&(dirp=readdir(dp))!=NULL)
		{
			cmd = std::string(dirp->d_name);
			if((cmd!=".")&&(cmd!=".."))
			arrived=true;
		}		
		closedir(dp);
	}
	
	return arrived;
}

void 
AutoRegManager::cmd_schedule(std::string cmdStr)
{
	FileCtrl *fc;
	fc=new FileCtrl(abs_cmd_folder()+"/"+cmdStr);
	fc->create();
	while(!fc->is_exist());	
	delete fc;		
}

bool 
AutoRegManager::emergency_arrived(std::string &emergency)
{
	bool arrived=false;
	DIR *dp;
	struct dirent *dirp;
	emergency="";
	dp = opendir(abs_cmd_folder().c_str());
	if(NULL!=dp)
	{
		while(!arrived&&(dirp=readdir(dp))!=NULL)
		{
			emergency = std::string(dirp->d_name);
			for(int i=0;i<EMERGENCY_VALID;i++)
			{
				if(emergency==validEmergency[i])
				{
					arrived = true;
					break;
				}
			}
		}		
		closedir(dp);
	}
	
	return arrived;	
}

AutoRegManager::AutoRegManager(void)
{
	DirectoryCtrl *dc;
	std::string folder;
	dc = new DirectoryCtrl();
	folder = abs_cmd_folder();
	if(!dc->is_exist(folder))
	{
		dc->create(abs_cmd_folder());
	}
	folder = abs_status_folder();
	if(!dc->is_exist(folder))
	{
		dc->create(folder);
	}
	delete dc;
	
	_tmr = new TimerCtrl();
}

AutoRegManager::~AutoRegManager(void)
{
	delete _tmr;
}
