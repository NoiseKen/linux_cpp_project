#ifndef _AUTO_REG_CFG_H_
#define _AUTO_REG_CFG_H_
#include <string>
#include "property.h"

enum{
	CFG_PCI_BSF=0,
	CFG_SVN_REPO,
	CFG_SHELL_FOLDER,
	CFG_AUTO_LOG_FOLDER,
	CFG_AUTO_LOG_COMPRESS_FILE,
	CFG_FW_BIN_FOLDER,
	CFG_MPTOOL_ROOT,
	CFG_TNVME_ROOT,
	CFG_FIO_TEST_TIME,
	CFG_FIO_TEST_SIZE,
	CFG_SERVER_IP,
	CFG_UBUNTU_SESSION,
	CFG_PROJECT_CLIENT,
	CFG_CLIENT_INFO,
	CFG_MAX
};

class AutoRegCfg
{
public:
	static std::string const cfgKey[CFG_MAX];
	static std::string const cfgFile;
	properties_ro<AutoRegCfg, std::string> value;
	AutoRegCfg(void);
	~AutoRegCfg(void);
protected:	
	std::string abs_cfg_file(void);
private:
	std::string _f_get_value(int i);
};

#endif //_FILE_CTRL_H_
