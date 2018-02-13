/*
 * Copyright (c) 2011-2014, Ken Wang
 *
 */

#ifndef _KW_IOCTL_H_
#define _KW_IOCTL_H_
#include <linux/lightnvm.h>
#include <linux/nvme_ioctl.h>
//please make sure nvme_ioctl.h the ioctl code not be ocuppied
#define NVME_IOCTL_GET_KW_DEBUG_VER	    _IOR('N', 0x99, uint32_t)
#endif /* _NVME_H */
