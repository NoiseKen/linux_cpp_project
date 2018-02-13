#include <errno.h>
#include <stdio.h>
#include <malloc.h>
#include "console_function.h"
#include "extension_string.h"
#include "sys_ctrl.h"
#include "file_ctrl.h"
#include "fun_action.h"
#include "fun_ioex.h"
#include "file_operation.h"
#include "disk_ioex.h"
#include "kw_ioctl.h"
#include "io_ctrl.h"
/*
#define EXIT_NOT_SUPPORT            -1
#define EXIT_INVALID_ARGUMENT       -2
#define EXIT_INVALID_FUNCTION       -3
#define EXIT_INVALID_PT_CMD         -4
#define EXIT_NO_NVME_DEVICE_EXIST   -5
*/
void usage(ConsoleFunction *fc, char *argv0)
{
    //TFileInfo *info;
    //info = new TFileInfo(AnsiString(argv0));
    //printf("Copyright of %s\n", info->Items[fiCompanyName]);
    //printf("%s, Ver %s\n", info->Items[fiProductName].c_str(), info->Items[fiFileVersion].c_str() );
    printf("Build Date : %s, %s\n\n", __DATE__, __TIME__);
    //delete info;
    printf("Usage : %s function=option\n", ConsoleArguments::appName.c_str());
    fc->show_var_help("valid function");
}

void init_app(char *argv0)
{
    ExtensionString *es;
    es = new ExtensionString("/");
    es->string = std::string(argv0);    
    *es >> ConsoleArguments::appName;
    ConsoleArguments::appRoot = es->string;
    if(ConsoleArguments::appRoot[0]=='.')
    {
        std::string localRoot;
        FileCtrl::get_current_root(localRoot);
        ConsoleArguments::appRoot.replace(0, 1, localRoot, 0, std::string::npos);
    }
    delete es;
}

int main(int argc, char *argv[])
{    
    int ret=0;
    init_app(argv[0]);
    ConsoleFunction *cf;
    cf=new ConsoleFunction(argc, argv);   
    if(!cf->is_valid())
    {
        usage(cf, argv[0]);
        ret = -EINVAL;
    }
    else
    {
        std::string cmd;
        bool valid=true;
        int numNVMeDevices;
        SysCtrl *sysCtrl;
        std::string cli;
        sysCtrl=new SysCtrl();
        //cli="ls -al /sys/class/nvme | grep nvme | wc -l";
        cli="ls -al /dev | grep nvme | grep crw | wc -l";
        ret=sysCtrl->exec_cmd(cli);
        if(ret==0)
        {
            cli=sysCtrl->string_rtrim(sysCtrl->stdout(), "\n");        
            numNVMeDevices=sysCtrl->string_to_int32(cli, valid);
        }
        else
        {
            valid=false;
        }

            
        if(!valid)
        {
            numNVMeDevices=0;
        }


        if(cf->is_list_device())
        {
            //cli="ls -al /sys/class/nvme | grep nvme";
            cli="ls -al /dev | grep nvme | grep crw";
            sysCtrl->exec_cmd(cli);
            printf("%s", sysCtrl->stdout().c_str());            
            printf("present NVMe Device=%d\n", numNVMeDevices);
            ret=numNVMeDevices;
        }
        else
        if(cf->is_driver_version())
        {
            uint32_t ver;
            IOCtrl *ioctl;
            char *path;
            int nth=cf->selected_nvme_device();
            path=new char[256];
            sprintf(path, "/dev/nvme%d", nth);
            ioctl=new IOCtrl(std::string(path), false);
            ret=ioctl->pass_through(NVME_IOCTL_GET_KW_DEBUG_VER, &ver);
            if(ret==0)
            {
                printf("KW DEBUG Driver : ver(0x%x)\n", ver);
            }
            else
            {
                printf("Current NVMe Driver not a KW DEBUG Driver!!\n");
            }
            delete path;
            delete ioctl;
        }
        else
        if(cf->action_command(cmd))
        {
            FunAction *act;
            printf("Action : %s\n", cmd.c_str());
            act = new FunAction(cmd);
            if(!act->is_valid())
            {
                printf("Invalid Action!!\n");
                act->list_supported_cmd();
                ret = -EINVAL;
            }
            else
            {
                int nth;
                nth = cf->selected_nvme_device();
                if(nth<numNVMeDevices)
                {
                    char *path;
                    path=new char[256];
                    sprintf(path, "/sys/class/nvme/nvme%d", nth); //device/resource0", nth);

                    //printf("path [%s]\n", path);
                    //for compatible with Shannon Open Channel Device, if defautl path not found, search venice path
                    FileCtrl *fc;
                    fc=new FileCtrl("");
                    if(!fc->is_exist(std::string(path)))
                    {
                        sprintf(path, "/sys/block/nvme%dn1", nth);
                    }

                    ret = act->exec(argc, argv, std::string(path));
                    delete path;
                    delete fc;
                }
                else
                {
                    printf("Avaliable device number=%d, Target device number=%d -- skip!!\n", numNVMeDevices, nth);
                    ret = -EINVAL;
                }
            }
            delete act;
        }
        else        
        if(cf->ioex_command(cmd))
        {
            FunIoEx *ioex;
            printf("IoEx : %s\n", cmd.c_str());
            ioex = new FunIoEx(cmd);
            if(!ioex->is_valid())
            {
                printf("Invalid IO Extension command!!\n");
                ioex->list_supported_cmd();
                ret = -EINVAL;
            }
            else
            {
                int nth;
                nth = cf->selected_nvme_device();
                if(nth<numNVMeDevices)
                {
                    //check direct io
                    //printf("Direct IO=%d\n", cf->is_direct_io());
                    IoExPlan::directIO=cf->is_direct_io();
                    ret = ioex->exec(argc, argv, nth, cf->debug_dump());
                }
                else
                {
                    printf("Avaliable device number=%d, Target device number=%d -- skip!!\n", numNVMeDevices, nth);
                    ret = -EINVAL;
                }
            }
            delete ioex;
        }
        else
        if(cf->fop_command(cmd))
        {
            FileOperation *fops;
            printf("FOP Command : %s\n", cmd.c_str());
            fops=new FileOperation(cmd);
            if(!fops->is_valid())
            {
                printf("Invalid fop command!!\n");
                fops->list_supported_cmd();
                ret = -EINVAL;
            }
            else
            {
                int nth;
                nth = cf->selected_nvme_device();
                if(nth<numNVMeDevices)
                {
                    ret = fops->exec(argc, argv, nth);
                }
                else
                {
                    printf("Avaliable device number=%d, Target device number=%d -- skip!!\n", numNVMeDevices, nth);
                    ret = EINVAL;
                }
                
            }
            delete fops;
        }

                
        delete sysCtrl;
    }

    delete cf;
    
    //if(ret!=0)
    //printf("Exit Code(0x%x)\n", ret);
    
    return ret;
}
