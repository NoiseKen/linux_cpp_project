#ifndef _FOP_CMD_H_
#define _FOP_CMD_H_
#include <malloc.h>
#include <string.h>
#include "console_arguments.h"
#include "disk_ioex.h"

class FOPCmd : public ConsoleArguments
{
public:
    std::string const cmdString;
    virtual bool is_help(void)=0;
    virtual int issue(uint8_t nthDev)=0;
    virtual void post(void){}
    virtual bool is_valid(void)=0;
    virtual void help_fop_example(void){};
    //__property NVMeCmd *nvmeCmd={read=fGetNVMeCmd};
    FOPCmd(int argc, char* argv[], std::string cmdStr, GlobalVariable *const pVars, int varsSize);
    virtual ~FOPCmd(void);
protected:
    bool post_ds_to_file(int idx, uint32_t byteCount, void *buf);
private:
};
#endif //ndef _GLOBAL_VARIABLE_H_
