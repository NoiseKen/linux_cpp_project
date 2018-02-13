#ifndef _IOEX_FILL_H_
#define _IOEX_FILL_H_
#include "global_variable.h"
#include "ioex_cmd.h"
class IoExFill : public IoExCmd
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(uint8_t nthDev);
    void help_ioex_example(void);
    IoExFill(int argc, char* argv[]);
    virtual ~IoExFill(void);
protected:
    uint64_t _slba;
    uint64_t _nlb;
    uint32_t _nsid;
    uint32_t _qd;
    uint32_t _type;
    uint32_t _pattern;
    uint32_t _interval;
    uint64_t _timeStamp;
private:
    enum{
        IOEX_FILL_SLBA=0,
        IOEX_FILL_NLB,
        IOEX_FILL_NSID,
        IOEX_FILL_QD,
        IOEX_FILL_EXBUF,
        IOEX_FILL_SEQ,
        IOEX_FILL_BACK_GROUND,
        IOEX_FILL_PATTERN_TYPE,
        IOEX_FILL_PATTERN,
        IOEX_FILL_INTERVAL,
        IOEX_FILL_TIME_STAMP,
        IOEX_FILL_RAW_DEV,
        IOEX_FILL_HELP
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _GLOBAL_VARIABLE_H_
