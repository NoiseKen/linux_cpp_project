#ifndef _NVME_TASK_FILL_LBA_H_
#define _NVME_TASK_FILL_LBA_H_
#include "nvme_ioex.h"

class NVMeTaskFillLba : public NVMeIoEx
{
public:
    //from BasicTask
//    void issue(void);
    //from NVMeTaskIoEx
    bool new_plan_before_issue(IoExPlan *plan);
    void pre_fill_after_issue(IoExPlan *plan);    
    //void io_ex_post(void);
    NVMeTaskFillLba(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth, uint64_t timeStamp);
    NVMeTaskFillLba(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth, uint64_t timeStamp);
    ~NVMeTaskFillLba(void);
protected:
private:
};
#endif
