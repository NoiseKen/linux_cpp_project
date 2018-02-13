#ifndef _AUTO_REG_SERVER_H_
#define _AUTO_REG_SERVER_H_
#include <string>
#include <vector>
#include "timer_ctrl.h"
#include "shell_script.h"

class AutoRegServer
{
public:
	static bool localREG;
	static std::string clientInfoFile;
	static std::string clientStatusFile;
	static std::string clientCmdFlowFinish;
	static std::string clientStopBtnShow;	
	static std::string serverCmdFile;
	static std::string abs_server_folder(void);
	static bool smb_mount_to_server(void);
	static bool ping_server(void);
	bool server_new_cmd_arrive(void);
	void cmd_flow_finish(void);	
	void test_status_update(std::string sts);
	void log_update(std::vector<std::string> &list);
	void routine(ShellScript *script, bool idle);
	AutoRegServer(void);
	~AutoRegServer(void);
protected:
	void client_status_update(std::string sts);
	bool regression_cmd_exist(void);	
	std::string current_result_folder(void);
	std::string current_server_cmd(void);
	void next_server_cmd(void);
	//std::vector<std::string> serverCmd;
private:
	void _local_cmd_destory(void);
	std::string _local_cmd_parsing(std::string key);
	static bool _server_mounted;
	TimerCtrl *_tmr;
	TimeTick _lastTick;	
};

#endif //_FILE_CTRL_H_
