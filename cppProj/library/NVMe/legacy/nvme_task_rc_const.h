#ifndef _NVME_TASK_READ_COMPARE_CONST_H_
#define _NVME_TASK_READ_COMPARE_CONST_H_
#include "nvme_task_rc.h"

class NVMeTaskReadCompareConst : public NVMeTaskReadCompare
{
public:
    uint32_t const dataPattern;
    //from NVMeTaskReadCompare
    void golden_pattern_build(IoExPlan *plan);
    NVMeTaskReadCompareConst(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Constant");
    NVMeTaskReadCompareConst(std::string devPath, uint64_t slba, uint64_t nlb
            , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp
            , std::string log="", std::string desc="Constant");
    ~NVMeTaskReadCompareConst(void);
protected:
private:
};
#endif
