#ifndef _NVME_SEQ_FILL_LBA_H_
#define _NVME_SEQ_FILL_LBA_H_
#include "nvme_ioex_seq.h"

class NVMeSeqFillLba : public NVMeIoExSeq
{
public:
    //from BasicTask
//    void issue(void);
    //from NVMeTaskIoEx
    bool new_plan_before_issue(IoExPlan *plan);
    //void io_ex_post(void);
    NVMeSeqFillLba(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth, uint64_t timeStamp);
    NVMeSeqFillLba(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth, uint64_t timeStamp);
    ~NVMeSeqFillLba(void);
protected:
private:
};
#endif
