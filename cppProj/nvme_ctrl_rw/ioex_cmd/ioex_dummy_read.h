#ifndef _IOEX_DUMMY_READ_H_
#define _IOEX_DUMMY_READ_H_
#include "global_variable.h"
#include "ioex_cmd.h"
class IoExDummyRead : public IoExCmd
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(uint8_t nthDev);
    void help_ioex_example(void);
    IoExDummyRead(int argc, char* argv[]);
    virtual ~IoExDummyRead(void);
protected:
    uint64_t _slba;
    uint64_t _nlb;
    uint32_t _nsid;
    uint32_t _qd;
private:
    enum{
        IOEX_DUMMY_READ_SLBA=0,
        IOEX_DUMMY_READ_NLB,
        IOEX_DUMMY_READ_NSID,
        IOEX_DUMMY_READ_QD,
        IOEX_DUMMY_READ_EXBUF,
        IOEX_DUMMY_READ_SEQ,
        IOEX_DUMMY_READ_RAW_DEV,
        IOEX_DUMMY_READ_HELP
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _GLOBAL_VARIABLE_H_
