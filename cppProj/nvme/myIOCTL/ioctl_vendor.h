#ifndef _IOCTL_VENDOR_H_
#define _IOCTL_VENDOR_H_

#define PCI_CLASS_STORAGE_EXPRESS	0x010802

#define VER(major,minor,build)	(((major&0xFF)<<24)|((minor&0xFF)<<16)|(build &0xFFFF))

//BUILD_VERSION will auto increase do not modify it!!!!
#define BUILD_VERSION   41
//BUILD_VERSION will auto increase do not modify it!!!!

#define IOCTL_APP_VER		VER(0, 0, BUILD_VERSION)

#define TO_STR(s)	(#s)

#define LOG_FILE_PATH	"/var/log/nxgn.log"

typedef struct auto_scan_pci{
	unsigned int cnt;
	struct{
	unsigned short vid;
	unsigned short did;
	unsigned int classCode;
	}info[];
}__attribute__((packed)) AutoScanPci;

typedef struct nvme_vendor_cmd{
	//user to kernel space
	unsigned int cmd;
	union vendor_cbw{
		//IOCTL_VENDOR_SCAN_PCIE
		struct pci_dev_scan{
			unsigned int vendorId;
			unsigned int deviceId;			
		}scanPciDev;
		//IOCTL_VENDOR_LOCK_PICE
		struct pci_dev_lock{
			bool initialize;	//true, if want to run init flow (original probe)
			unsigned int vendorId;
			unsigned int deviceId;
			unsigned int targetNumber;
		}lockPciDev;
	}cbw;
	
	void *result;
}NvmeVendorCmd;

#define NVME_IOCTL_VENDOR_CMD	_IOWR('N', 0x50, struct nvme_vendor_cmd)

//struct nvme_vendor_cmd.cmd
typedef enum{
	IOCTL_VENDOR_LOG_TIME=0,
	IOCTL_VENDOR_GET_VERSION,
	IOCTL_VENDOR_SCAN_PCI,	
	IOCTL_VENDOR_LOCK_PCI,
	IOCTL_VENDOR_READ_NVME_CR,
	IOCTL_VENDOR_WRITE_NVME_CR,
	IOCTL_VENDOR_CONFIG_ADMQ,
	IOCTL_VENDOR_TEST,
	IOCTL_VENDOR_CMD_MAX,
}ValidVendorCmd;

#endif//ndef _IOCTL_VENDOR_H_
