#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "nvme_ioctl.h"


int nvme_admin_raw(int fd, uint32_t *cdw, void *buffer, uint32_t length, uint32_t *cpldw, uint32_t timeout)
{
    int sc;
    struct nvme_passthru_cmd cmd;
    memset(&cmd, 0, sizeof(struct nvme_passthru_cmd));
    //copy CDW0~3 to command
    memcpy(&cmd, cdw, 4*sizeof(uint32_t));
    cmd.addr=(uint64_t)buffer;
    cmd.data_len=length;
    //copy CDW10~CDW15 to command
    memcpy(&cmd.cdw10, &cdw[10], 6*sizeof(uint32_t));
    cmd.timeout_ms=timeout;
    sc=ioctl(fd, NVME_IOCTL_ADMIN_CMD, &cmd);
    cpldw[0]=cmd.result;
    cpldw[3]=sc;
    return sc;
}

int nvme_io_raw(int fd, uint32_t *cdw, void *buffer, uint32_t length, uint32_t *cpldw, uint32_t timeout)
{
    int sc;
    struct nvme_passthru_cmd cmd;
    memset(&cmd, 0, sizeof(struct nvme_passthru_cmd));
    //copy CDW0~3 to command
    memcpy(&cmd, cdw, 4*sizeof(uint32_t));
    cmd.addr=(uint64_t)buffer;
    cmd.data_len=length;
    //copy CDW10~CDW15 to command
    memcpy(&cmd.cdw10, &cdw[10], 6*sizeof(uint32_t));
    cmd.timeout_ms=timeout;
    sc=ioctl(fd, NVME_IOCTL_IO_CMD, &cmd);
    cpldw[0]=cmd.result;
    cpldw[3]=sc;
    return sc;
}
        
