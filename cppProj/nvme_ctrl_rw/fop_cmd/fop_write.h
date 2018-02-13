#ifndef _FOP_CMD_WRITE_H_
#define _FOP_CMD_WRITE_H_
#include "global_variable.h"
#include "fop_cmd.h"
class FOPCmdWrite : public FOPCmd
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(uint8_t nthDev);
    void help_fop_example(void);
    FOPCmdWrite(int argc, char* argv[]);
    virtual ~FOPCmdWrite(void);
protected:
private:
    enum{
        FOP_WRITE_SLBA=0,
        FOP_WRITE_NLB,
        FOP_WRITE_FILE,
        FOP_WRITE_RAW_DEV,
        FOP_WRITE_HELP
    };
    static GlobalVariable _gVars[];
    uint64_t __slba;
    uint32_t __nlb;
};
#endif //ndef _GLOBAL_VARIABLE_H_
