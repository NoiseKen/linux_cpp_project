#include <QString>
#include <errno.h>
#include "utils.h"
#include "nvme_ioctrl.h"

QString NVMeIOCtrl::passThroughDeviceNode="/dev/nvme0";
QString NVMeIOCtrl::blockDeviceNode="/dev/nvme0n1";

unsigned int NVMeIOCtrl::ioCmdID[PASS_THROUGH_TYPE_MAX]={
	NVME_IOCTL_VENDOR_CMD,
	NVME_IOCTL_ADMIN_CMD,
	NVME_IOCTL_SUBMIT_IO,
};

NVMeIOCtrl::CMDSet NVMeIOCtrl::adminCmdSet[]={
	{"Delete I/O submission queue (0x00)", nvme_admin_delete_sq},
	{"Create I/O submission queue (0x01)", nvme_admin_create_sq},
	{"Get log page (0x02)", nvme_admin_get_log_page},
	{"Delete I/O completion queue (0x04)", nvme_admin_delete_cq},
	{"Create I/O completion queue (0x05)", nvme_admin_create_cq},
	{"Identify (0x06)", nvme_admin_identify},
	{"Abort (0x08)", nvme_admin_abort_cmd},
	{"Set feature (0x09)", nvme_admin_set_features},
	{"Get feature (0x0A)", nvme_admin_get_features},
	{"Asynchronous event request (0x0C)", nvme_admin_async_event},
	{"Firmware active (0x10)", nvme_admin_activate_fw},
	{"Firmware image download (0x11)", nvme_admin_download_fw},
	{"Format NVM (0x80)", nvme_admin_format_nvm},
	{"Security Send (0x81)", nvme_admin_security_send},
	{"Security Receive (0x82)", nvme_admin_security_recv},
	{"Invalid command for test (0x31)", nvme_admin_invalid_test},
};

QString NVMeIOCtrl::submis_op_to_string(unsigned int submisop)
{
	QString sub;
	switch(submisop)
	{
		case nvme_cmd_flush:
			sub = TO_STR(nvme_cmd_flush); break;
		case nvme_cmd_write:
			sub = TO_STR(nvme_cmd_write); break;
		case nvme_cmd_read:
			sub = TO_STR(nvme_cmd_read); break;
		case nvme_cmd_write_uncor:
			sub = TO_STR(nvme_cmd_write_uncor); break;
		case nvme_cmd_compare:
			sub = TO_STR(nvme_cmd_compare); break;
		case nvme_cmd_dsm:
			sub = TO_STR(nvme_cmd_dsm); break;			
		default:
			sub = TO_STR(invalid_submis_command); break;
	}
	return sub;	
}

QString NVMeIOCtrl::admin_op_to_string(unsigned int adminop)
{
	QString adm;
	switch(adminop)
	{
		case nvme_admin_delete_sq:
			adm = TO_STR(nvme_admin_delete_sq); break;
		case nvme_admin_create_sq:
			adm = TO_STR(nvme_admin_create_sq); break;
		case nvme_admin_get_log_page:
			adm = TO_STR(nvme_admin_get_log_page); break;			
		case nvme_admin_delete_cq:
			adm = TO_STR(nvme_admin_delete_cq); break;
		case nvme_admin_create_cq:
			adm = TO_STR(nvme_admin_create_cq); break;
		case nvme_admin_identify:
			adm = TO_STR(nvme_admin_identify); break;
		case nvme_admin_abort_cmd:
			adm = TO_STR(nvme_admin_abort_cmd); break;
		case nvme_admin_set_features:
			adm = TO_STR(nvme_admin_set_features); break;
		case nvme_admin_get_features:
			adm = TO_STR(nvme_admin_get_features); break;
		case nvme_admin_async_event:
			adm = TO_STR(nvme_admin_async_event); break;
		case nvme_admin_activate_fw:
			adm = TO_STR(nvme_admin_activate_fw); break;
		case nvme_admin_download_fw:
			adm = TO_STR(nvme_admin_download_fw); break;
		case nvme_admin_format_nvm:
			adm = TO_STR(nvme_admin_format_nvm); break;
		case nvme_admin_security_send:
			adm = TO_STR(nvme_admin_security_send); break;
		case nvme_admin_security_recv:
			adm = TO_STR(nvme_admin_security_recv); break;
		default:
			adm = TO_STR(invalid_admin_command); break;
	}
	return adm;
}

