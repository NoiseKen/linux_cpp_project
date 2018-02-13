#ifndef _NVME_SEQ_FILL_INCREASE_H_
#define _NVME_SEQ_FILL_INCREASE_H_
#include "nvme_ioex_seq.h"

class NVMeSeqFillIncrease : public NVMeIoExSeq
{
public:
    uint64_t const baseLba;
    uint32_t const dataInterval;
    uint32_t const dataPattern;
    //from NVMeTaskIoEx
    bool new_plan_before_issue(IoExPlan *plan);
    NVMeSeqFillIncrease(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp);
    NVMeSeqFillIncrease(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp);
    ~NVMeSeqFillIncrease(void);
protected:
private:
};
#endif
