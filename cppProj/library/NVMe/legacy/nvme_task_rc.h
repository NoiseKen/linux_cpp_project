#ifndef _NVME_TASK_READ_COMPARE_H_
#define _NVME_TASK_READ_COMPARE_H_
#include "nvme.h"
#include "nvme_ioex.h"
#include "memory.h"

class NVMeTaskReadCompare : public NVMeIoEx
{
public:
    virtual void golden_pattern_build(IoExPlan *plan)=0; 
    //from NVMeTaskIoEx
    int good_status_after_plan_completion(IoExPlan *plan, uint32_t &errOffset);
    //void io_ex_post(void);
    NVMeTaskReadCompare(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc);
    NVMeTaskReadCompare(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc);
    virtual ~NVMeTaskReadCompare(void);
protected:
    Memory<uint32_t> *golden;
private:
    virtual IoExPlan* __new_ioex_plan_in_scheduler(uint32_t allocSize, uint32_t id);
};
#endif
