#include "auto_reg_server.h"
#include "auto_reg_cfg.h"
#include "log_file.h"
#include "network_app.h"
#include "value_list.h"
#include "legacy_string.h"
#include "auto_reg_manager.h"

bool AutoRegServer::localREG=false;
bool AutoRegServer::_server_mounted=false;
std::string AutoRegServer::clientInfoFile="client_info.txt";
std::string AutoRegServer::clientStatusFile="client_status.txt";
std::string AutoRegServer::clientCmdFlowFinish="cmdflow_finished.txt";
std::string AutoRegServer::clientStopBtnShow="ctrlbtn_show.txt";
std::string AutoRegServer::serverCmdFile="cmd.ini";

bool 
AutoRegServer::smb_mount_to_server(void)
{
	bool linked;	
	FileCtrl *fc;	
	fc = new FileCtrl(abs_server_folder());
	linked = fc->is_exist();
	if(!linked)
	{
		std::string cli;
		AutoRegCfg *arc;	
		arc = new AutoRegCfg();
		cli = "gvfs-mount smb://"+arc->value[CFG_SERVER_IP]
				+ "/TestResults/" + arc->value[CFG_PROJECT_CLIENT];
		printf("cli=%s\n", cli.c_str());
		system(cli.c_str());
		delete arc;
		TimerCtrl::delay_ms(500);
		linked = fc->is_exist();
	}	
	delete fc;
	return linked;
}

std::string 
AutoRegServer::abs_server_folder(void)
{
	std::string folder;
	AutoRegCfg *arc;
	arc = new AutoRegCfg();
	folder = "/run/"+arc->value[CFG_UBUNTU_SESSION]+"/0/gvfs"
		+"/smb-share:server="+arc->value[CFG_SERVER_IP]
		+",share=testresults/"+arc->value[CFG_PROJECT_CLIENT];
	delete arc;
	return folder;
}

bool 
AutoRegServer::ping_server(void)
{
	bool status;
	AutoRegCfg *arc;
	arc = new AutoRegCfg();
	status = NetworkApp::ping(arc->value[CFG_SERVER_IP]);
	delete arc;
	return status;
}

void 
AutoRegServer::log_update(std::vector<std::string> &list)
{
	//build log folder
	std::string path;
	std::string cli;
	
	path = current_result_folder()+"/"+current_server_cmd()+"/Log";
	FileCtrl *fc;
	fc = new FileCtrl(path);		
	if(!fc->is_exist())
	{
		cli = "mkdir \""+path+"\"";
		//printf("log update cli=%s\n", cli.c_str());
		//getchar();
		system(cli.c_str());
	}
	delete fc;

	int cnt=list.size();
	for(int i=0;i<cnt;i++)
	{
		//cli = "cp ./"+list[i]+" "+path+"/"+list[i];
		cli = "cp ./"+list[i]+" \""+path+"/"+list[i]+"\"";
		//printf("cli=%s\n", cli.c_str());		
		system(cli.c_str());
	}
}

bool 
AutoRegServer::server_new_cmd_arrive(void)
{
	bool arrived;
	std::string server;
	std::string local;
	FileCtrl *fc;
	server = abs_server_folder()+"/TestCmd/"+serverCmdFile;
	local = ExternShell::appRoot+"/"+serverCmdFile;
	fc = new FileCtrl(server);
	arrived = fc->is_exist();
	delete fc;
	if(arrived)
	{//copy to local path	
		std::string cli;	
		cli="mv "+server+" "+local;
		system(cli.c_str());
		//add show btn option
		server = abs_server_folder()+"/"+clientStopBtnShow;
		fc = new FileCtrl(server);
		fc->create();
		delete fc;
		
	}
	
	return arrived;
}

void 
AutoRegServer::routine(ShellScript *script, bool idle)
{	
	bool testServerReady=false;
	AutoRegCfg *arc;
	arc = new AutoRegCfg();
	TimeTick nowTick;

	*_tmr >> nowTick;	
	if(nowTick.s!=_lastTick.s)
	{
		_lastTick.s=nowTick.s;
		testServerReady=true;
	}	
	
	if(testServerReady)
	{
		if(!_server_mounted)
		{
			if(ping_server())
			{		
				_server_mounted = smb_mount_to_server();
				printf("Detected Server [%s], mount smb -- %s\n", arc->value[CFG_SERVER_IP].c_str()
					, _server_mounted?"YES":"NO");
			
				if(_server_mounted)
				{
					LogFile *lf;
					lf = new LogFile(abs_server_folder()+"/"+clientInfoFile);
					lf->destory();
					lf->create();
					*lf << arc->value[CFG_CLIENT_INFO];					
					delete lf;
				}
			}
		}
		else
		{
			if(!ping_server())
			{			
				printf("Server [%s] Lose Link !!\n", arc->value[CFG_SERVER_IP].c_str());			
			}
			else
			{//Normal routine
				//check new command
				if(server_new_cmd_arrive())
				{
					printf("Server new Command arrived!!\n");
					//Parsing new command
					std::string cmd;
					cmd = _local_cmd_parsing("Cmd");
					if(cmd=="Stop")
					{
						AutoRegManager().cmd_schedule(AutoRegManager::validEmergency[AutoRegManager::EMERGENCY_ABORT]);
						printf("abort AutoREG!!\n");
						_local_cmd_destory();					
					}
				}		
		
				if(NULL==script)
				{//local cmd.ini execute
					//printf("idle=%s\n", (idle)?"YES":"NO");
					if(idle && regression_cmd_exist())
					{
						printf("run_regression\n");
						next_server_cmd();
						if(regression_cmd_exist())
						{
							std::string cmd;
							cmd = current_server_cmd();
							printf("Current Command=%s\n", cmd.c_str());
							//build cmd folder in server
							std::string cli;
							cli = "mkdir \"" + current_result_folder()+"/"+current_server_cmd()+"\"";
							system(cli.c_str());
						
							if(cmd=="FIO")
							{
								AutoRegManager::cmd_schedule(AutoRegManager::CMD_RUN_FIO);
							}
							else
							if(cmd=="TNVMe")
							{
								//printf("Server issue TNVME\n");
								//AutoRegManager::cmd_schedule(AutoRegManager::CMD_RUN_TNVME_GRP);
								AutoRegManager::cmd_schedule(AutoRegManager::CMD_RUN_TNVME_ALL);
							}
							else
							if(cmd=="Download FW from SVN")
							{
								//AutoRegManager::cmd_schedule(AutoRegManager::CMD_INIT_CARD);
								AutoRegManager::cmd_schedule(AutoRegManager::CMD_FW_CHECKOUT);
							}						
						}
					}
				}
				else			
				{					
					ExternShell *shell;
					shell = script->extern_shell();				
					{
						LegacyString *ls;
						ls = new LegacyString();
						ls->sprintf("%s, run time=%d", shell->currentStatus.c_str(), script->run_time());
						client_status_update(shell->absShell);
						test_status_update(ls->str());
						delete ls;
					}				
				}			
			}		
		}
	}
	delete arc;	
}

