#include "nvme_task_dummy_read.h"
#include "legacy_string.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//void
//NVMeTaskDummyRead::io_ex_post(void)
//{
//}
//---------------------------------------------------------------------------
NVMeTaskDummyRead::NVMeTaskDummyRead(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth)
    : NVMeIoEx(nthDev, nsid, slba, nlb, qDepth, 0, "")
{
    rep << LegacyString().sprintf("Dummy Read : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NVMeTaskDummyRead::NVMeTaskDummyRead(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth)
    : NVMeIoEx(devPath, slba, nlb, qDepth, 0, "")
{
    rep << LegacyString().sprintf("Dummy Read : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeTaskDummyRead::~NVMeTaskDummyRead(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
