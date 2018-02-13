#ifndef _NVME_TASK_FILL_DECREASE_H_
#define _NVME_TASK_FILL_DECREASE_H_
#include "nvme_ioex.h"

class NVMeTaskFillDecrease : public NVMeIoEx
{
public:
    uint64_t const baseLba;
    uint32_t const dataInterval;
    uint32_t const dataPattern;
    //from NVMeTaskIoEx
    bool new_plan_before_issue(IoExPlan *plan);
    void pre_fill_after_issue(IoExPlan *plan);
    NVMeTaskFillDecrease(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp);
    NVMeTaskFillDecrease(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp);
    ~NVMeTaskFillDecrease(void);
protected:
private:
};
#endif
