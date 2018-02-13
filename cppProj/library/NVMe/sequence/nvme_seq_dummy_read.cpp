#include "nvme_seq_dummy_read.h"
#include "legacy_string.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NVMeSeqDummyRead::NVMeSeqDummyRead(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth)
    : NVMeIoExSeq(nthDev, nsid, slba, nlb, qDepth, 0, "")
{
    rep << LegacyString().sprintf("Sequential Dummy Read : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeSeqDummyRead::NVMeSeqDummyRead(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth)
    : NVMeIoExSeq(devPath, slba, nlb, qDepth, 0, "")
{
    rep << LegacyString().sprintf("Sequential Dummy Read : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeSeqDummyRead::~NVMeSeqDummyRead(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
