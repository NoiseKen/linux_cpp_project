#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <scsi/sg.h> /* take care: fetches glibc's /usr/include/scsi/sg.h */

typedef enum{
    XFER_OUT=0,
    XFER_IN,
    NON_DATA
}XfertType;

int execute_scsi_command(
        int fd, XfertType xferType,
        int lenCDB,     uint8_t* ptrCDB,
        int lenXferBuf, uint8_t* ptrXferBuf,
        int lenSense,   uint8_t* ptrSense)
{
    int sc;
    sg_io_hdr_t io_hdr;

    //Prepare SCSI Command
    memset(&io_hdr, 0, sizeof(sg_io_hdr_t));

    io_hdr.interface_id = 'S'; //required

    io_hdr.cmd_len = lenCDB;
    io_hdr.mx_sb_len = lenSense;

    /*
     *  SG_DXFER_NONE   -   SCSI Test Unit Ready command
     *  SG_DXFER_TO_DEV -   SCSI WRITE command
     *  SG_DXFER_FROM_DEV - SCSI READ command
     */
    switch(xferType)
    {
        case XFER_OUT:
            io_hdr.dxfer_direction = SG_DXFER_TO_DEV;
            break;
        case XFER_IN:
            io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
            break;
        default:
        //case NON_DATA:
            io_hdr.dxfer_direction = SG_DXFER_NONE;
            break;
    }
    io_hdr.dxfer_len       = lenXferBuf;
    io_hdr.dxferp          = ptrXferBuf;

    io_hdr.cmdp = ptrCDB;
    io_hdr.sbp  = ptrSense;
    io_hdr.timeout = 20000; // 20000 millisecs = 20 seconds
    /* io_hdr.flags = 0; */     /* take defaults: indirect IO, etc */
    /* io_hdr.pack_id = 0; */
    /* io_hdr.usr_ptr = NULL; */

    if (ioctl(fd, SG_IO, &io_hdr) < 0) {
        //perror("sg_simple0: Inquiry SG_IO ioctl error");
        sc=errno;
        printf("SGIO Fail: %s\n", strerror(sc));
        return -sc;
    }

    return io_hdr.resid;
}
