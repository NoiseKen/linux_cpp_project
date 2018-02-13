#ifndef _NVME_TASK_DUMMY_READ_H_
#define _NVME_TASK_DUMMY_READ_H_
#include "nvme_ioex.h"

class NVMeTaskDummyRead : public NVMeIoEx
{
public:
    //from NVMeTaskIoEx
    //int good_status_after_plan_completion(IoExPlan *plan);
    //void io_ex_post(void);
    NVMeTaskDummyRead(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth);
    NVMeTaskDummyRead(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth);
    ~NVMeTaskDummyRead(void);
protected:
private:
};
#endif
