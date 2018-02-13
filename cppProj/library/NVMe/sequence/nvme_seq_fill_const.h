#ifndef _NVME_SEQ_FILL_CONST_H_
#define _NVME_SEQ_FILL_CONST_H_
#include "nvme_ioex_seq.h"


class NVMeSeqFillConst : public NVMeIoExSeq
{
public:
    uint32_t const dataPattern;
    //from NVMeTaskIoEx
    bool new_plan_before_issue(IoExPlan *plan);
    NVMeSeqFillConst(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp);
    NVMeSeqFillConst(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp);
    ~NVMeSeqFillConst(void);
protected:
private:
};
#endif
