#ifndef _NVME_SEQ_READ_COMPARE_DECREASE_H_
#define _NVME_SEQ_READ_COMPARE_DECREASE_H_
#include "nvme_seq_rc.h"

class NVMeSeqReadCompareDecrease : public NVMeSeqReadCompare
{
public:
    uint64_t const baseLba;
    uint32_t const dataInterval;
    uint32_t const dataPattern;
    //from NVMeTaskIoEx
    void golden_pattern_build(IoExPlan *plan);
    NVMeSeqReadCompareDecrease(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Decrease");
    NVMeSeqReadCompareDecrease(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Decrease");
    ~NVMeSeqReadCompareDecrease(void);
protected:
private:
};
#endif
