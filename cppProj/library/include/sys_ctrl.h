#ifndef _SYS_CTRL_H_
#define _SYS_CTRL_H_

#include <linux/limits.h>
#include <string>

class SysCtrl
{
public:
    static int string_to_int32(std::string str, bool &valid);
    static uint64_t string_to_int64(std::string str, bool &valid);
    static std::string string_rtrim(std::string str, std::string trimStr);
    std::string stdout(void) {return _stdout;};
    int exec_cmd(std::string cmd);
	SysCtrl(void);
	~SysCtrl(void);
protected:
    std::string _stdout;
private:
	
};

#endif //_SYS_CTRL_H_
