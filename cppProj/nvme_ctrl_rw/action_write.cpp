//#include <errno.h>
#include "action_write.h"
#include "fun_action.h"
#include "utils.h"
//---------------------------------------------------------------------------
GlobalVariable ActionWrite::_gVars[]=
{
    GlobalVariable("-addr", "register address\n"
                   "  -addr=0x1c    : HEX mode, read CSTS\n"
                   "  -addr=0       : DEC mode, read CAP\n"
                   ,"0"),
    GlobalVariable("-val", "write value\n"
                   "  -val=0x1c     : HEX mode, write 0x1c\n"
                   "  -val=0        : DEC mode, write zero\n"
                   ,"0"),                   
    GlobalVariable("-len", "write type, only valid when 1, 2, 4\n"
                   "  -len=1        : write 1 byte\n"
                   "  -len=4        : write 4 byte\n"
                   ,"0"),
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
ActionWrite::issue(std::string path)
{
    int sc=0;
    int pci_fd;
	
    path+="/device/resource0";
    
    pci_fd = open(path.c_str(), O_RDWR);
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
        
	    membase = mmap(NULL, memsize, PROT_READ|PROT_WRITE, MAP_SHARED, pci_fd, 0);
	    if ((membase == MAP_FAILED)||(membase==NULL))
        {
		    fprintf(stdout, "[%s] failed to map to ctrl register space, errno=%d!!\n", path.c_str(), errno);
		    sc=-1;
	    }
        else
        {   
            uint64_t backup;
            backup=bar0mapping;
            //printf("base=0x%x\n", bar0mapping);
            bar0mapping+=__addr;
            //printf("matto=0x%x\n", bar0mapping);
            switch(__len)
            {
                case 1:
                    printf("Set CtrlRegs[0x%x]=0x%x\n", __addr, __val8);
                    *(uint8_t *)(membase)=__val8;
                    break;                
                case 2:
                    printf("Set CtrlRegs[0x%x]=0x%x\n", __addr, __val16);
                    *(uint16_t *)(membase)=__val16;
                    break;
                case 4:
                    printf("Set CtrlRegs[0x%x]=0x%x\n", __addr, __val32);
                    *(uint32_t *)(membase)=__val32;
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
ActionWrite::is_help(void)
{
    return _gVars[ACTION_WRITE_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
ActionWrite::is_valid(void)
{
    bool valid=true;
    
    __addr=var_to_int32(ACTION_WRITE_ADDR, valid);
    __val32=var_to_int32(ACTION_WRITE_VAL, valid);
    __len=var_to_int32(ACTION_WRITE_LEN, valid);

    if(valid)
    {
        if((__len!=1)&&(__len!=2)&&(__len!=4))
        {
            printf("[%s] argument with invalid value %d\n", _gVars[ACTION_WRITE_LEN].keyWord.c_str(), __len);
            valid=false;
        }

        if(valid)
        {
            printf("valid Action Write\n");
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
ActionWrite::help_action_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -act=%s -addr=0x14 -len=4 -val=0\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. Write Ctrl Register : CC=0\n\n");

}
//---------------------------------------------------------------------------
ActionWrite::ActionWrite(int argc, char* argv[])
    : ActionBase(argc, argv, FunAction::cmd_string(FunAction::FA_READ), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
ActionWrite::~ActionWrite(void)
{
}
//---------------------------------------------------------------------------
