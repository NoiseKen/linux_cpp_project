#ifndef _NVME_SEQ_READ_COMPARE_CONST_H_
#define _NVME_SEQ_READ_COMPARE_CONST_H_
#include "nvme_seq_rc.h"

class NVMeSeqReadCompareConst : public NVMeSeqReadCompare
{
public:
    uint32_t const dataPattern;
    //from NVMeTaskReadCompare
    void golden_pattern_build(IoExPlan *plan);
    NVMeSeqReadCompareConst(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Constant");
    NVMeSeqReadCompareConst(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Constant");
    ~NVMeSeqReadCompareConst(void);
protected:
private:
};
#endif
