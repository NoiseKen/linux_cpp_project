#ifndef _NVME_SEQ_READ_COMPARE_H_
#define _NVME_SEQ_READ_COMPARE_H_

//------------------> from driver
#include "nvme.h"
#include "nvme_ioex_seq.h"
#include "memory.h"
//<------------------ from driver

class NVMeSeqReadCompare : public NVMeIoExSeq
{
public:
    virtual void golden_pattern_build(IoExPlan *plan)=0;
    //from NVMeTaskIoEx
    int good_status_after_plan_completion(IoExPlan *plan, uint32_t &errOffset);
    NVMeSeqReadCompare(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc);
    NVMeSeqReadCompare(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc);
    virtual ~NVMeSeqReadCompare(void);
protected:
    Memory<uint32_t> *golden;
private:
};

#endif //ndef _NVME_TASK_IOEX_ATOM_WRITE_H_