void
NVMeIOCtrl::supported_admin_cmd_set(QComboBox *cbBox)
{
	int cnt;
	CMDSet *set;
	set = adminCmdSet;
	cnt = sizeof(adminCmdSet)/sizeof(CMDSet);
	cbBox->clear();
	cbBox->setMaxCount(cnt);	
	for(int i=0; i<cnt; i++)
	{			
		cbBox->addItem(set->descriptor, set->value );
		set++;
	}
	
	cbBox->setCurrentIndex(0);	
	//cbBox->setMaxVisibleItems(4);
}

int 
NVMeIOCtrl::lock_pci_device(unsigned int vid, unsigned int did, unsigned int targetNumber
					, bool init, int *classCode, HtmlString &msg)
{
#define cmdCBW	(&vendor->cbw.lockPciDev)
	struct nvme_vendor_cmd *vendor;
	int ioctlStatus;
	vendor = new struct nvme_vendor_cmd();
	memset(vendor, 0, sizeof(struct nvme_vendor_cmd));	
	vendor->cmd = IOCTL_VENDOR_LOCK_PCI;
	vendor->result = classCode;
	cmdCBW->targetNumber = targetNumber;
	cmdCBW->vendorId = vid;
	cmdCBW->deviceId = did;
	cmdCBW->initialize = init;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_DEV_VENDOR, vendor, msg);
	delete vendor;
	return ioctlStatus;
#undef cmdCBW
}

int
NVMeIOCtrl::get_pci_dev_list(unsigned int vid, unsigned int did, HtmlString &msg, AutoScanPci *list)
{
#define cmdCBW	(&vendor->cbw.scanPciDev)	
	struct nvme_vendor_cmd *vendor;
	int ioctlStatus;
	vendor = new struct nvme_vendor_cmd();
	memset(vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor->cmd = IOCTL_VENDOR_SCAN_PCI;
	vendor->result = list;
	cmdCBW->vendorId = vid;
	cmdCBW->deviceId = did;	
	ioctlStatus = nvme_pass_through(PASS_THROUGH_DEV_VENDOR, vendor, msg);
	delete vendor;
	return ioctlStatus;
#undef cmdCBW
}


int
NVMeIOCtrl::get_driver_version(int *version, HtmlString &msg)
{
	struct nvme_vendor_cmd *vendor;
	int ioctlStatus;
	vendor = new struct nvme_vendor_cmd();
	memset(vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor->cmd = IOCTL_VENDOR_GET_VERSION;
	vendor->result = version;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_DEV_VENDOR, vendor, msg);
	delete vendor;
	return ioctlStatus;
}

int
NVMeIOCtrl::get_controller_register(struct nvme_bar *bar, HtmlString &msg)
{
	int ioctlStatus;
	struct nvme_vendor_cmd *vendor;
	vendor = new struct nvme_vendor_cmd();
	memset(vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor->cmd = IOCTL_VENDOR_READ_NVME_CR;
	vendor->result = bar;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_DEV_VENDOR, vendor, msg);
	delete vendor;
	return ioctlStatus;
}

int
NVMeIOCtrl::auto_lock_first(unsigned int vid, unsigned int did, bool init, HtmlString &msg)
{
	int ioctlStatus;
	AutoScanPci *list;
	list = (AutoScanPci *)new char[8192];
	memset(list, 0xFF, 8192);
	ioctlStatus = get_pci_dev_list(vid, did, msg, list);	
	if(ioctlStatus >= 0)
	{//now find 1st NVMe dev and lock it		
		unsigned int i;	
		bool locked=false;
		
		if(list->cnt != 0)
		{
			i=0;				
			do{
				if(list->info[i].classCode==PCI_CLASS_STORAGE_EXPRESS)
				locked = true;
			}while(!locked&&(++i!=list->cnt));
		}
		
		if(locked)
		{
			int dummy;
			ioctlStatus = lock_pci_device(vid, did, i, init, &dummy, msg);
			if(ioctlStatus == 0)
			{
				msg.sprintf("locked %03d: VID=0x%04X, DID=0x%04X, class code = 0x%08X", i,
						list->info[i].vid, list->info[i].did, list->info[i].classCode);
			}
		}
		else
		{
			msg << "no NVMe deivce can be locked!!";
		}
	}	
	
	delete list;
	return ioctlStatus;
}

int
NVMeIOCtrl::config_admin_queue(HtmlString &msg)
{
	int ioctlStatus;
	struct nvme_vendor_cmd *vendor;
	vendor = new struct nvme_vendor_cmd();
	memset(vendor, 0, sizeof(struct nvme_vendor_cmd));				
	vendor->cmd = IOCTL_VENDOR_CONFIG_ADMQ;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_DEV_VENDOR, vendor, msg);
	if(ioctlStatus == 0 )
	{
		msg << "controller admin queue init success!!";
	}
	delete vendor;
	return ioctlStatus;
}



