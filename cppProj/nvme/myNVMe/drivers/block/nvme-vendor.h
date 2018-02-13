#ifndef _NVME_VENDOR_H_
#define _NVME_VENDOR_H_

//#define DEBUG_MSG
#ifdef DEBUG_MSG
#define debug(...) printk(__VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif //def DEBUG_MSG

#define PRINTK_FUNCTION_NAME debug("NXGN : %s\n", __func__)

extern unsigned long log_us(int phase);
extern int log_sync_time(int num);
extern int log_out( char *str );
//extern int log_nvme_info(struct nvme_dev *dev);
extern int read_nvme_ctrl_register(struct nvme_bar *cr, struct nvme_dev *dev);
extern int log_clr(void);
extern int scan_pcie_dev(int vid, int did, void *list);
extern struct pci_dev * lock_pci_dev(unsigned int tNum, unsigned int vid, unsigned int did);
#endif//ndef _NVME_VENDOR_H_
