//#include "nvme_device.h"
#include "file_operation.h"
#include "fop_read.h"
#include "legacy_string.h"
//---------------------------------------------------------------------------
GlobalVariable FOPCmdRead::_gVars[]=
{
    GlobalVariable("-slba", "start lba\n"
                   "  -slba=0x11223344  : HEX mode\n"
                   "  -slba=33          : DEC mode\n"
                   ,"0"),
    GlobalVariable("-nlb", "number of lba\n"
                   "  -nlb=0x11223344  : HEX mode\n"
                   "  -nlb=33          : DEC mode\n"
                   ,"0"),
    GlobalVariable("-file", "data read to file\n"
                   "  -file=raw/read.bin : file=./raw/read.bin (ref path with this tool)\n"
                   "  -file=/tmp/read.bin  : absolute path\n"
                    ),
    GlobalVariable("-rawdev", "raw string for dev node path\n"
                   "  -rawdev=/dev/nvme1n1 : ioex function to /dev/nvme0n1 (same as -dev=1 -nsid=1)\n"
                   "  -rawdev=/dev/sda     : ioex function to /dev/sda\n"
                   "  *********** Be Careful for use option -rawdev *********** \n"
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
void
FOPCmdRead::post(void)
{//
    uint32_t byteCount;
    byteCount=(__nlb+1)*IoExPlan::nlbByteSize;
    if(!post_ds_to_file(FOP_READ_FILE, byteCount, readBuffer))
    {
        std::string line;
        LegacyString *extra;
        extra=new LegacyString();
        line=" --- FOP Read --- ";
        uint8_t *ptr=(uint8_t *)readBuffer;
        uint32_t dsSize=byteCount;
        int col=0, row=0;
        while(dsSize--)
        {
            if(col==0)
            {
                line+=extra->sprintf("\n[%04XH] : ", row);
                row += 16;
            }
            line+=extra->sprintf("%02X ", *ptr++);

            col = (col+1)%16;
        }
        printf("%s\n", line.c_str());
        delete extra;
    }
}
//---------------------------------------------------------------------------
int
FOPCmdRead::issue(uint8_t nthDev)
{
    int sc;
    DiskIoEx *diskIoEx;
    std::string path;
    if(_gVars[FOP_READ_RAW_DEV].enabled)
    {
        path=_gVars[FOP_READ_RAW_DEV].value();
    }
    else
    {
        path=LegacyString().sprintf("/dev/nvme%dn1", nthDev);
    }

    diskIoEx=new DiskIoEx(path);
    size_t byteCount=(__nlb+1)*IoExPlan::nlbByteSize;
    readBuffer=(char *)memalign(IoExPlan::nlbByteSize, byteCount);
    memset(readBuffer, 0xff, byteCount);
    sc=diskIoEx->io_read(__slba*IoExPlan::nlbByteSize, readBuffer, byteCount);
    if(sc==(int)byteCount)
    {
        sc=0;
    }

    delete diskIoEx;
    return sc;    
}
//---------------------------------------------------------------------------
bool
FOPCmdRead::is_help(void)
{
    return _gVars[FOP_READ_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
FOPCmdRead::is_valid(void)
{
    bool valid=true;

    __slba = var_to_int64(FOP_READ_SLBA, valid);
    __nlb = var_to_int32(FOP_READ_NLB, valid);

    if(valid)
    {
        valid_io_in_file_name(FOP_READ_FILE, valid);
        if(valid)
        {
            printf("valid File Operation Read command : slba=0x%lX, nlb=0x%X\n", __slba, __nlb);
            //readBuffer=new char[(nlb+1)*attr->lbaSize];
            //FillMemory(readBuffer,(nlb+1)*attr->lbaSize, 0xff);
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
FOPCmdRead::help_fop_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -fop=%s -file=read.bin\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. read 1 sector data from slba 0 and store to .\\read.bin\n\n");
    printf(" %s -fop=%s -slba=123 -nlb=2\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. read 3 sector data from slba 123 adn just dump to stdout\n\n");
}
//---------------------------------------------------------------------------
FOPCmdRead::FOPCmdRead(int argc, char* argv[])
    : FOPCmd(argc, argv, FileOperation::cmd_string(FileOperation::FOP_READ), REGISTER(_gVars))
{
    readBuffer=NULL;
}
//---------------------------------------------------------------------------
FOPCmdRead::~FOPCmdRead(void)
{
    if(readBuffer!=NULL)
    {
        free(readBuffer);
    }
}
//---------------------------------------------------------------------------