bool 
AutoRegServer::regression_cmd_exist(void)
{
	bool exist;
	std::string local;
	FileCtrl *fc;
	local = ExternShell::appRoot+"/"+serverCmdFile;
	fc = new FileCtrl(local);
	exist = fc->is_exist();
	delete fc;
	return exist;	
}

void 
AutoRegServer::client_status_update(std::string sts)
{
	StringList *sl;
	LegacyString *ls;					
	sl = new StringList(true);										
	ls = new LegacyString();
	//client status
	ls->sprintf("Status=%s", sts.c_str());
	*sl << ls->str();
	ls->sprintf("Msg=%s", FileCtrl::get_date_time().c_str());
	*sl << ls->str();
	sl->save_to(abs_server_folder()+"/"+clientStatusFile);
	delete ls;
	delete sl;	
}

void 
AutoRegServer::test_status_update(std::string sts)
{
	StringList *sl;
	sl = new StringList(true);										
	*sl << sts;
	sl->save_to(current_result_folder()+"/"+current_server_cmd()+"/Status.txt");	
	delete sl;
}

std::string 
AutoRegServer::current_server_cmd(void)
{
	LegacyString *ls;
	int nth;
	std::string val;
	ls = new LegacyString();
	val = _local_cmd_parsing("Cmd");
	nth = atoi(val.c_str());
	ls->sprintf("Cmd%02d", nth);
	val = _local_cmd_parsing(ls->str());
	delete ls;
	return val;
}

std::string 
AutoRegServer::current_result_folder(void)
{
	std::string folder;
	folder = abs_server_folder()+"/TestCmd/"+_local_cmd_parsing("DefDir");
	return folder;
}

void 
AutoRegServer::cmd_flow_finish(void)
{
	FileCtrl *fc;
	fc = new FileCtrl(abs_server_folder()+"/"+clientCmdFlowFinish);
	fc->create();
	delete fc;	
	
	_local_cmd_destory();	
}

void
AutoRegServer::next_server_cmd(void)
{
	std::string val;
	int nth;
	int nTotal;
	ValueList *vl;
	LegacyString *ls;
	ls = new LegacyString();
	vl = new ValueList("=", true);
	vl->load_from(ExternShell::appRoot+"/"+serverCmdFile);
	val = vl->get_value("CmdNum");
	nTotal = atoi(val.c_str());
	val = vl->get_value("Cmd");	
	nth = (val=="Start")?-1:atoi(val.c_str());
	nth++;	
	if(nth<nTotal)
	{
		ls->sprintf("%02d", nth);
		vl->set_value("Cmd", ls->str());
		vl->save_to(ExternShell::appRoot+"/"+serverCmdFile);
	}
	else
	{//all command finish, delete cmd.ini
		cmd_flow_finish();
		//_local_cmd_destory();
		//FileCtrl *fc;
		//fc = new FileCtrl(abs_server_folder()+"/"+clientCmdFlowFinish);
		//fc->create();
		//delete fc;		
	}
	delete ls;
	delete vl;
}

void 
AutoRegServer::_local_cmd_destory(void)
{
	FileCtrl *fc;
	fc = new FileCtrl(ExternShell::appRoot+"/"+serverCmdFile);
	fc->destory();
	while(fc->is_exist());
	delete fc;			
}

std::string 
AutoRegServer::_local_cmd_parsing(std::string key)
{
	std::string val;
	ValueList *vl;
	std::string cmd;
	vl = new ValueList("=", true);
	vl->load_from(ExternShell::appRoot+"/"+serverCmdFile);
	val = vl->get_value(key);
	delete vl;
	return val;
}

AutoRegServer::AutoRegServer(void)
{
	_tmr = new TimerCtrl();	
}

AutoRegServer::~AutoRegServer(void)
{
	delete _tmr;
}
