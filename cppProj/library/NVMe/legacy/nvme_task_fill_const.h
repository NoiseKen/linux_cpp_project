#ifndef _NVME_TASK_FILL_CONST_H_
#define _NVME_TASK_FILL_CONST_H_
#include "nvme_ioex.h"


class NVMeTaskFillConst : public NVMeIoEx
{
public:
    uint32_t const dataPattern;
    //from NVMeTaskIoEx
    bool new_plan_before_issue(IoExPlan *plan);
    void pre_fill_after_issue(IoExPlan *plan);    
    //void io_ex_post(void);
    NVMeTaskFillConst(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp);
    NVMeTaskFillConst(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp);
    ~NVMeTaskFillConst(void);
protected:
private:
};
#endif
