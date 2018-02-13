#ifndef _NVME_SEQ_DUMMY_READ_H_
#define _NVME_SEQ_DUMMY_READ_H_
#include "nvme_ioex_seq.h"

class NVMeSeqDummyRead : public NVMeIoExSeq
{
public:
    //from NVMeTaskIoEx
    //int good_status_after_plan_completion(IoExPlan *plan);
    NVMeSeqDummyRead(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth);
    NVMeSeqDummyRead(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth);
    ~NVMeSeqDummyRead(void);
protected:
private:
};
#endif
