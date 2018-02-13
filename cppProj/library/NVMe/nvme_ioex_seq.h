#ifndef _NVME_IOEX_SEQ_H_
#define _NVME_IOEX_SEQ_H_

//------------------> from driver
#include "nvme_ioex.h"
//<------------------ from driver

class NVMeIoExSeq : public NVMeIoEx
{
public:
    NVMeIoExSeq(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth
        , uint64_t timeStamp, std::string log);
    NVMeIoExSeq(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth
        , uint64_t timeStamp, std::string log);
    virtual ~NVMeIoExSeq(void);
protected:
    //virtual function overdrive NVMeTaskIoEx
    bool next_plan(IoExPlan *plan);
    int caculate_progress(void);
private:
    virtual IoExPlan* __new_ioex_plan_in_scheduler(uint32_t allocSize, uint32_t id);
    uint64_t const __totalNlb;
    uint64_t __currentLBA;
    uint64_t __remainedNlb;
};

#endif //ndef _NVME_IOEX_SEQ_H_
