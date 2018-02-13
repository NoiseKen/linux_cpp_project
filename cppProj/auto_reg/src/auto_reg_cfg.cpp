#include "auto_reg_cfg.h"
#include "extern_shell.h"
#include "value_list.h"

std::string const AutoRegCfg::cfgFile="cfg/auto_reg.cfg";

std::string const AutoRegCfg::cfgKey[CFG_MAX]={
	"PCI_BSF",
	"SVN_REPO",
	"SHELL_FOLDER",
	"AUTO_LOG_FOLDER",
	"AUTO_LOG_COMPRESS",
	"FW_BIN_FOLDER",
	"MP_TOOLS_ROOT",
	"TNVME_TOOLS_ROOT",
	"FIO_TEST_TIME",
	"FIO_TEST_SIZE",
	"SERVER_IP",
	"UBUNTU_SESSION",
	"PROJECT_CLIENT",
	"CLIENT_INFO",
};

std::string 
AutoRegCfg::_f_get_value(int i)
{
	std::string val="";
	if(i<CFG_MAX)
	{
		ValueList *vl;
		vl = new ValueList("=");
		val = vl->get_config_value(abs_cfg_file(), cfgKey[i]);
		delete vl;
	}
	return val;
}

std::string 
AutoRegCfg::abs_cfg_file(void)
{
	return ExternShell::appRoot+"/"+cfgFile;
}

AutoRegCfg::AutoRegCfg(void) : value(this, &AutoRegCfg::_f_get_value)
{
}

AutoRegCfg::~AutoRegCfg(void)
{
}
