#include "action_read.h"
#include "fun_action.h"
#include "utils.h"
//---------------------------------------------------------------------------
GlobalVariable ActionRead::_gVars[]=
{
    GlobalVariable("-addr", "register address\n"
                   "  -addr=0x1c    : HEX mode, read CSTS\n"
                   "  -addr=0       : DEC mode, read CAP\n"
                   ,"0"),
    GlobalVariable("-len", "read type, only valid when 1, 2, 4, 8\n"
                   "  -len=8        : read 8 byte\n"
                   "  -len=4        : read 4 byte\n"
                   ,"4"),
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
ActionRead::issue(std::string path)
{
    int sc=0;
    int pci_fd;

    path+="/device/resource0";
    
    pci_fd = open(path.c_str(), O_RDONLY);
    if (pci_fd < 0)
    {
        fprintf(stdout, "[%s] did not find a pci resource\n", path.c_str());
        sc=-1;
    }
    else
    {
        union{
        void *membase;
        uint64_t bar0mapping;
        };

        uint32_t memsize=16*1024; //getpagesize();
        
        membase = mmap(NULL, memsize, PROT_READ, MAP_PRIVATE, pci_fd, 0);
        if ((membase == MAP_FAILED)||(membase==NULL))
        {
            fprintf(stdout, "[%s] failed to map to ctrl register space, errno=%d!!\n", path.c_str(), errno);
            sc=-1;
        }
        else
        {
            union{
            uint64_t d64;
            uint32_t d32;
            uint16_t d16;
            uint8_t data;
            };
            uint64_t backup;
            backup=bar0mapping;
            //printf("base=0x%x\n", bar0mapping);
            bar0mapping+=__addr;
            //printf("matto=0x%x\n", bar0mapping);
            switch(__len)
            {
                case 1:
                    data = mmio_read8(membase);
                    printf("CtrlRegs[0x%x]=0x%x\n", __addr, data);
                    break;                
                case 2:
                    d16 = mmio_read16(membase);
                    printf("CtrlRegs[0x%x]=0x%x\n", __addr, d16);
                    break;
                case 4:
                    d32 = mmio_read32(membase);
                    printf("CtrlRegs[0x%x]=0x%x\n", __addr, d32);
                    break;
                case 8:
                    d64 = mmio_read64(membase);
                    printf("CtrlRegs[0x%x]=0x%lx\n", __addr, d64);
                    break;
            }
            bar0mapping=backup;
        }
        munmap(membase,memsize);
        close(pci_fd);        
    }

    return sc;
}
//---------------------------------------------------------------------------
bool
ActionRead::is_help(void)
{
    return _gVars[ACTION_READ_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
ActionRead::is_valid(void)
{
    bool valid=true;
    
    __addr=var_to_int32(ACTION_READ_ADDR, valid);
    __len=var_to_int32(ACTION_READ_LEN, valid);

    if(valid)
    {
        if((__len!=1)&&(__len!=2)&&(__len!=4)&&(__len!=8))
        {
            printf("[%s] argument with invalid value %d\n", _gVars[ACTION_READ_LEN].keyWord.c_str(), __len);
            valid=false;
        }

        if(valid)
        {
            printf("valid Action Read\n");
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
ActionRead::help_action_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -act=%s -addr=0 -len=8\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. Read Ctrl Register : CAP\n\n");
    printf(" %s -act=%s -addr=0x1c -len=4\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. Read Ctrl Register : CSTS\n\n");
}
//---------------------------------------------------------------------------
ActionRead::ActionRead(int argc, char* argv[])
    : ActionBase(argc, argv, FunAction::cmd_string(FunAction::FA_READ), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
ActionRead::~ActionRead(void)
{
}
//---------------------------------------------------------------------------
