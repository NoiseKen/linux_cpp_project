/*
 * Copyright (c) 2011-2014, Ken Wang
 *
 */

#ifndef _KW_DEBUG_H_
#define _KW_DEBUG_H_

#include <linux/nvme.h>
#include <linux/pci.h>
#include <linux/kref.h>
#include <linux/blk-mq.h>
#include <linux/lightnvm.h>
#include <linux/nvme_ioctl.h>
#define TO_STR(n)   #n
#define KW_DEBUG(fmt, ...)    printk("\033[38;5;51m%s[%d] : \033[39m" fmt, __func__, __LINE__, ##__VA_ARGS__)

#define VER_MAJOR   0
#define VER_MINOR   0
#define VER_RELEASE 1

#define KW_DEBUG_VER    (VER_MAJOR<<16)|(VER_MINOR<<8)|(VER_RELEASE<<0)
#define PUT_KW_DEBUG_VER(to)    put_user(KW_DEBUG_VER, (uint32_t *)to);

//ken wang used debug opc used in Admin or IO command (0x
#define KW_DEBUG_OPC            0xc8
#define WRITE_PRP_ERR           (KW_DEBUG_OPC|nvme_cmd_write)   //0xc9
#define READ_PRP_ERR            (KW_DEBUG_OPC|nvme_cmd_read)    //0xca

typedef union{
    struct nvme_command nvmeCmd;
    struct {
	    __u8			    errOpc;
    	__u8			    flags;
	    __u16			    command_id;
	    __le32			    nsid;
	    __le32			    errCase;
        //cdw3  (index<<16)|(unalignOffset<<0), index for choice prp list entry index
        __le16              unalignOffset;
        __le16              index;
	    __le64			    metadata;
	    union nvme_data_ptr	dptr;
	    __le32			    cdw10[6];
    };
}KWDebugCommand;



enum{
    PRP_ERR_CASE_PRP1_PLUS_1,
    PRP_ERR_CASE_PRP1_PLUS_2,
    PRP_ERR_CASE_PRP1_PLUS_3,
    PRP_ERR_CASE_PRP2_NON_MPS_ALIGN,
    PRP_ERR_CASE_PRP2_LIST_ENTRY_ITEM_NON_MPS_ALIGN,
};

extern uint64_t prp_list_err_condition_check(int currentIdx, uint64_t prp, struct nvme_command *cmd, bool admin);
extern int data_ptr_error_injection(struct nvme_command *cmd, bool admin);

//please make sure nvme_ioctl.h the ioctl code not be ocuppied
#define NVME_IOCTL_GET_KW_DEBUG_VER	    _IOR('N', 0x99, uint32_t)

#endif /* _NVME_H */
