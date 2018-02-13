//#include "nvme_device.h"
#include "file_operation.h"
#include "fop_write.h"
#include "legacy_string.h"
#include "memory.h"
//---------------------------------------------------------------------------
GlobalVariable FOPCmdWrite::_gVars[]=
{
    GlobalVariable("-slba", "start lba\n"
                   "  -slba=0x11223344  : HEX mode\n"
                   "  -slba=33          : DEC mode\n"
                   ,"0"),
    GlobalVariable("-nlb", "number of lba\n"
                   "  -nlb=0x11223344  : HEX mode\n"
                   "  -nlb=33          : DEC mode\n"
                   ,"0"),
    GlobalVariable("-file", "data write to file\n"
                   "  -file=raw\\write.bin : file=.\\raw\\write.bin (ref path with this tool)\n"
                   "  -file=D:\\write.bin  : absolute path\n"
                    ),
    GlobalVariable("-rawdev", "raw string for dev node path\n"
                   "  -rawdev=/dev/nvme1n1 : ioex function to /dev/nvme0n1 (same as -dev=1 -nsid=1)\n"
                   "  -rawdev=/dev/sda     : ioex function to /dev/sda\n"
                   "  *********** BE Careful for use option -rawdev *********** \n"
                   "  if use direct assign ioex fill function to OS disk, it may cause platform crash!!! \n"
                   ),
    GlobalVariable("-help", "show this page\n"
                   ),
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int
FOPCmdWrite::issue(uint8_t nthDev)
{
    int sc;
    DiskIoEx *diskIoEx;
    std::string path;
    if(_gVars[FOP_WRITE_RAW_DEV].enabled)
    {
        path=_gVars[FOP_WRITE_RAW_DEV].value();
    }
    else
    {
        path=LegacyString().sprintf("/dev/nvme%dn1", nthDev);
    }

    diskIoEx=new DiskIoEx(path);
    size_t byteCount=(__nlb+1)*IoExPlan::nlbByteSize;
    bool valid=true;
    valid_io_out_file_name(FOP_WRITE_FILE, byteCount, valid);
    if(valid)
    {
        char *wb=(char *)memalign(IoExPlan::nlbByteSize, byteCount);
        Memory<char> *memWrite;
        memWrite = new Memory<char>(wb);
        memWrite->load_from(_gVars[FOP_WRITE_FILE].value(), byteCount);
        sc=diskIoEx->io_write(__slba*IoExPlan::nlbByteSize, wb, byteCount);
        if(sc==(int)byteCount)
        {
            sc=0;
        }
        delete memWrite;
        free(wb);            
    }
    else
    {
        sc=-EINVAL;
    }    
    delete diskIoEx;
    return sc;
}
//---------------------------------------------------------------------------
bool
FOPCmdWrite::is_help(void)
{
    return _gVars[FOP_WRITE_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
FOPCmdWrite::is_valid(void)
{
    bool valid=true;
    __slba = var_to_int64(FOP_WRITE_SLBA, valid);
    __nlb = var_to_int64(FOP_WRITE_NLB, valid);
    if(valid)
    {
        printf("valid File Operation Write command : slba=0x%lX, nlb=0x%X\n", __slba, __nlb);
    }
    return valid;
}
//---------------------------------------------------------------------------
void
FOPCmdWrite::help_fop_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -fop=%s -file=write.bin\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. write 1 sector data to slba 0 and from .\\write.bin\n\n");
}
//---------------------------------------------------------------------------
FOPCmdWrite::FOPCmdWrite(int argc, char* argv[])
    : FOPCmd(argc, argv, FileOperation::cmd_string(FileOperation::FOP_WRITE), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
FOPCmdWrite::~FOPCmdWrite(void)
{
}
//---------------------------------------------------------------------------
