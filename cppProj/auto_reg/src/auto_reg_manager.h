#ifndef _AUTO_REG_MANAGER_H_
#define _AUTO_REG_MANAGER_H_
#include <string>
#include "timer_ctrl.h"
#include "shell_script.h"

class AutoRegManager
{
public:
	enum{		
		CMD_INIT_CARD=0,	//init card
		CMD_DO_REBOOT,
		CMD_FW_CHECKOUT,
		CMD_NEW_SUMMARY,
		CMD_RUN_FIO,
		CMD_RUN_TNVME,		//run tnvme
		CMD_RUN_TNVME_ALL,	//run_tnvme_all
		CMD_LOG_BACK_UP,
		CMD_RUN_TNVME_GRP,	//run_tnvme_grp
		CMD_VALID
	};
	enum{
		EMERGENCY_ABORT,
		EMERGENCY_VALID
	};
	enum{
		STATUS_REBOOT=0,	//status confirm reboot already (card initialized)
		STATUS_VALID
	};
	static std::string const validCmd[CMD_VALID];
	static std::string const validEmergency[EMERGENCY_VALID];
	static std::string const validStatus[STATUS_VALID];
	static std::string const lspciLog;
	static std::string const cmdFolder;
	static std::string const statusFolder;
	static bool device_alive(void);
	static bool was_reboot(void);
	static void cmd_schedule(int i);
	static void cmd_schedule(std::string cmdStr);
	static bool exit;
	//static void purge_all_cmd(void);
	void new_auto_log_folder(void);
	std::string abs_auto_log_folder(void);
	bool is_idle(void);
	void do_reboot(void);
	bool check_reboot(void);
	void routine(ShellScript *&script);
	AutoRegManager(void);
	~AutoRegManager(void);
protected:
	static bool wasReboot;	
	static std::string abs_cmd_folder(void);
	static std::string abs_status_folder(void);	
	bool cmd_arrived(std::string &cmd);
	bool emergency_arrived(std::string &emergency);
private:
	TimerCtrl *_tmr;
	TimeTick _lastTick;	
};

#endif //_FILE_CTRL_H_
