#ifndef _IOEX_CMD_H_
#define _IOEX_CMD_H_
#include "console_arguments.h"
#include "property.h"
#include "nvme_ioex.h"
#include "nvme.h"
class IoExCmd : public ConsoleArguments
{
public:
    std::string const cmdString;
    virtual bool is_help(void)=0;
    virtual int issue(uint8_t nthDev)=0;
    virtual void post(void){};
    virtual bool is_valid(void)=0;
    virtual void help_ioex_example(void){};
    property_ro<IoExCmd, uint64_t> lastFailLba;
    IoExCmd(int argc, char* argv[], std::string cmdStr, GlobalVariable *const pVars, int varsSize);
    virtual ~IoExCmd(void);
protected:
    //void valid_io_in_file_name(int idx, bool &valid, bool acceptEmpty=true);
    //AnsiString valid_io_out_file_name(int idx, DWORD requiredByteCount, bool &valid);
    //bool post_ds_to_file(int idx, DWORD byteCount, void *buf);
    int _task_routine(NVMeIoEx *task);
    uint64_t _lastFailLba;
    enum{
        typeConst=0, typeInc, typeDec, typeLba,
    };
    uint8_t _type;
private:
    uint64_t __f_get_last_fail_lba(void) {return _lastFailLba;};
};
#endif //ndef _GLOBAL_VARIABLE_H_
