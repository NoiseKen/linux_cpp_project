#ifndef _NVME_TASK_READ_COMPARE_DECREASE_H_
#define _NVME_TASK_READ_COMPARE_DECREASE_H_
#include "nvme_task_rc.h"

class NVMeTaskReadCompareDecrease : public NVMeTaskReadCompare
{
public:
    uint64_t const baseLba;
    uint32_t const dataInterval;
    uint32_t const dataPattern;
    //from NVMeTaskIoEx
    void golden_pattern_build(IoExPlan *plan);
    NVMeTaskReadCompareDecrease(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Decrease");
    NVMeTaskReadCompareDecrease(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Decrease");
    ~NVMeTaskReadCompareDecrease(void);
protected:
private:
};
#endif
