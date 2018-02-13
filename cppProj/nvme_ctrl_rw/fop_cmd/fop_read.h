#ifndef _FOP_CMD_READ_H_
#define _FOP_CMD_READ_H_
#include "global_variable.h"
#include "fop_cmd.h"
class FOPCmdRead : public FOPCmd
{
public:
    bool is_help(void);
    bool is_valid(void);
    void post(void);
    int issue(uint8_t nthDev);
    void help_fop_example(void);
    FOPCmdRead(int argc, char* argv[]);
    virtual ~FOPCmdRead(void);
protected:
    void *readBuffer;
private:
    enum{
        FOP_READ_SLBA=0,
        FOP_READ_NLB,
        FOP_READ_FILE,
        FOP_READ_RAW_DEV,
        FOP_READ_HELP
    };
    static GlobalVariable _gVars[];
    uint64_t __slba;
    uint32_t __nlb;
};
#endif //ndef _GLOBAL_VARIABLE_H_
