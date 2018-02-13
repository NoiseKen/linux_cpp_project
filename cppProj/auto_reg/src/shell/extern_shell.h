#ifndef _EXTERN_SHELL_H_
#define _EXTERN_SHELL_H_
#include <string>

class ExternShell
{
public:
	static std::string appRoot;
	static std::string const runningFile;
	static std::string const resultFile;
	static std::string morph_result_file(std::string extend);
	std::string const absShell;
	std::string const currentStatus;
	//parameter in
	class ExternShell &operator <<(std::string in);
	int exec(void);
	bool is_running(void);
	int result_num(void);
	std::string result_str(void);
	void kill(void);	
	ExternShell(std::string shell, std::string stat="None");
	virtual ~ExternShell(void);
protected:
	std::string parameter;
private:
};

#endif //_FILE_CTRL_H_
