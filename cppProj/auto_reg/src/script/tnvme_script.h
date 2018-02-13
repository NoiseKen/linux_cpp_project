#ifndef _TNVME_SCRIPT_H_
#define _TNVME_SCRIPT_H_
#include "tnvme_shell.h"
#include "shell_script.h"


class TNVMeScript : public ShellScript
{
public:
	static std::string const grpXYZCfgFile;
	static std::string const summaryFile;
		
	static int group_count(void);
	//start is start line in grpXYZCfgFile
	static int group_test_count(int grp, int *start=NULL);
	static std::string group_sub_test(int grp, int n);
	void final_summary(void);
	void summary_log_prepare(bool start);
	TNVMeScript(std::string logFolder, std::string grpXYZ="");
	~TNVMeScript(void);
protected:
	std::string const summaryFolder;
	void shell_monitor(int sc);
private:
	static TNVMeShell *_new_shell(std::string test);
};

#endif //_FILE_CTRL_H_
