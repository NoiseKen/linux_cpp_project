#ifndef _NVME_TASK_READ_COMPARE_LBA_H_
#define _NVME_TASK_READ_COMPARE_LBA_H_
#include "nvme_task_rc.h"

class NVMeTaskReadCompareLba : public NVMeTaskReadCompare
{
public:
    //from NVMeTaskIoEx
    void golden_pattern_build(IoExPlan *plan);
    NVMeTaskReadCompareLba(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            ,uint32_t qDepth, uint64_t timeStamp, std::string log="", std::string desc="LBA");
    NVMeTaskReadCompareLba(std::string devPath, uint64_t slba, uint64_t nlb
            ,uint32_t qDepth, uint64_t timeStamp, std::string log="", std::string desc="LBA");
    ~NVMeTaskReadCompareLba(void);
protected:
private:
};
#endif