int
NVMeIOCtrl::nvme_pass_through(ValidPassThroughType type, void *cbw, HtmlString &msg)
{
	int status=-EINVAL;	
	if(type<PASS_THROUGH_TYPE_MAX)
	{
		//QString node;		
		IOCtrl *ioCtrl;
		status =0;

		//ioCtrl = new IOCtrl(passThroughDeviceNode.toStdString());
		if(type==PASS_THROUGH_SUBMIS_CMD)
		{
			ioCtrl = new IOCtrl(blockDeviceNode.toStdString());
		}
		else
		{
			ioCtrl = new IOCtrl(passThroughDeviceNode.toStdString());
		}
	
		
		
		if (!ioCtrl->bConnected)
		{	
			msg << QString("open node %1 failed").arg(passThroughDeviceNode);
			
			status = -1;
		} 
		else
		{
			status = ioCtrl->pass_through(ioCmdID[type], cbw);			
			if(status < 0)
			{
				lastErrno = errno;
				msg << QString("IOCTL issue fail, errno = %1\n").arg(lastErrno);
				switch(lastErrno)
				{
					case EACCES:
						msg << "Do not try to lock multi NVMe device, this App only can lock one device.\n";
						break;
					case ENXIO:
						msg << "No such device or address.\n";
						break;
					case EEXIST:
						msg << "Do not try to init admin queue again, system will crash.\n";
						break;						
				}
			}
		}

		delete ioCtrl;
    }
    
    return status;		
}

void 
NVMeIOCtrl::report_status_code(uint16_t sc, HtmlString &report)
{
	HtmlString::ColorStr cstr;
	QString d;
	
	report << d.sprintf("status code = 0x%04X ==> ", sc);
	
	if(sc & (1<<14))
	{
		report << "DO_NOT_RETRY, ";
	}

	if(sc & (1<<13))
	{
		report << "MORE, ";
	}	
	
	sc &= 0x7FF;
	
	switch(sc)
	{
		case NVME_SC_SUCCESS:
			cstr.color = HtmlString::colorGreen;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_SUCCESS));
			report << cstr;
			break;
		case NVME_SC_INVALID_FIELD:						
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_INVALID_FIELD));
			cstr.string += d.sprintf("invalid parameter involved!!\n");						
			report << cstr;
			break;			
		case NVME_SC_ABORT_REQ:			
			report << d.sprintf("%s\n", TO_STR(NVME_SC_ABORT_REQ));
			break;
		case NVME_SC_INVALID_NS:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_INVALID_NS));
			cstr.string += d.sprintf("invalid namespace involved!!\n");
			report << cstr;
			break;
		case NVME_SC_QID_INVALID:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_QID_INVALID));
			cstr.string += d.sprintf("invalid queue id number involved!!\n");
			report << cstr;
			break;
		case NVME_SC_CQ_INVALID:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_CQ_INVALID));
			cstr.string += d.sprintf("invalid completion queue number involved!!\n");
			report << cstr;
			break;
		case NVME_SC_QUEUE_SIZE:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_QUEUE_SIZE));
			cstr.string += d.sprintf("invalid queue size involved!!\n");
			report << cstr;
			break;
		case NVME_SC_INVALID_OPCODE:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_INVALID_OPCODE));
			cstr.string += d.sprintf("invalid opcode invloved!!\n");
			report << cstr;
			break;
		case NVME_SC_INVALID_LOG_PAGE:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_INVALID_LOG_PAGE));
			cstr.string += d.sprintf("invalid log page id involved!!\n");
			report << cstr;
			break;
		case NVME_SC_INVALID_CQ_DELETE:
			cstr.color = HtmlString::colorRed;
			cstr.string  = d.sprintf("%s\n", TO_STR(NVME_SC_INVALID_CQ_DELETE));
			cstr.string += d.sprintf("invalid completion queue delete!!\n");
			cstr.string += d.sprintf("this CQ still used by some SQ(s), please delete that SQ(s) first!!\n");
			report << cstr;
			break;			
		default:
			report << d.sprintf("please refer spec\n");
			break;			
	}
		
}

void 
NVMeIOCtrl::report_completion(int status, HtmlString &report)
{	
	if(status>=0)
	{
		report_status_code(status, report);
	}
}


NVMeIOCtrl::NVMeIOCtrl(void)
{
}

NVMeIOCtrl::~NVMeIOCtrl(void)
{
}
