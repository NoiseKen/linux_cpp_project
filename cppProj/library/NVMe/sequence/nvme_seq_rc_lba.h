#ifndef _NVME_SEQ_READ_COMPARE_LBA_H_
#define _NVME_SEQ_READ_COMPARE_LBA_H_
#include "nvme_seq_rc.h"

class NVMeSeqReadCompareLba : public NVMeSeqReadCompare
{
public:
    //from NVMeTaskIoEx
    void golden_pattern_build(IoExPlan *plan);
    NVMeSeqReadCompareLba(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
            ,uint32_t qDepth, uint64_t timeStamp, std::string log="", std::string desc="LBA");
    NVMeSeqReadCompareLba(std::string devPath, uint64_t slba, uint64_t nlb
            ,uint32_t qDepth, uint64_t timeStamp, std::string log="", std::string desc="LBA");
    ~NVMeSeqReadCompareLba(void);
protected:
private:
};
#endif
