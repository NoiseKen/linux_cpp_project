/*
 * NVM Express device driver
 * Ken Wang Debug use
 */

#include <linux/aer.h>
#include <linux/bitops.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/blk-mq-pci.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/idr.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/poison.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/t10-pi.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/io-64-nonatomic-lo-hi.h>
#include <asm/unaligned.h>

#include "../nvme.h"

#include "kw_debug.h"

//static uint32_t debugStatus=0;
#define ERR_CASE(n)    case n:  KW_DEBUG("case : %s\n", TO_STR(n))

static uint64_t
prp_list_err_case(uint64_t prp, KWDebugCommand *cmd)
{    
    //prp&=~(ctrl->page_size-1);
    prp|=cmd->unalignOffset;
    return prp;
}

static void
prp_err_case(KWDebugCommand *cmd)
{

    struct nvme_command *nvmeCmd=&cmd->nvmeCmd;
    KW_DEBUG("B : prp1=0x%llx, prp2=0x%llx\n", nvmeCmd->common.dptr.prp1, nvmeCmd->common.dptr.prp2);
    switch(cmd->errCase)
    {
        ERR_CASE(PRP_ERR_CASE_PRP1_PLUS_1);
            nvmeCmd->common.dptr.prp1+=1;
            break;
        ERR_CASE(PRP_ERR_CASE_PRP1_PLUS_2);
            nvmeCmd->common.dptr.prp1+=2;
            break;
        ERR_CASE(PRP_ERR_CASE_PRP1_PLUS_3);
            nvmeCmd->common.dptr.prp1+=3;
            break;
        ERR_CASE(PRP_ERR_CASE_PRP2_NON_MPS_ALIGN);
            //prp2 shall all MPS align so, we don't need fix it again
            //nvmeCmd->common.dptr.prp2&=~(ctrl->page_size-1);
            nvmeCmd->common.dptr.prp2|=cmd->unalignOffset;
            break;
        //ERR_CASE(PRP_ERR_CASE_PRP2_LIST_ENTRY_ITEM_NON_MPS_ALIGN); 
            //here can not modify prp entry any more, move to prp_list_err_condition_check
            //listItem=(uint64_t *)nvmeCmd->common.dptr.prp2;
            //KW_DEBUG("listItem[%d]=0x%llx\n", cmd->index, listItem[cmd->index]);
            //KW_DEBUG("ENTRY[%d][0x%llx]=0x%llx\n", cmd->index, (uint64_t)&listItem[cmd->index], listItem[cmd->index]);
            //nvmeCmd->common.dptr.prp2&=~(ctrl->page_size-1);
            //nvmeCmd->common.dptr.prp2|=cmd->unalignOffset;
            //break;
        default:
            break;
    }
    KW_DEBUG("A : prp1=0x%llx, prp2=0x%llx\n", nvmeCmd->common.dptr.prp1, nvmeCmd->common.dptr.prp2);    
    
    //back to normal command
    //nvmeCmd->common.cdw2[0]=0;
    cmd->errCase=0;
}

uint64_t
prp_list_err_condition_check(int currentIdx, uint64_t prp, struct nvme_command *nvmeCmd, bool admin)
{
    if(admin)
    {
    }
    else
    {
        KWDebugCommand *cmd=(KWDebugCommand *)nvmeCmd;
        switch(nvmeCmd->common.opcode)
        {
            case READ_PRP_ERR:
            case WRITE_PRP_ERR:
                if(    (PRP_ERR_CASE_PRP2_LIST_ENTRY_ITEM_NON_MPS_ALIGN==cmd->errCase)
                    && (currentIdx==cmd->index))
                {
                    KW_DEBUG("case : %s\n", TO_STR(PRP_ERR_CASE_PRP2_LIST_ENTRY_ITEM_NON_MPS_ALIGN));
                    KW_DEBUG("B : prp2 entry[%d]=0x%llx\n", currentIdx, prp);
                    prp=prp_list_err_case(prp, cmd);
                    KW_DEBUG("A : prp2 entry[%d]=0x%llx\n", currentIdx, prp);
                }
                break;
            default:
                break;
        }
    }
    return prp;
}

int 
data_ptr_error_injection(struct nvme_command *cmd, bool admin)
{
    if(admin)
    {
    }
    else
    {
        switch(cmd->common.opcode)
        {
            case READ_PRP_ERR:
                prp_err_case((KWDebugCommand *)cmd);
                cmd->common.opcode=nvme_cmd_read;
                cmd->common.cdw2[0]=cmd->common.cdw2[1]=0;
                break;
            case WRITE_PRP_ERR:
                prp_err_case((KWDebugCommand *)cmd);
                cmd->common.opcode=nvme_cmd_write;
                cmd->common.cdw2[0]=cmd->common.cdw2[1]=0;
                break;
            default:
                break;
        }
    }
    return 0;
}

