#ifndef _IOEX_COMPARE_H_
#define _IOEX_COMPARE_H_
#include "global_variable.h"
#include "ioex_cmd.h"
class IoExCompare : public IoExCmd
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(uint8_t nthDev);
    void help_ioex_example(void);
    IoExCompare(int argc, char* argv[]);
    virtual ~IoExCompare(void);
protected:
    uint64_t _slba;
    uint64_t _nlb;
    uint32_t _nsid;
    uint32_t _qd;
    uint32_t _type;
    uint32_t _pattern;
    uint32_t _interval;
    uint64_t _timeStamp;
    std::string _log;
private:
    enum{
        IOEX_CMP_SLBA=0,
        IOEX_CMP_NLB,
        IOEX_CMP_NSID,
        IOEX_CMP_QD,
        IOEX_CMP_EXBUF,
        IOEX_CMP_SEQ,
        IOEX_CMP_BACK_GROUND,
        IOEX_CMP_PATTERN_TYPE,
        IOEX_CMP_PATTERN,
        IOEX_CMP_INTERVAL,
        IOEX_CMP_TIME_STAMP,
        IOEX_CMP_LOG,
        IOEX_CMP_RAW_DEV,
        IOEX_CMP_HELP
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _GLOBAL_VARIABLE_H_
