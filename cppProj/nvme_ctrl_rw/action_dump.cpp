#include "action_dump.h"
#include "fun_action.h"
#include "disk_ioex.h"
#include "utils.h"
#include "memory.h"
//---------------------------------------------------------------------------
GlobalVariable ActionDump::_gVars[]=
{
    GlobalVariable("-file", "output dump file to 16K controller register data\n"
                   "  -file=dump            : dump ctrl register memory to file ./dump\n"
                   "  -file=/home/user/d.bin: actual=/home/user/d.bin\n"
                   ,""),
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
ActionDump::issue(std::string path)
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
            Memory<int> *memDump;
            memDump=new Memory<int>((int *)membase);
            if(_dumpFile.empty())
            {
                std::string out;
                memDump->dump(0, memsize, out);
                printf("---------- DUMP CTRL REGISTER ---------- \n%s", out.c_str());
            }
            else
            {
                sc=memDump->save_to(_dumpFile, memsize);
                printf("Save Dump file to [%s]\n", _dumpFile.c_str());//, DiskIoEx::decode_sc(sc).c_str());
            }
            delete memDump;
            #if 0
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
            #endif
        }
        munmap(membase,memsize);
        close(pci_fd);        
    }
    return sc;
}
//---------------------------------------------------------------------------
bool
ActionDump::is_help(void)
{
    return _gVars[ACTION_DUMP_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
ActionDump::is_valid(void)
{
    bool valid=true;
    _dumpFile=valid_io_in_file_name(ACTION_DUMP_FILE, valid, true);
    /*
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
    }*/
    return valid;
}
//---------------------------------------------------------------------------
void
ActionDump::help_action_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -act=%s -dev=1\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. reset nvme device 1\n\n");
}
//---------------------------------------------------------------------------
ActionDump::ActionDump(int argc, char* argv[])
    : ActionBase(argc, argv, FunAction::cmd_string(FunAction::FA_DUMP), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
ActionDump::~ActionDump(void)
{
}
//---------------------------------------------------------------------------
