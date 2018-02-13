#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/nvme-local.h>
#include <linux/pci.h>
#include "ioctl_vendor.h"
static const char *log_path=LOG_FILE_PATH;
static struct file* 
file_open(const char* path, int flags, int rights)
{
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

static void 
file_close(struct file* file) 
{
    filp_close(file, NULL);
}
/*
static int 
file_write(struct file* file, unsigned long long offset, 
				unsigned char* data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}
*/
static int 
file_sync(struct file* file) 
{
    vfs_fsync(file, 0);
    return 0;
}

static int 
file_append(struct file *file, char *str )
{
	mm_segment_t oldfs;
    int ret; 

	loff_t pos;
	int size=0;
	
	do{
		++size;
	}
	while(str[size-1]!=0);
	
    oldfs = get_fs();
    set_fs(get_ds());    
	pos = vfs_llseek(file, 0, SEEK_END);	
    ret = vfs_write(file, str, size, &pos);	
    set_fs(oldfs);
    return ret;
}

unsigned long
log_us(int phase)
{
	static struct timeval tval[2];
	unsigned long val=0;

	if(phase<3)
	{
		if(phase==2)
		{
			val = (tval[1].tv_sec - tval[0].tv_sec) * 1000000;
			val += (tval[1].tv_usec - tval[0].tv_usec);			
		}
		else
		{
			do_gettimeofday(&tval[phase]);
		}
	}
	
	return val;
}



int 
log_out( char *str )
{
	struct file* fs;	
	fs = file_open(log_path, O_CREAT|O_WRONLY, 0777);
	file_append(fs, str);
	file_sync(fs);
	file_close(fs);
	return 0;
}

int 
log_sync_time( int num )
{
	char str[80];
	struct timeval t;
	struct tm broken;
	do_gettimeofday(&t);
	time_to_tm(t.tv_sec, 0, &broken);
	
	broken.tm_year+=1900;
	broken.tm_mon+=1;

	sprintf(str,"@ [%ld/%d/%d/%d:%d:%d] --- %d ---\n", broken.tm_year, broken.tm_mon, 
							broken.tm_mday, broken.tm_hour, broken.tm_min, 
							broken.tm_sec, num );
	log_out( str );						
	return 0;
}

int 
scan_pcie_dev(int vid, int did, AutoScanPci *list)
{
#if 0
	int cnt=-1;
	char str[80];
	struct pci_dev *dev;	
	dev = NULL;
	
	(vid==0)?(vid = PCI_ANY_ID):(vid=vid);
	(did==0)?(did = PCI_ANY_ID):(did=did);
	
	if(logout)
	{		
		fs = file_open(log_path, O_CREAT|O_WRONLY, 0777);
	}
		
	do{
		dev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, dev);
		cnt++;
			
		if(logout)
		{
			if(dev != NULL)
			{				
				sprintf(str, "%03d: VID=0x%04X, DID=0x%04X, class code = 0x%08X\n", cnt, dev->vendor, dev->device, dev->class);
				//log_out(str);
				file_append(fs, str);
			}
		}
		
 	}while(dev != NULL);
 	
 	if(logout)
 	{
		file_sync(fs);
		file_close(fs);
	}
#else
	int cnt=-1;	
	struct pci_dev *dev;	
	dev = NULL;

	(vid==0)?(vid=PCI_ANY_ID):(vid=vid);
	(did==0)?(did=PCI_ANY_ID):(did=did);

	do{
		dev = pci_get_device(vid, did, dev);
		cnt++;
		if(dev!=NULL)
		{
			list->info[cnt].vid = dev->vendor;
			list->info[cnt].did = dev->device;
			list->info[cnt].classCode = dev->class;			
		}
 	}while(dev != NULL);
 	list->cnt = cnt;	
#endif 	
 	return cnt;
}

struct pci_dev *
lock_pci_dev(unsigned int tNum, unsigned int vid, unsigned int did)
{
	int cnt=-1;	
	struct pci_dev *dev;
	dev = NULL;
	
	(vid==0)?(vid = PCI_ANY_ID):(vid=vid);
	(did==0)?(did = PCI_ANY_ID):(did=did);
	
	do{
		dev = pci_get_device(vid, did, dev);
		cnt++;
 	}while((dev != NULL)&&(cnt!=tNum));
	//printk("lock number %d, class code = 0x%08X\n", cnt, dev->class);
 	return dev;
}

